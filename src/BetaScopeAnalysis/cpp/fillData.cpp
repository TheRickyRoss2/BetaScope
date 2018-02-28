#include "../hpp/betaAnalysis.h"

void betaAnalysis::fillData()
{
	//double tempHolder;
	
	for( int fill = 0; fill < this->numEvent; fill++)
	{
		this->iTree->GetEntry( fill );
		
		
		//======================================================================================
		for( unsigned int b = 0; b < this->channel.size(); b++ )
		{
			for(int i = 0; i < this->npoint; i++)
			{
				if( iBranchType == 'v' )
				{
					this->w[b].push_back( this->wf_v[b]->at(i) * this->voltageScalar );
					this->t[b].push_back( this->tf_v[b]->at(i) * this->timeScalar );
					this->wbase[b].push_back( this->wf_v[b]->at(i) * this->voltageScalar );
				}
				else
				{
				 	this->w[b].push_back( this->wf_a[b][i] * this->voltageScalar );
					this->t[b].push_back( this->tf_a[b][i] * this->timeScalar );
					this->wbase[b].push_back( this->wf_a[b][i] * this->voltageScalar );
				}
			}
		}
		
		//======================================================================================
		int OnOff;
		if(0) //correct the time walk first.
		{
			double correct ;
			
			std::vector<double> tempw3;
			
			for(unsigned int i = 0; i < w[2].size() ; i++){tempw3.push_back(w[2][i]);}
			betaAnalysis::base_line(tempw3.size(), tempw3, this->m_inoise);
			
			for(unsigned int i = 0; i < tempw3.size() ; i++){tempw3[i] = -1.0*tempw3[i];}
			
			std::pair<double,int> pmaxHolder_trig = betaAnalysis::pulse_max_with_index_fixed ( this->npoint, this->w[2], this->t[2], OnOff, this->TimeZoomRange );
		
			correct = betaAnalysis::cfd_index(tempw3.size(), 20, tempw3, t[2], pmaxHolder_trig, 0, this->TimeZoomRange);
		
			tempw3.clear();
		
			for(unsigned int c = 0; c < this->channel.size(); c++)
			{
		   		if( this->channel.at(c) )
		   		{
		   			for(int i = 0; i< this->npoint; i++)
		   			{
		   				this->t[c][i] = this->t[c][i] - correct;
		   			}
		   		}
			}
		}
		//======================================================================================
		
		for( unsigned int b = 0; b < this->channel.size(); b++ )
		{
			betaAnalysis::base_line(this->npoint, this->w[b], this->m_inoise);
			if( this->invertPulse.at(b) )
			{
				for( int i = 0; i < this->npoint; i++ )
				{
					this->w[b][i] = -1.0 * this->w[b][i];
				}	
			}

			if( this->channel.at(b) == this->trig ){ OnOff = 0; }
			else{ OnOff = this->TimeZoomRange_Switch; }
			
			std::pair<double,int> pmaxHolder = betaAnalysis::pulse_max_with_index_fixed ( this->npoint, this->w[b], this->t[b], OnOff, this->TimeZoomRange );
			
			this->pmax[b].push_back( pmaxHolder.first );
			
			//pmax_fit[ch_num].push_back( fit_pmax(points, w[ch_num], t[ch_num], m_inoise, 2, OnOff, TimeZoomRange ) );
						
			for(int k =0; k<101; k++){ double percentage = 1.0*k; this->cfd[b].push_back( betaAnalysis::cfd_index( this->npoint, percentage, this->w[b], this->t[b], pmaxHolder, OnOff, this->TimeZoomRange) ); }
			
			for(int k =0; k<101; k++){ this->cfd_fall[b].push_back( betaAnalysis::cfd_index_falling( this->npoint, k, this->w[b], this->t[b], pmaxHolder, OnOff, this->TimeZoomRange) ); }
		
			for( int step = 0; step < (this->fineCFDStep); step++ ){ double percentage = 0.2 * step; this->fineCFDRise[b].push_back( betaAnalysis::cfd_index( this->npoint, percentage, this->w[b], this->t[b], pmaxHolder, OnOff, this->TimeZoomRange) ); }
				
			//------------------Tmax----------------------------------------------------------------------
			
			this->tmax[b].push_back( betaAnalysis::time_max( this->npoint, this->w[b], this->t[b], pmaxHolder, OnOff, this->TimeZoomRange) );
				
			//--------------------------------------------------------------------------------------------

			this->rms[b].push_back( betaAnalysis::noise_rms( this->npoint, this->w[b], this->m_inoise ) );

			this->pulse_area_fill[b].push_back( betaAnalysis::pulse_area( this->npoint, this->w[b], this->t[b], pmaxHolder, OnOff, this->TimeZoomRange) ); 
				
			//------------------Rise Time 10 to 90---------------------------------------------------------

			this->rise_1090[b].push_back( betaAnalysis::rise_time( this->npoint, this->w[b], this->t[b], pmaxHolder, 0.1, 0.9, OnOff, this->TimeZoomRange) );
				
			//--------------------------------------------------------------------------------------------

			for(int d =0; d<101; d++){ this->dvdt[b].push_back( betaAnalysis::pulse_dvdt_cfd( this->npoint, d, 0, this->w[b], this->t[b], pmaxHolder, OnOff, this->TimeZoomRange) ); }
				
			for(int k = 0; k < 50; k++){ this->thTime[b].push_back( betaAnalysis::getThresholdTime( this->npoint, double(k), this->w[b], this->t[b], pmaxHolder, OnOff, this->TimeZoomRange) ); } 
				
			/*

			for(int d = 0; d < 101; d++) 
			{
					 
				dvdt_fit[ch_num].push_back( dvdt_linear(2, points, d, w[ch_num], t[ch_num], m_inoise, OnOff, TimeZoomRange ) ); 
					
			}
			*/
			
			//**********************************************************************************************
			//====Multiple Peaks====
			
			int StartPoint = 0;
			
			bool multiplePeakDetection = true;
				
			//int previousIndex = 0;
				
			for (int peak = 0; peak < this->lookForNumPeak; peak++)
			{
				std::pair <double, int> multiplePeaksHolder = multiplePeaks( this->npoint, this->w[b], this->t[b], StartPoint, OnOff, this->TimeZoomRange );
				
				this->multiplePmax[b].push_back( multiplePeaksHolder.first );
					
				int tsize = this->npoint;
					
				if ( multiplePeaksHolder.second  == tsize ) multiplePeaksHolder.second  = multiplePeaksHolder.second - 1;
					
				this->multipleTmax[b].push_back( TimeAtIndex( this->t[b], multiplePeaksHolder.second ) );

				StartPoint = multiplePeaksHolder.second;
			}
			
			if( multiplePeakDetection )
			{
				if( abs(this->multipleTmax[b].at(0) - this->multipleTmax[b].at(3)) > 150.0 )
				{
					this->singlePeak[b].push_back( 0 );
				}
				else
				{
					this->singlePeak[b].push_back( 1 );
				}
				
				multiplePeakDetection = false;
			}
			
			//double_pmax_holder.clear();
				
			//==============================================================================================
				

			for(int i = 0; i < this->npoint; i++)
			{
				this->w[b][i] = -1.0 * this->w[b][i];
			}
		}	
		//======================================================================================
		
		TThread::Lock();
		this->oTree->Fill();
		betaAnalysis::clearBuffer();
		TThread::UnLock();
		
		if ( fill % 300 == 0)
		{
			TThread::Lock();
			if( this->parallelStatus )
			{
				std::cout << this->ifileName <<" Finished = " << fill << std::endl;
			}
			else
			{
				std::cout << "Finished = " << fill << std::endl;
			}
			TThread::UnLock();
		}
	}
}
