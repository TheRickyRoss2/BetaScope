#include "../hpp/jitterScan.h"

void jitterScan( const char *in_file, double Dut_tmin, double Dut_tmax, double Dut_pmin, double Dut_pmax, double Trig_tmin, double Trig_tmax, double Trig_pmin, double Trig_pmax, std::string SensorName, std::string Bias, std::string Fluence, std::string Temperature, std::string fitFunction )
{
	gROOT->SetBatch(true);
	TFile *infile = TFile::Open(in_file);
	TTree *loadTree = (TTree*) infile->Get("wfm");
	
	const char* Dut_tcut = Form("tmax%d[0] - cfd%d[20]", 2, 3);
	const char* Trig_tcut = Form("tmax%d[0] - cfd%d[20]", 3, 3);
		
	const char* Dut_pcut = Form("pmax%d[0]", 2);
	const char* Trig_pcut = Form("pmax%d[0]", 3);

	TF1 *fitter = new TF1( "fitter", fitFunction.c_str() );
	double MPV = 0.0;
	double Sigma = 0.0;
	int nbin = sqrt(loadTree->GetEntries());
			
	TCut cuts = Form(" %s > %f && %s < %f && "
				
					" %s > %f && %s < %f && "
		
					" %s > %f && %s < %f && "
						
					" %s > %f && %s < %f  ",
						  
					  Dut_tcut, Dut_tmin, Dut_tcut, Dut_tmax, 
						  
					  Dut_pcut, Dut_pmin, Dut_pcut, Dut_pmax,
						  
					  Trig_tcut, Trig_tmin, Trig_tcut, Trig_tmax, 
						  
					  Trig_pcut, Trig_pmin, Trig_pcut, Trig_pmax );
	
	bool header = true;
	
	for(int scan=0; scan<100; scan++)
	{
		TH1D *preHistogram = new TH1D( "preHistogram", "First Jitter Histogram", 100, 1, 1 );
		
		loadTree->Project( "preHistogram", Form("rms2/dvdt2[%d]", scan), cuts);
	
		preHistogram->Fit( fitter ,"Q 0");
		
		MPV = fitter->GetParameter(1);
		
		Sigma = fitter->GetParameter(2);
		
		double Ari_mean = preHistogram->GetMean();
		
		double Ari_STD = preHistogram->GetStdDev();
		
		for ( int i = 0; i < 20; i++)
		{	
				
			TH1D *loopHistogram;
			
			const char *name = "loopHistogram";
			
			if (i < 5) 
			{ 
				loopHistogram = new TH1D( name, " ", nbin, MPV - 3 * Sigma, MPV + (3+i) * Sigma );
				
				loadTree->Project( name, Form("rms2/dvdt2[%d]", scan), cuts); 
				
				fitter->SetParameter( 1, MPV );
				
				fitter->SetParameter( 2, Sigma );
				
				int empty = loopHistogram->Fit(fitter ,"Q 0");
				
				if(empty)
				{
					std::cout << "bad fit" << std::endl;
					std::cout << "resetting MPV and Sigma"  << std::endl;
					MPV = Ari_mean;
					Sigma = Ari_STD;
				}
				
				else
				{
					MPV = fitter->GetParameter(1);
		
					Sigma = fitter->GetParameter(2);	
				}
				
				delete gROOT->FindObject(name);
			}
			
			if ( i > 5 && i < 10) 
			{ 
				loopHistogram = new TH1D( name, " ", nbin, MPV - 5 * Sigma, MPV + i * Sigma );
				
				loadTree->Project( name, Form("rms2/dvdt2[%d]", scan), cuts); 
				
				fitter->SetParameter(1,MPV);
				
				fitter->SetParameter(2,Sigma);
				
				loopHistogram->Fit(fitter ,"Q 0");
				
				MPV = fitter->GetParameter(1);
		
				Sigma = fitter->GetParameter(2);	
				
				delete gROOT->FindObject(name);	
			}
			
			if ( i > 10 && i < 15) 
			{ 
				loopHistogram = new TH1D( name, " ", nbin, MPV - 5 * Sigma, MPV + i * Sigma );
				
				loadTree->Project( name, Form("rms2/dvdt2[%d]", scan), cuts); 
				
				fitter->SetParameter(1,MPV);
				
				fitter->SetParameter(2,Sigma);
				
				loopHistogram->Fit(fitter ,"Q 0");
				
				MPV = fitter->GetParameter(1);
		
				Sigma = fitter->GetParameter(2);	
				
				delete gROOT->FindObject(name);	
			}
			
			if ( i > 15 && i < 20) 
			{ 
				loopHistogram = new TH1D( name, " ", nbin, MPV - 5 * Sigma, MPV + i * Sigma );
				
				loadTree->Project( name, Form("rms2/dvdt2[%d]", scan), cuts);
				
				loopHistogram->Fit(fitter ,"Q 0");
				
				MPV = fitter->GetParameter(1);
		
				Sigma = fitter->GetParameter(2);	
				
				delete gROOT->FindObject(name);
			}
		}
	
		TCanvas *outCanvas = new TCanvas("outCanvas", "", 1620, 1000);
		gStyle->SetOptFit();
		gStyle->SetOptStat();
		outCanvas->cd();
	
		TH1D *postHistogram = new TH1D( "postHistogram", "Finalized Jitter Histogram", nbin, MPV - 5 * Sigma, MPV + 20 * Sigma );
		
		loadTree->Project( "postHistogram", Form("rms2/dvdt2[%d]", scan), cuts);
		
		postHistogram->Fit( fitter ,"Q" );
		
		std::cout<< fitter->GetParameter(1) << std::endl;
		
		
		if (true) //saving image and txt
      	{
      		std::string substr( in_file+4, strlen(in_file) );
      		
      		std::ofstream ofile;
      		ofile.open ( Form("%s_jitter_scan.txt", substr.c_str() ), std::ios::app);
      		
      		if(header)
      		{
      			ofile << "Sensor: " << SensorName << std::endl;
      			ofile << "Bias: " << Bias << std::endl;
      			ofile << "Fluence: " << Fluence << std::endl;
      			ofile << "Temperature: " << Temperature << std::endl;
      			ofile << "CFD" << " " << "Jitter [ps]" << " " << "Error [ps]" << std::endl;
      			header = false;
      		}
      		
      		ofile << scan << " " << fitter->GetParameter(1) << " " << fitter->GetParError(1) << std::endl;
      		ofile.close();
      		//delete ofile;
      		
      		postHistogram->Draw();
      		gSystem->ProcessEvents();
      		TImage *img = TImage::Create();
      		img->FromPad (outCanvas);
      		img->WriteImage(Form("%s_jitter[%d].png", substr.c_str(), scan));
      		delete img;
      	}
		
		delete preHistogram;
		delete postHistogram;
		delete outCanvas;
	}
	
	//bash command
	if (true)
	{
		gROOT->ProcessLine(".! mkdir jitter_scan_Plots");
		gROOT->ProcessLine(".! mv *.png jitter_scan_Plots");
		//gROOT->ProcessLine(".! mv *jitter_scan.txt jitter_scan_Plots");
	}	
	
	gROOT->SetBatch(false);	
}

