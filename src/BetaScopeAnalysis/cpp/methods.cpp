//////////////////////////////////////
////methods implementation          //
////                                //
//////////////////////////////////////

#include "../hpp/betaAnalysis.h"

void betaAnalysis::base_line(int npoints, std::vector<double>& w, int m_inoise)
{
	double mean =0;

	for(int j = 0; j < m_inoise; j++){mean += w[j];}
	
	mean = mean/m_inoise;

	for(int j = 0; j < npoints; j++){w[j] = w[j]- mean;}
} 

bool betaAnalysis::pulse_baseline(std::vector<double> w, std::pair<double, double>& baseline, std::pair<double, double>& noise_rms, int m_inoise)

{

  baseline = std::make_pair(-1, -1);

  noise_rms = std::make_pair(-1, -1);

  double mean = 0.0;

  double strd = 0.0;

  for (int j = 0; j < m_inoise; j++) mean += w[j];

  mean = mean / m_inoise;

  for (int i = 0; i < m_inoise; ++i) strd += pow(w[i] - mean, 2);

  strd = sqrt(strd / m_inoise);

  double limit1 = ceil((mean - 8*strd)*1000)/1000;

  double limit2 = ceil((mean + 8*strd)*1000)/1000;

  int bins = ceil(16*strd*1000);

  TH1D* noise = new TH1D("PulseNoise", "PulseNoise", bins, limit1, limit2);

  for (int i = 0; i<m_inoise; i++) noise->Fill(w[i]);

  double m = 0.0;

  double dm = 0.0;

  double sigma = 0.0;

  double dsigma = 0.0;

  if (noise->Integral()>0)

     {

      double mx = noise->GetMean();

      double rms = noise->GetRMS();

      double rmin = mx - 3 * rms;

      double rmax = mx + 3 * rms;

      TF1* mygau = new TF1("mygau", "gaus", rmin, rmax);

      noise->Fit(mygau,"Q0");

      m = mygau->GetParameter(1);

      dm = mygau->GetParError(1);

      sigma = sqrt(mygau->GetParameter(2));

      dsigma = 0.5*pow(mygau->GetParameter(2), -0.5)*mygau->GetParError(2);

      mygau->Delete();

     }

  noise->Delete();

  baseline = std::make_pair(m, dm);

  noise_rms = std::make_pair(sigma, dsigma);

  return true;

}

//---------------------For multiple identical peaks-----------------------------

std::pair <double, int> betaAnalysis::multiplePeaks( int npoints, std::vector<double> w, std::vector<double> t, int StartIndex, int TimeRestriction_OnOff, double TimeRestrictionRange)
{ 	
    double pmax        = 0.0;
    
    int    pmax_index  = StartIndex;
    
    bool   StrangePeak = true;
    
    bool   FirstPoint  = true; 
    
    //std::cout << "StartIndex: " << StartIndex << std::endl;
    
    if(StartIndex)
    {    
    	if(TimeRestriction_OnOff)
    	{
    		for( int j = StartIndex; j < npoints; j++)
    		{
    		
    			if (j == StartIndex)
				{
       				pmax = w[j];
        			
        			pmax_index = j;
        			
        			FirstPoint = false;
       			}
       			
    		
    			if (t[j] > -TimeRestrictionRange && t[j] < TimeRestrictionRange) //zoom in to find the Pmax
    			{
        			if (j != StartIndex &&  w[j] == pmax)
        			{
        				pmax = w[j];
        			
        				pmax_index = j;
        				
        				StrangePeak = false;
        			}
    			}
    		}
    		
    		if(StrangePeak && FirstPoint) //previous sick point will be sent to here.
    		{
    			pmax = 10000.0;
        			
        		pmax_index = npoints;
        	}
    	}
    
		else
		{
			for ( int j = StartIndex; j < npoints; j++)
			{
		    	if (j == StartIndex)
		    	{
		    		pmax = w[j];
		    			
		    		pmax_index = j;
		    	}
		    	
		    	if (j != StartIndex && w[j] == pmax)
		    	{
		    		pmax = w[j];
		    			
		    		pmax_index = j;
				}
			} 	
		}
	}
	
	else
	{    
    	if(TimeRestriction_OnOff)
    	{
    		for( int j = 0; j < npoints; j++)
    		{
    			if (t[j] > -TimeRestrictionRange && t[j] < TimeRestrictionRange) //zoom in to find the Pmax
    			{
					if ( FirstPoint && w[j] > 0 )
		    		{
		    			pmax = w[j];
		    			
		    			pmax_index = j;
		    			
		    			FirstPoint = false;
		    			
		    			StrangePeak = false;
		    		}
    			
    			
        			if ( w[j] > pmax )
        			{
        				pmax = w[j];
        			
        				pmax_index = j;
        			}
    			}
    		}
    		
    		if (StrangePeak) //if it cannot find any positive point within the range, return this number
    		{
    			pmax = 10000.0;
    			
    			pmax_index = npoints;
    		}
    	}
    
		else
		{
			for ( int j = 0; j < npoints; j++)
			{
				//std::cout << "here increment " << j << std::endl;
		    	
		    	if (j == 0)
		    	{
		    		pmax = 0.0;
		    			
		    		pmax_index = 0;
		    	}
		    	
		    	if (j != 0 && w[j] > pmax)
		    	{
		    		pmax = w[j];
		    			
		    		pmax_index = j;
				}
			} 	
		}
	}
    
    //std::cout << "pmax_index: " << pmax_index << std::endl;
    
    return std::make_pair (pmax, pmax_index);
}

//----------------------------------------------------------------------------

//-----------------Simple function to give the time at index ----------------------------------------

double betaAnalysis::TimeAtIndex (std::vector<double> t, int index)
{
	return t.at(index);
}

//----------------------------------------------------------------------------


//-----------------------Pmax with index -----------------------------------------------------
std::pair<double, int> betaAnalysis::pulse_max_with_index_fixed( const int npoints, 
                                                   
                                                   std::vector<double> w, 
                                                   
                                                   std::vector<double> t, 
                                                   
                                                   const int    TimeRestriction_OnOff, 
                                                   
                                                   const double TimeRestrictionRange)
{ 
    double pmax        = 0.0;
    
    int    pmax_index  = 0;
    
    bool   StrangePeak = true;
    
    bool   FirstPoint  = true; 
    
    if(TimeRestriction_OnOff)
    {
    	for( int j = 0; j < npoints; j++)
    	{
    		if (t[j] > -TimeRestrictionRange && t[j] < TimeRestrictionRange) //zoom in to find the Pmax
    		{
        		if ( FirstPoint && w[j] > 0.0 )
        		{
        			pmax       = w[j];
        			
        			pmax_index = j;
        			
        			FirstPoint = false;
        		}
        	
        		if (j != 0 && w[j] > pmax)
        		{
        			pmax        = w[j];
        			
        			pmax_index  = j;
        			
        			StrangePeak = false;
        		}
    		}
    	}
    	
    	if(StrangePeak && FirstPoint)
    	{
    		pmax       = 10000.0;
        			
        	pmax_index = npoints - 1;
        }
    }
    
    else
    {
    	for ( int j = 0; j < npoints; j++)
    	{
        	if (j == 0)
        	{
        		pmax = w[j];
        			
        		pmax_index = j;
        	}
        	
        	if (j != 0 && w[j] > pmax)
        	{
        		pmax = w[j];
        			
        		pmax_index = j;
    		}
    	}
    }
    
    return std::make_pair( pmax, pmax_index);
}

//===========================================================================================================================================

std::pair<double, int> betaAnalysis::pulse_max_with_index( const int npoints,
                                             
                                             std::vector<double> w, 
                                             
                                             std::vector<double> t, 
                                             
                                             const int    TimeRestriction_OnOff, 
                                             
                                             const double TimeRestrictionRange1, 
                                             
                                             const double TimeRestrictionRange2 )
{ 
    double pmax         = 0.0;
    
    int    pmax_index   = 0;
    
    bool   StrangePeak  = true;
    
    bool   FirstPoint   = true; 
    
    if(TimeRestriction_OnOff)
    {
    	for( int j = 0; j < npoints; j++)
    	{
    		if (t[j] > TimeRestrictionRange1 && t[j] < TimeRestrictionRange2) //zoom in to find the Pmax
    		{
        		if ( FirstPoint && w[j] > 0.0 )
        		{
        			pmax = w[j];
        			
        			pmax_index = j;
        			
        			FirstPoint = false;
        		}
        	
        		if (j != 0 && w[j] > pmax)
        		{
        			pmax = w[j];
        			
        			pmax_index = j;
        			
        			StrangePeak = false;
        		}
    		}
    	}
    	
    	if(StrangePeak && FirstPoint)
    	{
    		pmax = 10000.0;
        			
        	pmax_index = npoints-1;
        }
    }
    
    else
    {
    	for ( int j = 0; j < npoints; j++)
    	{
        	if (j == 0)
        	{
        		pmax = w[j];
        			
        		pmax_index = j;
        	}
        	
        	if (j != 0 && w[j] > pmax)
        	{
        		pmax = w[j];
        			
        		pmax_index = j;
    		}
    	}
    }
    
    return std::make_pair( pmax, pmax_index);
}

//===========================================================================================================================================


double betaAnalysis::time_max( const int npoints,
                               std::vector<double> w, 
                               std::vector<double> t,
                               const std::pair<double,int> pmax_holder, 
                               const int    TimeRestriction_OnOff, 
                               const double TimeRestrictionRange )
{
	//number of data points in event, 
	//voltage vector, 
	//time vector
    
    double tmax = 0.0;
    
    //std::pair<double, int> pmax_holder = betaAnalysis::pulse_max_with_index_fixed ( npoints, w, t, TimeRestriction_OnOff, TimeRestrictionRange );
    
    tmax = t[pmax_holder.second];
    
    return tmax;
}

//===========================================================================================================================================

double betaAnalysis::pulse_area( const int npoints,
                                 std::vector<double> w,
                                 std::vector<double> t,
                                 const std::pair<double,int> pmax_holder,
                                 const int    TimeRestriction_OnOff,
                                 const double TimeRestrictionRange )
{
	// number of data points in event, 
	// time vector, 
	// voltage vector

    double       pulse_area      = 0.0; 
    const double time_difference = t[1] - t[0];  
    
    //std::pair<double, int> pmax_holder = betaAnalysis::pulse_max_with_index_fixed ( npoints, w, t, TimeRestriction_OnOff, TimeRestrictionRange );
    
    const int imax = pmax_holder.second;
    int istart = 0;
    int iend;
    
    for ( int j = imax; j>-1; j-- ) // find index of start of pulse
    {     
    	if(w.at(j) <= 0)
    	{ 
    		istart = j;
    		
    		break;
    	} 
    } 
    
    for ( int j = imax; j< npoints; j++ ) // find index of end of pulse
    {
    	if(w.at(j) <= 0)
    	{ 
    		iend = j;
    		
    		break;
    	}
    	
    	if( j == npoints-1 )
    	{
    		iend = j;
    	} 
    } 
    
    for ( int j = istart; j < iend; j++ ) 
    {
    	pulse_area = pulse_area + w.at(j)/1000;
    }
    
    pulse_area = pulse_area * time_difference /1E12;
    
    return pulse_area; // collected pulse area, assuming voltage is in volts, time is in seconds
}

//===========================================================================================================================================

double betaAnalysis::rise_int ( const double x1,
                                const double y1,
                                const double x2,
                                const double y2,
                                const double y3 )
{
    double m, x3;
    
    m = (y2 - y1) / (x2 - x1);
    
    x3 = ( y3 - y1 ) / m + x1;
    
    return x3;
}

//===========================================================================================================================================

double betaAnalysis::cfd_index ( const int npoints,
                                 const double fraction,
                                 std::vector<double> w,
                                 std::vector<double> t,
                                 const std::pair<double,int> pmax_holder,
                                 const int    TimeRestriction_OnOff,
                                 const double TimeRestrictionRange)
{
    // function to calculate index of constant fraction - not truly a constant fraction discriminator
    
    double pmax = 0.0, time_fraction = 0.0;
    
    int imax = 0, ifraction = 0;
    
    //std::pair<double, int> pmax_holder = betaAnalysis::pulse_max_with_index_fixed ( npoints, w, t, TimeRestriction_OnOff, TimeRestrictionRange );
    
    imax = pmax_holder.second;
    
    pmax = pmax_holder.first; 
     
    for ( int j = imax; j>-1; j--)
    {
        if (w[j] <= pmax*double(fraction)/100.0)
        {
            ifraction     = j;              //find index of first point before constant fraction of pulse
            time_fraction = t[j];
            
            break;
        }    
    }
    
    time_fraction = time_fraction + (t[ifraction+1] - t[ifraction])* (pmax*double(fraction)/100.0 - w[ifraction]) /(w[ifraction+1] - w[ifraction]);
    
    return time_fraction;
}

//===========================================================================================================================================

double betaAnalysis::pulse_min( int npoints, std::vector<double> w){  // number of data points in event, voltage vector
    // function to calculate pulse minimum
    double pmin = 0;
    for (int j = 0; j < npoints; j++){
        if (j == 0){ pmin = 0;}
        if (w[j] < pmin){ pmin = w[j];}
    }
    
    return pmin;
}

//===========================================================================================================================================

double betaAnalysis::rise_time( const int           npoints,
                                std::vector<double> w,
                                std::vector<double> t,
                                const std::pair<double,int> pmax_holder,
                                double              bottom,
                                double              top,
                                const int           TimeRestriction_OnOff,
                                const double        TimeRestrictionRange)
{ 
	// number of data points in event, 
	// voltage vector, 
	// time vector
    // function to calculate 10-90 rise time
    // would be nice to add linear (or better) interpolation
   
    double rise = 0.0, pmax = 0.0, lowerval = 0.0, upperval = 0.0, m1 = 0.0, m2 = 0.0, tbottom = 0.0, ttop = 0.0;
    
    int imax = 0, itop = 0, ibottom = 0;
    
    bool ten = true, ninety = true;

    
    //std::pair<double, int> pmax_holder = betaAnalysis::pulse_max_with_index_fixed ( npoints, w, t, TimeRestriction_OnOff, TimeRestrictionRange );
    
    imax     = pmax_holder.second;
    
    pmax     = pmax_holder.first;
            	
    lowerval = pmax * bottom;
            	
    upperval = pmax * top; 

    
    for ( int j = imax; j > -1; j--)
    {
        if (ninety && w[j]<upperval)
        {
            itop    = j;     //find the index right below 90%
            ninety  = false;
        }
        
        if (ten && w[j]<lowerval)
        {
            ibottom = j;      //find the index right below 10%
            ten     = false;
        }
        
        if ( !ten && !ninety )
        {
            break;
        }
    }
    
    tbottom = rise_int( t[ibottom], w[ibottom], t[ibottom + 1], w[ibottom + 1], lowerval);
    
    ttop    = rise_int( t[itop], w[itop], t[itop + 1], w[itop + 1], upperval);
    
    rise    = ttop - tbottom; // rise
   
    
    bool WithinRange = false;
    
    if (TimeRestriction_OnOff) 
    {
    	if ( t.at(imax) < TimeRestrictionRange && t.at(imax) > -TimeRestrictionRange) WithinRange = true;
   	}
    
     if( true && rise < 0 &&  WithinRange ) 
     {
     	std::cout<<" tbottom " << tbottom << std::endl; 
     	std::cout<<" ttop " << ttop << std::endl;  
     	std::cout<<" TimeRestrictionRange " << TimeRestrictionRange << std::endl;
     	std::cout<<" pmax_holder.second " << pmax_holder.second << std::endl;
     	std::cout<<" Tmax " << t[imax] << std::endl;
     	std::cout<<" TimeRestriction_OnOff " << TimeRestriction_OnOff << std::endl;
     	std::cout<<" ibottom  " << ibottom << std::endl; 
     	std::cout<<" itop  " << itop << std::endl;
		std::cout<<" t[ibottom]  " << t[ibottom] << std::endl;
		std::cout<<" t[itop]  " << t[itop] << std::endl;
		std::cout<<" t[imax]  " << t[imax] << std::endl;
		std::cout<<" w[imax]  " << w[imax] << std::endl;
		std::cout<<" w[itop]  " << w[itop] << std::endl;
		std::cout<<" w[ibottom]  " << w[ibottom] << std::endl;
		std::cout<<" w[itop+1]  " << w[itop+1] << std::endl;
		std::cout<<std::endl;
		return -10;
	}
    
    return rise;
}

//===========================================================================================================================================

double betaAnalysis::noise_rms( const int           npoints,
                                std::vector<double> w,
                                const int           inoise )
{
	// finding the rms 
	
	// number of data points in event,
	// voltage vector
    
    double rms = 0.0, mean = 0.0, var = 0.0;
    
    for (int j = 0; j < inoise; j++)
    {
    	rms  += w[j]*w[j]; 
    	mean += w[j];
    }
    
    mean = mean / inoise;
    rms  = rms/inoise;
    var  = rms - mean * mean;
    rms  = pow(var, 0.5); // stupid convention where we call the standard deviation the rms
    
    return rms;
}

//===========================================================================================================================================

double betaAnalysis::pulse_dvdt_cfd( const int           npoints,
                                     const int           fraction,
                                     const int           ndif,
                                     std::vector<double> w,
                                     std::vector<double> t,
                                     const std::pair<double,int> pmax_holder,
                                     const int           TimeRestriction_OnOff,
                                     const double        TimeRestrictionRange )
{
    // function to dv/dt at a given constant fraction value.
    
    double pmax            = 0.0;  
    double time_difference = 0.0; 
    double dvdt            = 0.0;
    
    int imax            = 0;
    int ifraction       = 0;
    
    time_difference = t[1] - t[0];
    
    // find index of pulse maxima
    
    //std::pair<double, int> pmax_index = betaAnalysis::pulse_max_with_index_fixed ( npoints, w, t, TimeRestriction_OnOff, TimeRestrictionRange );
    
    
    pmax = pmax_holder.first;
    imax = pmax_holder.second;
    
    
    for ( int j = imax; j>-1; j--)
    {
    	if(w[j] <= pmax*double(fraction)/100)
    	{ 
    		ifraction = j; 
    		
    		break;
    	}  
    }//find index of first point before constant fraction of pulse
    
    
    if (ndif == 0)
    {         
    	dvdt = (w[ifraction+1] - w[ifraction])/time_difference;
    }
    
    else 
    {
    	dvdt = (w[ifraction+ndif] - w[ifraction-ndif])/(time_difference*(ndif*2));
    }
    
    return dvdt;
}

//===========================================================================================================================================

double betaAnalysis::cfd_index_falling( const int           npoints,
                                        const int           fraction,
                                        std::vector<double> w,
                                        std::vector<double> t,
                                        const std::pair<double,int> pmax_holder,
                                        const int           TimeRestriction_OnOff,
                                        const double        TimeRestrictionRange)
{
    // function to calculate index of constant fraction - not truly a constant fraction discriminator
        
    double pmax = 0.0, time_fraction = 0.0;
    
    //std::pair<double, int> pmax_holder = betaAnalysis::pulse_max_with_index_fixed ( npoints, w, t, TimeRestriction_OnOff, TimeRestrictionRange );
    
    pmax = pmax_holder.first;
    
    const int imax = pmax_holder.second;
    
    int ifraction = 0;
    
    for ( int j = imax; j<npoints; j++)
    {
        if (w[j] <= pmax*double(fraction)/100)
        {
            ifraction = j;              //find index of first point before constant fraction of pulse
            time_fraction = t[j];
            break;
        }
        
    }
    
    time_fraction = time_fraction + (t[ifraction-1] - t[ifraction])* (pmax*double(fraction)/100 - w[ifraction]) /(w[ifraction-1] - w[ifraction]);
    
    return time_fraction;
}

//-----------8 21 2017------------------------------------------------
//add dvdt linear fit



int image_counter = 0;

TF1 *linear_fit = new TF1("linear_fit", "[0]*x + [1]");

double betaAnalysis::dvdt_linear(int ndata_pt, int npoints, int fraction, std::vector<double> w, std::vector<double> t, int inoise, int TimeRestriction_OnOff, double TimeRestrictionRange)
{
	std::pair<double, int> pmax_holder = betaAnalysis::pulse_max_with_index_fixed ( npoints, w, t, TimeRestriction_OnOff, TimeRestrictionRange );
	
	double voltage_at_fraction = double(fraction)/100 * pmax_holder.first;

	std::vector<double> x; 
	std::vector<double> y;
	int temp_index;
	//int counter = 0;

	for (int j = pmax_holder.second; j > -1; j--)
	{
		if ( w.at(j) < voltage_at_fraction )
		{
			temp_index = j;
			
			if ( (temp_index - ndata_pt) <= 0 ) 
			{
				for ( int i = 0; i < 2*ndata_pt; i++ )
				{	
					x.push_back( t.at(0 + i) ); 
					y.push_back( w.at(0 + i) );
				}
				break;
			}
			
			if ( (temp_index + ndata_pt) > pmax_holder.second )
			{
				for ( int i = 0; i < 2*ndata_pt; i++ )
				{	
					x.push_back( t.at(pmax_holder.second - i) );
					y.push_back( w.at(pmax_holder.second - i) );
				}
				break;
			}

			else
			{
				for ( int i = 0; i < 2*ndata_pt; i++)
				{
					x.push_back( t.at(temp_index - ndata_pt + i) );
					y.push_back( w.at(temp_index - ndata_pt + i) );
				}
				break;
			}
			
		}
	}
	
		if(x.empty())
		{
			return 0;
		}

	TGraph *dvdt_g = new TGraph( x.size() , &x[0], &y[0]);

	//for(int k=0;k<x.size();k++){std::cout<<y.at(k)<<std::endl;}
	
	
	//TF1 *linear_fit = new TF1("", "[0]*x + [1]");//, x.at(0), x.at( x.size()-1 ) );
	

	//cout<< x.at(0)<<endl;cout<<x.at(x.size()-1)<<endl;	
	
	TThread::Lock();
		
	dvdt_g->Fit(linear_fit, "Q");
	
	double dvdt_o = linear_fit->GetParameter(0);
	
	TThread::UnLock();
	
	if(0)
	{
		gROOT->SetBatch(true);

		TCanvas *c = new TCanvas;
	
		dvdt_g->Draw("AC*");
		
		TImage *img = TImage::Create();
		
		img->FromPad(c);
		
		img->WriteImage( Form("%d.png", image_counter) );
		
		TThread::Lock();
		
		image_counter++;
		
		TThread::UnLock();
		
		delete img;
		delete c;
	}
			
	delete dvdt_g;
	//delete linear_fit;
	x.clear();
	y.clear();
	
	return dvdt_o;
}

//----------------------------8 29 2017---------------------------------- 
//pmax witth ploy fit

TF1 *ploy_2 = new TF1("ploy_2", "[0]*x*x + [1]*x + [2]");

double betaAnalysis::fit_pmax ( int npoints, std::vector<double> w, std::vector<double> t, int inoise, int range, int TimeRestriction_OnOff, double TimeRestrictionRange) 
{
	std::pair<double, int> pmax_holder = betaAnalysis::pulse_max_with_index_fixed ( npoints, w, t, TimeRestriction_OnOff, TimeRestrictionRange ); //locating the pmax and its index.
	
	std::vector<double> pmax_neighber_v; //getting the points around pmax. 
	
	std::vector<double> pmax_neighber_t;
	
	int wsize = w.size();
	
	if( (pmax_holder.second + range) > wsize || (pmax_holder.second - range) < 0 )
	{
		range = range - 1;
	}
	
	for (int step = 0; step < range; step++) //fill up the vector with points around pmax
	{
		pmax_neighber_v.push_back ( w.at(pmax_holder.second + step) );
		
		pmax_neighber_t.push_back ( t.at(pmax_holder.second + step) );
		
		pmax_neighber_v.push_back ( w.at(pmax_holder.second - step) );
		
		pmax_neighber_t.push_back ( t.at(pmax_holder.second - step) );		
	}
	
	TGraph *pmax_g = new TGraph ( pmax_neighber_v.size() , &(pmax_neighber_t[0]), &(pmax_neighber_v[0]) );
	
	TThread::Lock();
	
	pmax_g -> Fit(ploy_2, "Q");
	
	double a0 = ploy_2->GetParameter(0);
	
	double a1 = ploy_2->GetParameter(1); 
	
	double a2 = ploy_2->GetParameter(2);
	
	double pmax_t = -a1/(2*a0);
	
	double pmax = a0 * pmax_t * pmax_t + a1 * pmax_t + a2;
	
	TThread::UnLock();
	
	delete pmax_g;
	
	pmax_neighber_v.clear();
	
	pmax_neighber_t.clear();
	
	return pmax;
}


//---------------------------------------------------------------

double betaAnalysis::xlinearInter( const double x1,
                     const double y1,
                     const double x2,
                     const double y2,
                     const double y )
{
	double x = 0.0;
	 
	x = x1 + (y - y1)*(x2 - x1)/(y2 - y1);
	
	return x;
}

double betaAnalysis::getThresholdTime( const int           npoints,
                         const double        thresholdLevel,
                         std::vector<double> voltage,
                         std::vector<double> time,
                         const std::pair<double,int> pmax_holder,
                         const int           TimeRestriction_OnOff,
                         const double        TimeRestrictionRange)
{
	double pmax = 0.0, timeAtThreshold = 0.0, timeBelowThreshold = 0.0;

	int pmax_index = 0, timeBelowThreshold_index = 0;
	
	//std::pair<double,int> pmaxHolder = betaAnalysis::pulse_max_with_index_fixed ( npoints, voltage, time, TimeRestriction_OnOff, TimeRestrictionRange );
	
	pmax_index = pmax_holder.second;
    
    pmax       = pmax_holder.first;
    
    if( pmax < thresholdLevel ){ return 9999.0;}
    else
    {
    	for( int i = pmax_index; i > -1; i-- )
    	{
    		if( voltage.at(i) <= thresholdLevel )
    		{
    			timeBelowThreshold_index = i;
    			
    			timeBelowThreshold       = time.at(i);
    			
    			break;
    		}
    	}
    	
    	timeAtThreshold = xlinearInter( timeBelowThreshold, voltage.at(timeBelowThreshold_index), time.at(timeBelowThreshold_index+1), voltage.at(timeBelowThreshold_index+1), thresholdLevel );
    	
    	return timeAtThreshold;
    }
}


