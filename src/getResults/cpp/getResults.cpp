#include "../hpp/getResults.h"

///update: 8 7 2017----------------------------------------

TCut getData::setcuts(double Dut_tmin, double Dut_tmax, double Dut_pmin, double Dut_pmax, double Trig_tmin, double Trig_tmax, double Trig_pmin, double Trig_pmax, int DUT, int Trigger)
{
	const char* Dut_tcut = Form("tmax%d[0] - cfd%d[20]", DUT + 1, Trigger + 1 );
	const char* Trig_tcut = Form("tmax%d[0] - cfd%d[20]", Trigger + 1, Trigger + 1);
		
	const char* Dut_pcut = Form("pmax%d[0]", DUT + 1);
	const char* Trig_pcut = Form("pmax%d[0]", Trigger + 1);
	
	TCut cuts = Form(" %s > %f && %s < %f && "
				
					" %s > %f && %s < %f && "
		
					" %s > %f && %s < %f && "
						
					" %s > %f && %s < %f  ",
						  
					  Dut_tcut, Dut_tmin, Dut_tcut, Dut_tmax, 
						  
					  Dut_pcut, Dut_pmin, Dut_pcut, Dut_pmax,
						  
					  Trig_tcut, Trig_tmin, Trig_tcut, Trig_tmax, 
						  
					  Trig_pcut, Trig_pmin, Trig_pcut, Trig_pmax );
	
	return cuts;
}

std::pair<double,double> getData::Make_Histogram(const char* path, std::string HistName, std::string expression, std::string FitFunc, TCut cuts, int Channel, int &binNum, double &range_min, double &range_max, int draw, bool saveFile, bool Roo, bool confine)
{
      TFile *loadFile = TFile::Open(path);
      TTree* loadTree = (TTree*) loadFile->Get("wfm");
      entries = loadTree->GetEntries();
      int bins = entries/sqrt(2);
     
      TH1D *histo = new TH1D( Form("%s%d_h", HistName.c_str(), Channel) , Form("%s%d_h", HistName.c_str(), Channel+1) , binNum, range_min, range_max);
      
      loadTree->Project( Form("%s%d_h", HistName.c_str(), Channel), expression.c_str(), cuts);
      
      double sampleMean = histo->GetMean();
      double sampleSigma = histo->GetStdDev();
      
      TF1 *histo_fit = new TF1;
      
      std::string fitOption1;
      std::string fitOption2;
      
      if (FitFunc.compare("landau") == 0)
      {
      	histo_fit = new TF1( Form("%s%d_fit", HistName.c_str(), Channel+1), FitFunc.c_str());
      	
      	fitOption1 = "Q 0";
      	fitOption2 = "Q";
      	
      	histo_fit->SetParameter(1, sampleMean);
      	histo_fit->SetParameter(2, sampleSigma);
      }
      
      else if (FitFunc.compare("gaus") == 0)
      {
      	histo_fit = new TF1( Form("%s%d_fit", HistName.c_str(), Channel+1), FitFunc.c_str());
      	
      	
      	if( confine )
      	{
      		fitOption1 = "Q 0 B";
      		fitOption2 = "Q B";
      		histo_fit->SetParLimits(1, histo->GetXaxis()->GetBinCenter(histo->GetMaximumBin())-100, histo->GetXaxis()->GetBinCenter(histo->GetMaximumBin())+100);
      		histo_fit->SetParameter(1, histo->GetXaxis()->GetBinCenter(histo->GetMaximumBin()) );
      		histo_fit->SetParameter(2, histo->GetXaxis()->GetBinCenter(histo->GetMaximumBin())/2.0 );
      		histo_fit->SetParLimits(2, 0, histo->GetXaxis()->GetBinCenter(histo->GetMaximumBin()));	
      	}
      	else
      	{
      		fitOption1 = "Q 0";
      		fitOption2 = "Q";
      	}
      }
      
      else
      {
      	printf("Invalid fit function! \n");
      	return std::make_pair(-1,-1);
      }
      
      //if(confine)
      //{
      //histo ->Fit(histo_fit, "Q 0 R");
      //}
      
      //else
      //{
      int emptyFit = histo->Fit(histo_fit, fitOption1.c_str());
      //} 
      
      if(emptyFit)
      {
      	std::cout << "bad fit" << std::endl;
      	std::cout << "resetting ranges"  << std::endl;
      	std::cout << "histo entries = " << histo->GetEntries() << std::endl;
      	std::cout << "range_min = " << range_min << std::endl;
      	std::cout << "range_max = " << range_max << std::endl;
      	std::cout << "sampleMean = " << histo->GetMean() << std::endl;
      	std::cout << "sampleSigma = " << sampleSigma << std::endl;
      	range_min= -100;
      	range_max = 50000;
      	return std::make_pair( sampleMean, sampleSigma/histo->GetEntries() );
      }
       
      histo-> SetDirectory(0);
      
      if(auto_adjust)
	  {
	  	coin_events = histo->GetEntries();
	  	
	  	if (FitFunc.compare("landau") == 0)
     	{
      		range_min= histo_fit->GetParameter(1) - landau_left * histo_fit->GetParameter(2);
      		range_max = histo_fit->GetParameter(1) + landau_right * histo_fit->GetParameter(2);
      	}
      
      	if (FitFunc.compare("gaus") == 0)
      	{
      		double FD_width = 2.0 * 1.35 * histo->GetStdDev() * std::pow( coin_events, -1.0/3.0 );
      		//binNum = sqrt(coin_events);
      		range_min= histo_fit->GetParameter(1) - 5 * histo_fit->GetParameter(2);
      		range_max = histo_fit->GetParameter(1) + 5 * histo_fit->GetParameter(2);
      		binNum = (range_max - range_min)/FD_width;
      	}
	  	
      	
      }
      
      double Par = histo_fit->GetParameter(1);
      
      double ParErr = histo_fit->GetParError(1);
      
      TCanvas *oCanvas = new TCanvas;
      
      if(draw)
      {
      	oCanvas = new TCanvas( Form("%s%d_canvas", HistName.c_str(), Channel+1) );
      	
      	oCanvas->cd();
     
      	if (saveFile)
      	{
      		histo ->Fit(histo_fit, fitOption2.c_str());
      		gStyle->SetOptFit(1);
      		gSystem->ProcessEvents();
      		TImage *img = TImage::Create();
      		img->FromPad (oCanvas);
      		img->WriteImage( Form("%s_ch%d_%s.png", path, Channel, HistName.c_str()) );
      		delete img;
      		
      		delete histo_fit;
      		delete histo;
      		delete oCanvas;
      	}
      	
      	else
      	{
      		histo ->Fit(histo_fit, fitOption2.c_str());
      		gStyle->SetOptFit(1);
      	}
      }
      
      loadFile-> Close(); 
      delete loadFile;
      
      return std::make_pair( Par, ParErr );
}



void getData::pulses(const char* path, TCut cuts, int DUT, int Trigger, int draw)
{
	if(draw)
	{
		TFile *loadFile = TFile::Open(path);
		
		TTree* loadTree = (TTree*) loadFile->Get("wfm");
		
		TCanvas* wp2 = new TCanvas("wp2");
		
		wp2->cd();
		
		loadTree->Draw( Form("w%d:t%d", DUT+1, DUT+1), cuts);
		
		TCanvas* wp3 = new TCanvas("wp3");
		
		wp3->cd();
		
		loadTree->Draw( Form("w%d:t%d", Trigger+1, Trigger+1), cuts);
	}
}
      
void getData::magic2(const char* path, double Dut_tmin, double Dut_tmax, double Dut_pmin, double Dut_pmax, double Trig_tmin, double Trig_tmax, double Trig_pmin, double Trig_pmax, int draw, bool saveFile, bool logging, bool Roo) 
{
	gROOT->Reset();
	
	TCut temp_cuts = setcuts(Dut_tmin, Dut_tmax, Dut_pmin, Dut_pmax, Trig_tmin, Trig_tmax, Trig_pmin, Trig_pmax, DUT, Trigger);
	
	public_cuts = temp_cuts;

	current_file = path;
	
	getData::pulses(path, temp_cuts, DUT, Trigger, draw);
	
	std::pair<double, double> pmax_dut_o = getData::Make_Histogram (path, "Pmax", "pmax2[0]", "landau", temp_cuts, DUT, pmax_bin[DUT], pmax_range[DUT][0], pmax_range[DUT][1], draw, saveFile, Roo);
	
	std::pair<double, double> pmax_trigger_o = getData::Make_Histogram (path, "Pmax", "pmax3[0]", "landau", temp_cuts, Trigger, pmax_bin[Trigger], pmax_range[Trigger][0], pmax_range[Trigger][1], draw, saveFile, Roo);
	
	std::pair<double, double> charge_dut_o = getData::Make_Histogram (path, "PulseArea", "pulse_area2[0]/1E-15", "landau", temp_cuts, DUT, charge_bin[DUT], charge_range[DUT][0], charge_range[DUT][1], draw, saveFile, Roo);
	
	std::pair<double, double> charge_trigger_o = getData::Make_Histogram (path, "PulseArea", "pulse_area3[0]/1E-15", "landau", temp_cuts, Trigger, charge_bin[Trigger], charge_range[Trigger][0], charge_range[Trigger][1], draw, saveFile, Roo);
	
	std::pair<double, double> rms_dut_o = getData::Make_Histogram (path, "RMS", "rms2", "gaus", temp_cuts, DUT, rms_bin[DUT], rms_range[DUT][0], rms_range[DUT][1], draw, saveFile, Roo);
	
	std::pair<double, double> rms_trigger_o = getData::Make_Histogram (path, "RMS", "rms3", "gaus", temp_cuts, Trigger, rms_bin[Trigger], rms_range[Trigger][0], rms_range[Trigger][1], draw, saveFile, Roo);
	
	std::pair<double, double> dvdt_dut_o = getData::Make_Histogram (path, "dvdt", "dvdt2[20]", "landau", temp_cuts, DUT, charge_bin[DUT], dvdt_range[DUT][0], dvdt_range[DUT][1], draw, saveFile, Roo);
	
	std::pair<double, double> dvdt_trigger_o = getData::Make_Histogram (path, "dvdt", "dvdt3[20]", "landau", temp_cuts, Trigger, charge_bin[Trigger], dvdt_range[Trigger][0], dvdt_range[Trigger][1], draw, saveFile, Roo);
		
	std::pair<double, double> rise1090_dut_o = getData::Make_Histogram (path, "Rise Time", "rise2_1090", "gaus", temp_cuts, DUT, rise_1090_bin[DUT], rise1090_range[DUT][0], rise1090_range[DUT][1], draw, saveFile, Roo, true);
	
	std::pair<double, double> rise1090_trigger_o = getData::Make_Histogram (path, "Rise Time", "rise3_1090", "gaus", temp_cuts, Trigger, rise_1090_bin[Trigger], rise1090_range[Trigger][0], rise1090_range[Trigger][1], draw, saveFile, Roo);
	
	std::pair<double,double> FWHM_dut_o = getData::Make_Histogram (path, "FWHM", "cfd2_fall[50] - cfd2[50]", "gaus", temp_cuts, DUT, FWHM_bin[DUT], FWHM_range[DUT][0], FWHM_range[DUT][1], draw, saveFile, Roo);
	
	std::pair<double,double> FWHM_trigger_o = getData::Make_Histogram (path, "FWHM", "cfd3_fall[50] - cfd3[50]", "gaus", temp_cuts, Trigger, FWHM_bin[Trigger], FWHM_range[Trigger][0], FWHM_range[Trigger][1], draw, saveFile, Roo);
	
	if(logging)
	{
		std::cout << "-------------" << Form("ch%d", DUT+1) <<"------------------"<< std::endl;
		
		std::cout << "charge2" << ",,,,,,," << "pmax2" << "," << "rms2" << ",,,,," << "rise2_1090" << "," << "dvdt2" << ",,,,,,,,," << " FWHM2" << std::endl;
		std::cout << charge_dut_o.first <<", "<< charge_dut_o.second << ",,,,,,,"
				  << pmax_dut_o.first <<", "<< pmax_dut_o.second << ","  
				  << rms_dut_o.first <<", "<< rms_dut_o.second << ",,,,,"
	 			  << rise1090_dut_o.first <<", "<< rise1090_dut_o.second << ","
				  << dvdt_dut_o.first <<", "<< dvdt_dut_o.second << ",,,,,,,,,"
				  << FWHM_dut_o.first <<", "<<  FWHM_dut_o.second << std::endl;

		std::cout << "-------------" << Form("ch%d", Trigger+1) <<"------------------"<< std::endl;
		
		std::cout << "charge3" << ",,,,,,," << "pmax3" << "," << "rms3" << ",,,,," << "rise3_1090" << "," << "dvdt3" << ",,,,,,,,," << " FWHM3" << std::endl;
		std::cout << charge_trigger_o.first <<", "<< charge_trigger_o.second << ",,,,,,,"
				  << pmax_trigger_o.first <<", "<< pmax_trigger_o.second << ","  
				  << rms_trigger_o.first <<", "<< rms_trigger_o.second << ",,,,,"
	 			  << rise1090_trigger_o.first <<", "<< rise1090_trigger_o.second << ","
				  << dvdt_trigger_o.first <<", "<< dvdt_trigger_o.second << ",,,,,,,,,"
				  << FWHM_trigger_o.first <<", "<<  FWHM_trigger_o.second << std::endl;
		
		std::cout << std::endl;
		std::cout << std::endl;	
	}
	
	else
	{}
}     


void getData::magic3(const char* path, double Dut_tmin, double Dut_tmax, double Dut_pmin, double Dut_pmax, double Trig_tmin, double Trig_tmax, double Trig_pmin, double Trig_pmax, int draw, bool saveFile, bool logging, bool Roo) 
{
	//gROOT->Reset();
	
	std::cout<< "Start processing..." << std::endl;
	
	for(int j = 0; j < 5; j++)
	{
		getData::magic_loop(path, Dut_tmin, Dut_tmax, Dut_pmin, Dut_pmax, Trig_tmin, Trig_tmax, Trig_pmin, Trig_pmax, 0, false, logging);
	}
	
	TCut temp_cuts = setcuts(Dut_tmin, Dut_tmax, Dut_pmin, Dut_pmax, Trig_tmin, Trig_tmax, Trig_pmin, Trig_pmax, DUT, Trigger);
	
	public_cuts = temp_cuts;

	//current_file = path;
	
	gROOT->SetBatch(true);
	
	std::cout<< "Saving plots..." << std::endl;
	
	//getData::pulses(path, temp_cuts, DUT, Trigger, draw);
	
	std::pair<double, double> pmax_dut_o = getData::Make_Histogram (path, "Pmax", "pmax2[0]", "landau", temp_cuts, DUT, pmax_bin[DUT], pmax_range[DUT][0], pmax_range[DUT][1], draw, saveFile, Roo);
	
	std::pair<double, double> pmax_trigger_o = getData::Make_Histogram (path, "Pmax", "pmax3[0]", "landau", temp_cuts, Trigger, pmax_bin[Trigger], pmax_range[Trigger][0], pmax_range[Trigger][1], draw, saveFile, Roo);
	
	std::pair<double, double> charge_dut_o = getData::Make_Histogram (path, "PulseArea", "pulse_area2/1E-15", "landau", temp_cuts, DUT, charge_bin[DUT], charge_range[DUT][0], charge_range[DUT][1], draw, saveFile, Roo);
	
	std::pair<double, double> charge_trigger_o = getData::Make_Histogram (path, "PulseArea", "pulse_area3/1E-15", "landau", temp_cuts, Trigger, charge_bin[Trigger], charge_range[Trigger][0], charge_range[Trigger][1], draw, saveFile, Roo);
	
	std::pair<double, double> rms_dut_o = getData::Make_Histogram (path, "RMS", "rms2", "gaus", temp_cuts, DUT, rms_bin[DUT], rms_range[DUT][0], rms_range[DUT][1], draw, saveFile, Roo);
	
	std::pair<double, double> rms_trigger_o = getData::Make_Histogram (path, "RMS", "rms3", "gaus", temp_cuts, Trigger, rms_bin[Trigger], rms_range[Trigger][0], rms_range[Trigger][1], draw, saveFile, Roo);
	
	std::pair<double, double> dvdt_dut_o = getData::Make_Histogram (path, "dvdt", "dvdt2[20]", "landau", temp_cuts, DUT, charge_bin[DUT], dvdt_range[DUT][0], dvdt_range[DUT][1], draw, saveFile, Roo);
	
	std::pair<double, double> dvdt_trigger_o = getData::Make_Histogram (path, "dvdt", "dvdt3[20]", "landau", temp_cuts, Trigger, charge_bin[Trigger], dvdt_range[Trigger][0], dvdt_range[Trigger][1], draw, saveFile, Roo);
		
	std::pair<double, double> rise1090_dut_o = getData::Make_Histogram (path, "Rise Time", "rise2_1090", "gaus", temp_cuts, DUT, rise_1090_bin[DUT], rise1090_range[DUT][0], rise1090_range[DUT][1], draw, saveFile, Roo, true);
	
	std::pair<double, double> rise1090_trigger_o = getData::Make_Histogram (path, "Rise Time", "rise3_1090", "gaus", temp_cuts, Trigger, rise_1090_bin[Trigger], rise1090_range[Trigger][0], rise1090_range[Trigger][1], draw, saveFile, Roo);
	
	std::pair<double,double> FWHM_dut_o = getData::Make_Histogram (path, "FWHM", "cfd2_fall[50] - cfd2[50]", "gaus", temp_cuts, DUT, FWHM_bin[DUT], FWHM_range[DUT][0], FWHM_range[DUT][1], draw, saveFile, Roo);
	
	std::pair<double,double> FWHM_trigger_o = getData::Make_Histogram (path, "FWHM", "cfd3_fall[50] - cfd3[50]", "gaus", temp_cuts, Trigger, FWHM_bin[Trigger], FWHM_range[Trigger][0], FWHM_range[Trigger][1], draw, saveFile, Roo);
	
	std::ofstream ofile;
	
	std::string prefix = path;
	
	ofile.open ( "_results.txt", std::ios::app);
	
	ofile << "-------------" << Form("%s_ch%d", prefix.c_str(), DUT+1) <<"------------------"<< std::endl;
	
	ofile << Dut_tmin << ", " << Dut_tmax << ", " << Dut_pmin << ", " << Dut_pmax << ", " << Trig_tmin << ", " << Trig_tmax << ", " << Trig_pmin << ", " << Trig_pmax << std::endl;
	
	ofile << "charge2" << ",,,,,,," << "pmax2" << "," << "rms2" << ",,,,," << "rise2_1090" << "," << "dvdt2" << ",,,,,,,,," << " FWHM2" << std::endl;
	ofile << charge_dut_o.first <<", "<< charge_dut_o.second << ",,,,,,,"
			  << pmax_dut_o.first <<", "<< pmax_dut_o.second << ","  
			  << rms_dut_o.first <<", "<< rms_dut_o.second << ",,,,,"
 			  << rise1090_dut_o.first <<", "<< rise1090_dut_o.second << ","
			  << dvdt_dut_o.first <<", "<< dvdt_dut_o.second << ",,,,,,,,,"
			  << FWHM_dut_o.first <<", "<<  FWHM_dut_o.second << std::endl;

	ofile << "-------------" << Form("%s_ch%d", prefix.c_str(), Trigger+1) <<"------------------"<< std::endl;
	
	ofile << "charge3" << ",,,,,,," << "pmax3" << "," << "rms3" << ",,,,," << "rise3_1090" << "," << "dvdt3" << ",,,,,,,,," << " FWHM3" << std::endl;
	ofile << charge_trigger_o.first <<", "<< charge_trigger_o.second << ",,,,,,,"
			  << pmax_trigger_o.first <<", "<< pmax_trigger_o.second << ","  
			  << rms_trigger_o.first <<", "<< rms_trigger_o.second << ",,,,,"
 			  << rise1090_trigger_o.first <<", "<< rise1090_trigger_o.second << ","
			  << dvdt_trigger_o.first <<", "<< dvdt_trigger_o.second << ",,,,,,,,,"
			  << FWHM_trigger_o.first <<", "<<  FWHM_trigger_o.second << std::endl;
	
	ofile << std::endl;
	ofile << std::endl;	
	
	std::cout << prefix << " is finished" << std::endl;
	std::cout << std::endl;
	//gROOT->SetBatch(false);
}

//===============================================================================================

	
//-----------------------------------------------------------------------------------------------

//8 10 2017--------------------------------------------------------------------------------------

void getData::see_status()
{
	printf (" \n");
	printf ("current file : %s \n", current_file.c_str());
	printf ("DUT: %d \n", DUT);
	printf ("Trigger: %d \n", Trigger);
	printf ("landau left factor : %d \n", landau_left);
	printf ("landau right factor : %d \n", landau_right);
	printf ("Current cuts : %s \n", public_cuts.c_str());
	printf (" \n");
}



//-----------------------------------------------------------------------------------------------


//------------8 11 2017------------------------------------------------------------------------------

void getData::landau_hist_factor(int left, int right)
{
	landau_left = left;
	landau_right = right;
}

//-----------------------------------------------------------------------------------------------



//------------9 6 2017------------------------------------------------------------------------------


void getData::magic_loop(const char* path, double Dut_tmin, double Dut_tmax, double Dut_pmin, double Dut_pmax, double Trig_tmin, double Trig_tmax, double Trig_pmin, double Trig_pmax, int draw, bool saveFile, bool logging, bool Roo) 
{
	gROOT->SetBatch(true);

	for( int i = 0; i < 5; i++)
	{
		getData::magic2 (path, Dut_tmin, Dut_tmax, Dut_pmin, Dut_pmax, Trig_tmin, Trig_tmax, Trig_pmin, Trig_pmax, draw, saveFile, logging, Roo);
	}

	gROOT->SetBatch(false); 
}

//===============================================================================================
   
//-----------------------------------------------------------------------------------------------

//updated:2017 7 10 -----------------------------------------------------------------------------

void getData:: adjust_hist( bool on_off)
{
	auto_adjust = on_off;
}

void getData::set_ch(int dut, int trigger)
{
	DUT = dut - 1;
	Trigger = trigger - 1;
}

void getData::rebin(int bins)
{
   	for(int ch = 0; ch < 4; ch++)
	{
		pmax_bin[ch] = bins;
		rise_1090_bin[ch] = bins;
		charge_bin[ch] = bins;
		dvdt_bin[ch] = bins;
		rms_bin[ch] = bins;
	}
}


void getData::rebin( std::string name, int ch, int bins )
{
	if(name.compare("pmax")==0) pmax_bin[ch] = bins;
	if(name.compare("rise1090")==0) rise_1090_bin[ch] = bins;
	if(name.compare("pmax1090")==0) rise_1090_bin[ch] = bins;
	if(name.compare("charge")==0) charge_bin[ch] = bins;
	if(name.compare("dvdt")==0) dvdt_bin[ch] = bins;
	if(name.compare("rms")==0) rms_bin[ch] = bins;
}


void getData::range(std::string name, int ch, double min, double max)
{
   if(name.compare("pmax")==0) { pmax_range[ch][0] = min; pmax_range[ch][1] = max;}
   if(name.compare("rise1090")==0) { rise1090_range[ch][0] = min; rise1090_range[ch][1] = max;}
   if(name.compare("charge")==0) { charge_range[ch][0] = min; charge_range[ch][1] = max;}
   if(name.compare("dvdt")==0) { dvdt_range[ch][0] = min; dvdt_range[ch][1] = max;}
   if(name.compare("rms")==0) { rms_range[ch][0] = min; rms_range[ch][1] = max;}
}

void getData::max(const char* path, int dut, int trig, TCut cuts, bool saveFile, double Ymin, double Ymax, double Xmax)
{
      gROOT->Reset();
      TFile *loadFile = TFile::Open(path);
      TTree* loadTree = (TTree*) loadFile->Get("wfm");
 
      loadTree->SetMarkerStyle(7);    
 
      TCanvas *c_dut = new TCanvas("c_dut");
      c_dut->cd();
      loadTree->Draw( Form("tmax%d-cfd%d[20]:pmax%d >> hist_dut",dut,trig,dut), cuts);
      
      TCanvas *c_trig = new TCanvas("c_trig");
      c_trig->cd();
      loadTree->Draw( Form("tmax%d-cfd%d[20]:pmax%d >> hist_trig",trig,trig,trig), cuts);
      
      if (saveFile)
      {     
      		TH2D *hist_dut = new TH2D;
      		TH2D *hist_trig = new TH2D;	
       		
      		if ( (Ymin  != (double) -1.0) && (Ymax  != (double) -1.0) )
      		{			
      			hist_dut = (TH2D *) c_dut->GetPrimitive("hist_dut");
      			
      			hist_trig = (TH2D *) c_trig->GetPrimitive("hist_trig");
      			
      			if (!hist_dut || !hist_trig) 
      			{
        			fprintf(stdout,"no histogram  tmax pmax\n");
        			delete c_dut;
      				delete c_trig;
        			return void();
    			}

				if((Xmax != (double) -1.0))
      			{ 
      				hist_dut->SetAxisRange( 0, Xmax, "X");
      				hist_trig->SetAxisRange( 0, Xmax, "X");
      			}

      			hist_dut->SetAxisRange( Ymin, Ymax, "Y");
      			//c_dut->cd();
      			//hist_dut->Draw();
      			c_dut->Modified();
      			c_dut->Update();
      			
      			hist_trig->SetAxisRange( Ymin, Ymax, "Y");
      			//c_trig->cd();
      			//hist_trig->Draw();
      			c_trig->Modified();
      			c_trig->Update();
      		}
      		
      		gSystem->ProcessEvents();
      		
      		TImage *img1 = TImage::Create();
      		TImage *img2 = TImage::Create();
      		
      		img1->FromPad (c_dut);
      		img1->WriteImage(Form("%s_ch%d_pmax.png", path, dut));
      		
      		img2->FromPad (c_trig);
      		img2->WriteImage(Form("%s_ch%d_pmax.png", path, trig));

      		delete img1;
      		delete img2;
      		    		
      		delete hist_dut;
      		delete hist_trig;
      		      		
      		delete c_dut;
      		delete c_trig;
      }
      
      gROOT->Reset();
}


//old method-------------------------------------------------------------------
/*
void getData::magic(const char* path, double tmax2_min, double tmax2_max, double pmax2_min, double pmax2_max, double tmax3_min, double tmax3_max, double pmax3_min, double pmax3_max)
   { 
      gROOT->Reset();
      TFile *loadFile = TFile::Open(path);
      TTree* loadTree = (TTree*) loadFile->Get("wfm");
      entries = loadTree->GetEntries();
      int bins = entries/sqrt(2);
      
      loadTree->ResetBranchAddresses();
      std::vector<double>* pmax[4] = {};
      std::vector<double>* tmax[4] = {};
      std::vector<double>* rise_1040[4] = {};
      std::vector<double>* rise_1090[4] = {};
      std::vector<double>* dvdt[4] = {};
      std::vector<double>* cfd[4] = {};
      std::vector<double>* rms[4] = {};
      std::vector<double>* pulse_area[4] = {};
	
	  int dut_trigger [2] = {DUT, Trigger};
	  int holder;
	  
	  for(int i=0; i<2; i++) //setting branch addresses
	  {
	  	holder = dut_trigger[i];	
	  	
      	loadTree->SetBranchAddress( Form("pmax%d", holder+1), &pmax[holder]);
      	loadTree->SetBranchAddress( Form("tmax%d", holder+1), &tmax[holder]);
      	loadTree->SetBranchAddress( Form("rise%d_1040", holder+1), &rise_1040[holder]);
      	loadTree->SetBranchAddress( Form("rise%d_1090", holder+1), &rise_1090[holder]);
      	loadTree->SetBranchAddress( Form("dvdt%d", holder+1), &dvdt[holder]);
      	loadTree->SetBranchAddress( Form("cfd%d", holder+1), &cfd[holder]);
      	loadTree->SetBranchAddress( Form("rms%d", holder+1), &rms[holder]);
      	loadTree->SetBranchAddress( Form("pulse_area%d", holder+1), &pulse_area[holder]);
	  }
	  
	  for(int i = 0; i<2; i++) //create histograms
	  {
	  	
	  	holder = dut_trigger[i];
	  	
      	pmax_h[holder] = new TH1D( Form("pmax%d_h",holder) , Form("pmax%d_hh", holder+1) , pmax_bin[holder], pmax_range[holder][0], pmax_range[holder][1]);
      
      	rise_1040_h[holder] = new TH1D( Form("rise%d_1040_h", holder) , Form("rise%d_1040_hh", holder+1) ,rise_1040_bin[holder], rise1040_range[holder][0], rise1040_range[holder][1]);
      
      	rise_1090_h[holder] = new TH1D( Form("rise%d_1090_h", holder) , Form("rise%d_1090_hh", holder+1) ,rise_1090_bin[holder], rise1090_range[holder][0], rise1090_range[holder][1]);
      
      	dvdt_h[holder] = new TH1D( Form("dvdt%d_h", holder), Form("dvdt%d_hh", holder+1) , dvdt_bin[holder], dvdt_range[holder][0], dvdt_range[holder][1]);
      
      	rms_h[holder] = new TH1D( Form("rms%d_h", holder), Form("rms%d_hh", holder+1), rms_bin[holder], rms_range[holder][0], rms_range[holder][1]);
      
     	charge_h[holder] = new TH1D( Form("charge%d_h", holder), Form("charge%d_hh", holder+1), charge_bin[holder], charge_range[holder][0], charge_range[holder][1]);
      
      }


      for(int i = 0; i < entries; i++) //filling histograms.
      {	
        loadTree->GetEntry(i);

	    if( (tmax[DUT]->at(0)- cfd[Trigger]->at(20) > tmax2_min  && tmax[DUT]->at(0)- cfd[Trigger]->at(20) < tmax2_max) 
		&& (pmax[DUT]->at(0) > pmax2_min && pmax[DUT]->at(0) < pmax2_max) 
		&& (tmax[Trigger]->at(0)-cfd[Trigger]->at(20) > tmax3_min  && tmax[Trigger]->at(0)- cfd[Trigger]->at(20) < tmax3_max) 
		&& (pmax[Trigger]->at(0) > pmax3_min && pmax[Trigger]->at(0) < pmax3_max)  )      
        {
        	for(int j=0; j<2; j++)
        	{
        		holder = dut_trigger[j];
        		
        		pmax_h[holder]->Fill(pmax[holder]->at(0));
		 		rise_1040_h[holder]->Fill(rise_1040[holder]->at(0));
		 		rise_1090_h[holder]->Fill(rise_1090[holder]->at(0));
		 		rms_h[holder]->Fill(rms[holder]->at(0));
		 		dvdt_h[holder]->Fill(dvdt[holder]->at(20));
		 		charge_h[holder]->Fill(pulse_area[holder]->at(0)/1E-15);
        	}
      	}
	}

	  TF1* pmax_fit[4] = {};
	  TF1* rise_1040_fit[4] = {};
	  TF1* rise_1090_fit[4] = {};
	  TF1* dvdt_fit[4] = {};
	  TF1* rms_fit[4] = {};
	  TF1* charge_fit[4] = {};
	  
	  for(int u=0; u<2; u++)
      {
      	holder = dut_trigger[u];
      	
      	pmax_fit[holder] = new TF1( Form("pmax%d_fit", holder+1), "landau");
      	rise_1040_fit[holder] = new TF1( Form("rise%d_1040_fit", holder+1), "gaus");
      	rise_1090_fit[holder] = new TF1( Form("rise%d_1090_fit", holder+1), "gaus");
      	dvdt_fit[holder] = new TF1( Form("dvdt%d_fit", holder+1), "landau");
      	rms_fit[holder] = new TF1( Form("rms%d_fit", holder+1), "gaus");
      	charge_fit[holder] = new TF1( Form("charge%d_fit", holder+1), "landau");
      }
	  
      //TF1* pmax2_fit = new TF1("pmax2_fit","landau");
      //TF1* rise2_1040_fit = new TF1("rise2_1040_fit","gaus");
      //TF1* rise2_1090_fit = new TF1("rise2_1090_fit","gaus");
      //TF1* dvdt2_fit = new TF1("dvdt2_fit","landau");
      //TF1* rms2_fit = new TF1("rms2_fit","gaus");
      //TF1* charge2_fit = new TF1("charge2_fit","landau");

      //TF1* pmax3_fit = new TF1("pmax3_fit","landau");
      //TF1* rise3_1040_fit = new TF1("rise3_1040_fit","gaus");
      //TF1* rise3_1090_fit = new TF1("rise3_1090_fit","gaus");
      //TF1* dvdt3_fit = new TF1("dvdt3_fit","landau");
      //TF1* rms3_fit = new TF1("rms3_fit","gaus");
      //TF1* charge3_fit = new TF1("charge3_fit","landau");

      TCanvas *p2 = new TCanvas("p2");
      p2->cd();
      ostringstream tpm2;
      tpm2 << "tmax2[0]-cfd3[20]>" << std::to_string(tmax2_min) << " && tmax2[0]-cfd3[20]<" << std::to_string(tmax2_max) << " && pmax2[0] >" << std::to_string(pmax2_min) + " && pmax2[0]<" << std::to_string(pmax2_max) << " && tmax3[0]-cfd3[20]>" << std::to_string(tmax3_min) << " && tmax3[0]-cfd3[20]<" << std::to_string(tmax3_max) << " && pmax3[0] >" << std::to_string(pmax3_min) << " && pmax3[0]<" + std::to_string(pmax3_max);
      TString tm = tpm2.str();
      loadTree->Draw("w2:t2", tm.Data());
      TCanvas *p3 = new TCanvas("p3");
      p3->cd();   
      loadTree->Draw("w3:t3", tm.Data());

	  TCanvas *c[256] = {};
	  int needed_canvas = 6 * 2;
	  for(int canvas_num = 0; canvas_num < needed_canvas; canvas_num++)
	  {
	  	c[canvas_num] = new TCanvas( Form("c%d", canvas_num) );
	  } 

      c[0]->cd();
      pmax_h[DUT] ->Fit(pmax_fit[DUT], "Q");
      
      
      c[1]->cd();
      rise_1040_h[DUT] ->Fit(rise_1040_fit[DUT], "Q");
      
      
      c[2]->cd();
      rise_1090_h[DUT] ->Fit(rise_1090_fit[DUT],"Q");
      
      
      c[3]->cd();
      dvdt_h[DUT] ->Fit(dvdt_fit[DUT], "Q");

 
      c[4]->cd();
      rms_h[DUT] ->Fit(rms_fit[DUT], "Q");
      

      c[5]->cd();
      charge_h[DUT] ->Fit(charge_fit[DUT], "Q");
      

      c[6]->cd();
      pmax_h[Trigger] ->Fit(pmax_fit[Trigger], "Q");


      c[7]->cd();
      rise_1040_h[Trigger] ->Fit(rise_1040_fit[Trigger], "Q");
      
      

      c[8]->cd();
      rise_1090_h[Trigger] ->Fit(rise_1090_fit[Trigger], "Q");


      c[9]->cd();
      dvdt_h[Trigger] ->Fit(dvdt_fit[Trigger], "Q");


      c[10]->cd();
      rms_h[Trigger] ->Fit(rms_fit[Trigger], "Q");
      

      c[11]->cd();
      charge_h[Trigger] ->Fit(charge_fit[Trigger], "Q");
      
	  
	  if(auto_adjust)
	  {
	  	coin_events = pmax_h[DUT]->GetEntries();
	  	
		//pmax_bin[DUT] = sqrt(coin_events);
      	pmax_range[DUT][0] = pmax_fit[DUT]->GetParameter(1) - 2*pmax_h[DUT]->GetStdDev(1);
      	pmax_range[DUT][1] = pmax_fit[DUT]->GetParameter(1) + 7*pmax_h[DUT]->GetStdDev(1);
      	//pmax_bin[DUT] = ( pmax_range[DUT][1]-pmax_range[DUT][0] )/ (pmax_h[DUT]->GetStdDev(1)/2);
      	
		//rise_1040_bin[DUT] = sqrt(coin_events);
		rise1040_range[DUT][0] = rise_1040_h[DUT]->GetMean(1) - 5*rise_1040_h[DUT]->GetStdDev(1);
		rise1040_range[DUT][1] = rise_1040_h[DUT]->GetMean(1) + 5*rise_1040_h[DUT]->GetStdDev(1);
		//rise_1040_bin[DUT] = ( rise1040_range[DUT][1]-rise1040_range[DUT][0] )/ (rise_1040_h[DUT]->GetStdDev(1)/2);
		
		//rise_1090_bin[DUT] = sqrt(coin_events);
      	rise1090_range[DUT][0] = rise_1090_h[DUT]->GetMean(1) - 5*rise_1090_h[DUT]->GetStdDev(1);
      	rise1090_range[DUT][1] = rise_1090_h[DUT]->GetMean(1) + 5*rise_1090_h[DUT]->GetStdDev(1);
      	//rise_1090_bin[DUT] = ( rise1090_range[DUT][1]-rise1090_range[DUT][0] )/ (rise_1090_h[DUT]->GetStdDev(1)/2);
      	
      	//dvdt_bin[DUT] = sqrt(coin_events);
      	dvdt_range[DUT][0] = dvdt_fit[DUT]->GetParameter(1) - 2*dvdt_h[DUT]->GetStdDev(1);
      	dvdt_range[DUT][1] = dvdt_fit[DUT]->GetParameter(1) + 7*dvdt_h[DUT]->GetStdDev(1);
      	//dvdt_bin[DUT] = ( dvdt_range[DUT][1]-dvdt_range[DUT][0] )/ (dvdt_h[DUT]->GetStdDev(1)/2);
      	
      	rms_bin[DUT] = sqrt(coin_events);
      	rms_range[DUT][0] = rms_h[DUT]->GetMean(1) - 5*rms_h[DUT]->GetStdDev(1);
      	rms_range[DUT][1] = rms_h[DUT]->GetMean(1) + 5*rms_h[DUT]->GetStdDev(1);
      	//rms_bin[DUT] = ( rms_range[DUT][1]-rms_range[DUT][0] )/ (rms_h[DUT]->GetStdDev(1)/2);
      	
      	//charge_bin[DUT] = sqrt(coin_events);
      	charge_range[DUT][0] = charge_fit[DUT]->GetParameter(1) - 2*charge_h[DUT]->GetStdDev(1);
      	charge_range[DUT][1] = charge_fit[DUT]->GetParameter(1) + 7*charge_h[DUT]->GetStdDev(1);
      	//charge_bin[DUT] = ( charge_range[DUT][1]-charge_range[DUT][0] )/ (charge_h[DUT]->GetStdDev(1)/2);
      	
      	//pmax_bin[Trigger] = sqrt(coin_events);
      	pmax_range[Trigger][0] = pmax_fit[Trigger]->GetParameter(1) - 2*pmax_h[Trigger]->GetStdDev(1);
      	pmax_range[Trigger][1] = pmax_fit[Trigger]->GetParameter(1) + 7*pmax_h[Trigger]->GetStdDev(1);
      	//pmax_bin[Trigger] = ( pmax_range[Trigger][1]-pmax_range[Trigger][0] )/ (pmax_h[Trigger]->GetStdDev(1)/2);
      	
      	//rise_1040_bin[Trigger] = sqrt(coin_events);
      	rise1040_range[Trigger][0] = rise_1040_h[Trigger]->GetMean(1) - 5*rise_1040_h[Trigger]->GetStdDev(1);
      	rise1040_range[Trigger][1] = rise_1040_h[Trigger]->GetMean(1) + 5*rise_1040_h[Trigger]->GetStdDev(1);
      	//rise_1040_bin[Trigger] = ( rise1040_range[Trigger][1]-rise1040_range[Trigger][0] )/ (rise_1040_h[Trigger]->GetStdDev(1)/2);
      	
      	//rise_1090_bin[Trigger] = sqrt(coin_events);
      	rise1090_range[Trigger][0] = rise_1090_h[Trigger]->GetMean(1) - 5*rise_1090_h[Trigger]->GetStdDev(1);
      	rise1090_range[Trigger][1] = rise_1090_h[Trigger]->GetMean(1) + 5*rise_1090_h[Trigger]->GetStdDev(1);
      	//rise_1090_bin[Trigger] = ( rise1090_range[Trigger][1]-rise1090_range[Trigger][0] )/ (rise_1090_h[Trigger]->GetStdDev(1)/2);
      	
      	//dvdt_bin[Trigger] = sqrt(coin_events);
      	dvdt_range[Trigger][0] = dvdt_fit[Trigger]->GetParameter(1) - 2*dvdt_h[Trigger]->GetStdDev(1);
      	dvdt_range[Trigger][1] = dvdt_fit[Trigger]->GetParameter(1) + 7*dvdt_h[Trigger]->GetStdDev(1);
      	//dvdt_bin[Trigger] = ( dvdt_range[Trigger][1]-dvdt_range[Trigger][0] )/ (dvdt_h[Trigger]->GetStdDev(1)/2);
      	
      	rms_bin[Trigger] = sqrt(coin_events);
      	rms_range[Trigger][0] = rms_h[Trigger]->GetMean(1) - 5*rms_h[Trigger]->GetStdDev(1);
      	rms_range[Trigger][1] = rms_h[Trigger]->GetMean(1) + 5*rms_h[Trigger]->GetStdDev(1);
      	//rms_bin[Trigger] = ( rms_range[Trigger][1]-rms_range[Trigger][0] )/ (rms_h[Trigger]->GetStdDev(1)/2);
      	
      	//charge_bin[Trigger] = sqrt(coin_events);
      	charge_range[Trigger][0] = charge_fit[Trigger]->GetParameter(1) - 2*charge_h[Trigger]->GetStdDev(1);
     	charge_range[Trigger][1] = charge_fit[Trigger]->GetParameter(1) + 7*charge_h[Trigger]->GetStdDev(1);
     	//charge_bin[Trigger] = ( charge_range[Trigger][1]-charge_range[Trigger][0] )/ (charge_h[Trigger]->GetStdDev(1)/2);
		
	  }
		
	std::cout << "-------------" << Form("ch%d", DUT+1) <<"------------------"<< std::endl;
	
	std::cout << "pmax2" << ",," << "rms2" << ",,,,,," << "rise2_1040" << ",,,," << "rise2_1090" << ",,,," << "dvdt2" << ",,,," << " charge2" << std::endl;
	std::cout << pmax_fit[DUT]->GetParameter(1)<<", "<< pmax_fit[DUT]->GetParError(1) << ","  
			  << rms_fit[DUT]->GetParameter(1)<<", "<< rms_fit[DUT]->GetParError(1) << ",,,,,"
 			  << rise_1040_fit[DUT]->GetParameter(1) <<", "<< rise_1040_fit[DUT]->GetParError(1) << ",,,"
 			  << rise_1090_fit[DUT]->GetParameter(1) <<", "<< rise_1090_fit[DUT]->GetParError(1) << ",,,"
			  << dvdt_fit[DUT]->GetParameter(1)<<", "<< dvdt_fit[DUT]->GetParError(1) << ",,,"
			  << charge_fit[DUT]->GetParameter(1)<<", "<<  charge_fit[DUT]->GetParError(1) << std::endl;

	std::cout << "-------------" << Form("ch%d", Trigger+1) <<"------------------"<< std::endl;
	
	std::cout << "pmax3" << ",," << "rms3" << ",,,,,," << "rise3_1040" << ",,,," << "rise3_1090" << ",,,," << "dvdt3" << ",,,," << " charge3" << std::endl;
	std::cout << pmax_fit[Trigger]->GetParameter(1)<<", "<< pmax_fit[Trigger]->GetParError(1) << "," 
			  << rms_fit[Trigger]->GetParameter(1)<<", "<< rms_fit[Trigger]->GetParError(1) << ",,,,,"
			  << rise_1040_fit[Trigger]->GetParameter(1)<<", "<< rise_1040_fit[Trigger]->GetParError(1) << ",,,"
			  << rise_1090_fit[Trigger]->GetParameter(1)<<", "<< rise_1090_fit[Trigger]->GetParError(1) << ",,,"
			  << dvdt_fit[Trigger]->GetParameter(1)<<", "<< dvdt_fit[Trigger]->GetParError(1) << ",,," 
			  << charge_fit[Trigger]->GetParameter(1)<<", "<< charge_fit[Trigger]->GetParError(1) << std::endl;
	
	loadTree->ResetBranchAddresses();
   } 
*/
//-----------------------------------------------------------------------------------------------

void getData::reset()
{
	//loadTree->ResetBranchAddresses();

        //gROOT->Reset();
}


