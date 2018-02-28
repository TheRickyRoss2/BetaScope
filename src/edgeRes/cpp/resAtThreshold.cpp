#include "../hpp/edgeRes.h"

std::pair<double,double> resAtThreshold( const char *in_file, double Dut_tmin, double Dut_tmax, double Dut_pmin, double Dut_pmax, double Trig_tmin, double Trig_tmax, double Trig_pmin, double Trig_pmax, int threshold, const char* addition_cuts, bool Save_Mode, int DUT, int Trig)
{
	TFile *infile = TFile::Open(in_file);
	
	TTree *loadTree = (TTree*) infile->Get("wfm");
	
	const char* Dut_tcut = Form("tmax%d[0] - cfd%d[20]", DUT, Trig);
	const char* Trig_tcut = Form("tmax%d[0] - cfd%d[20]", Trig, Trig);
		
	const char* Dut_pcut = Form("pmax%d[0]", DUT);
	const char* Trig_pcut = Form("pmax%d[0]", Trig);
		
		
	TCut cuts = Form(" %s > %f && %s < %f && "
				
					" %s > %f && %s < %f && "
		
					" %s > %f && %s < %f && "
						
					" %s > %f && %s < %f %s ",
						  
					  Dut_tcut, Dut_tmin, Dut_tcut, Dut_tmax, 
						  
					  Dut_pcut, Dut_pmin, Dut_pcut, Dut_pmax,
						  
					  Trig_tcut, Trig_tmin, Trig_tcut, Trig_tmax, 
						  
					  Trig_pcut, Trig_pmin, Trig_pcut, Trig_pmax, addition_cuts );
	
	TH1D *preHistogram = new TH1D("preHistogram", "Preprare threhold time - trigger CFD[20] histogram", 100, 1, 1);
	
	loadTree->Project("preHistogram", Form("thTime%d[%d] - cfd%d[20]", DUT, threshold, Trig), cuts);
	
	TCanvas *c1 = new TCanvas("c1", "", 1000, 1000);	
	c1->Divide(1, 3);
	gStyle->SetOptFit();
	gStyle->SetOptStat();
	
	c1->cd(1);
	preHistogram-> SetDirectory(0);
	preHistogram->Draw();
	
	double mean = preHistogram->GetMean(1);
	double std = preHistogram->GetStdDev(1);
	int ent = preHistogram->GetEntries();
	int nbin = sqrt(ent);
	
	if(ent < 20) return std::make_pair( -1.0, -1.0 );
		
	for(int i = 0; i < 10; i++)   //looping multiple cfd histrogram to ensure range.
	{
		delete gROOT-> FindObject("loopHistogram");
		
		TH1D *loopHistogram;
		
		if (i < 5) 
		{
			loopHistogram = new TH1D ("loopHistogram", "Looping over multiple histograms", nbin, mean - (i+1) * std, mean + (i+1) * std);
		
			loadTree->Project("loopHistogram", Form("thTime%d[%d] - cfd%d[20]", DUT, threshold, Trig), cuts);
			
			TF1 *loopingFitter = new TF1 ("loopingFitter", "gaus");
			
			loopingFitter->SetParameter( 1, mean );
		
			loopingFitter->SetParameter( 2, std );
		
			loopHistogram->Fit( loopingFitter, "Q 0" );
		
			mean = loopingFitter->GetParameter( 1 );
		
			std = loopingFitter->GetParameter( 2 );
		
			delete loopingFitter;
		
			delete loopHistogram;
		}
		
		if (i > 5 && i < 10) 
		{
			loopHistogram = new TH1D ("loopHistogram", "Looping over multiple histograms", nbin, mean - 5.0 * std, mean + 5.0 * std);
		
			loadTree->Project("loopHistogram", Form("thTime%d[%d] - cfd%d[20]", DUT, threshold, Trig), cuts);
			
			TF1 *loopingFitter = new TF1 ("loopingFitter", "gaus");
			
			loopingFitter->SetParameter( 1, mean );
			
			loopingFitter->SetParameter( 2, std );
		
			loopHistogram->Fit( loopingFitter, "Q 0" );
		
			mean = loopingFitter->GetParameter( 1 );
		
			std = loopingFitter->GetParameter( 2 );
		
			delete loopingFitter;
		
			delete loopHistogram;
		}
	}
	
	TH1D *interHistogram = new TH1D("interHistogram","Intermediate threhold time - trigger CFD[20] histogram", nbin, mean - 5.0 * std, mean + 5.0 * std);
	loadTree->Project("interHistogram", Form( "thTime%d[%d] - cfd%d[20]", DUT, threshold, Trig ), cuts);
	
	TF1 *interFitter = new TF1("interFitter", "gaus");
	interFitter->SetParameter( 1, mean );		
	interFitter->SetParameter( 2, std );
	
	c1->cd(2);
	interHistogram->SetDirectory( 0 );	
	interHistogram->Fit( interFitter, "Q" );
	
	mean = interFitter->GetParameter( 1 );
	std = interFitter->GetParameter( 2 );

	TH1D *postHistogram = new TH1D( "postHistogram", "Finalized threhold time - trigger CFD[20] histogram", nbin, mean - 5.0 * std, mean + 5.0 * std);
	loadTree->Project("postHistogram", Form("thTime%d[%d] - cfd%d[20]", DUT, threshold, Trig), cuts);
	
	TF1 *postFitter = new TF1("postFitter", "gaus");
	postFitter->SetParameter( 1, mean );		
	postFitter->SetParameter( 2, std );
	
	c1->cd(3);
	postHistogram->SetDirectory( 0 );	
	postHistogram->Fit( postFitter, "Q" );
	
	double Sigma = postFitter->GetParameter( 2 );
	double Sigma_err = postFitter->GetParError( 2 );
	
	std::cout << "Res Result: " << Sigma << " , " << Sigma_err << std::endl;
	
	if (Save_Mode)
	{
		gSystem->ProcessEvents();
		TImage *img = TImage::Create();
		img->FromPad(c1);
		std::string substr( in_file+4, strlen(in_file) );
		img->WriteImage(Form("%s_threshold_resolution[%d].png", substr.c_str(), threshold));
		delete img;
		
		delete postFitter;
		delete postHistogram;
		delete c1;
	}
	
	else
	{
		delete c1;
	}
	
	infile->Close();
	
	return std::make_pair( Sigma, Sigma_err );
}

