#include "../hpp/getResolution.h"

std::pair<double,double> res( const char *in_file, double Dut_tmin, double Dut_tmax, double Dut_pmin, double Dut_pmax, double Trig_tmin, double Trig_tmax, double Trig_pmin, double Trig_pmax, bool Save_Mode, const char* addition_cuts, int DUT, int Trig, int cfd_per)
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
	
	TH1D *cfd_his_pre = new TH1D("cfd_his_pre", "prepare cfd histogram",100,1,1);
	
	loadTree->Project("cfd_his_pre", Form("cfd%d[%d] - cfd%d[20]", DUT, cfd_per, Trig), cuts);
	
	double arithmicMean = cfd_his_pre->GetMean(1);
	double arithmicStd  = cfd_his_pre->GetStdDev(1);
	int ent = cfd_his_pre->GetEntries();
	int nbin = sqrt(ent);
	
	double mean = 0.0, std = 0.0;
	
	TF1 *cfd_pre_fitter = new TF1 ("cfd_pre_fit", "gaus");
	
	cfd_pre_fitter-> SetParameter (1, arithmicMean);
			
	cfd_pre_fitter-> SetParameter (2, arithmicStd);
	
	TCanvas *c1 = new TCanvas("c1");	
	c1->cd();
	cfd_his_pre-> SetDirectory(0);	
	int firstFitChecker = cfd_his_pre->Fit(cfd_pre_fitter, "Q");
	
	if( firstFitChecker )
	{	
		std::cout << std::endl;
		std::cout << "bad fit" << std::endl;
		std::cout << "using arithmic mean and std" << std::endl;
		std::cout << std::endl;
		
		mean = cfd_pre_fitter->GetParameter(1);
		std = cfd_pre_fitter->GetParameter(2);
	}
	
	else
	{
		mean = cfd_pre_fitter->GetParameter(1);
		std = cfd_pre_fitter->GetParameter(2);
	}
	
	delete cfd_his_pre;
	//std::cout << "first mean " << mean << std::endl;
	//std::cout << "first std " << std << std::endl;
	//std::cout << "entries " << ent << std::endl;
	
	int verboseLevel = 2;
	
	for(int i = 0; i < 10; i++)   //looping multiple cfd histrogram to ensure range.
	{
		delete gROOT-> FindObject("cfd_hist_looping");
		
		TH1D *cfd_hist_looping;
		
		if (i < 5) 
		{
			cfd_hist_looping = new TH1D ("cfd_hist_looping", "looping cfd histogram", nbin, mean - (i+1) * std, mean + (i+1) * std);
		
			loadTree->Project("cfd_hist_looping", Form("cfd%d[%d] - cfd%d[20]", DUT, cfd_per, Trig), cuts);
			
			TF1 *cfd_looping_fitter = new TF1 ("cfd_looping_fit", "gaus", mean - (i+1) * std, mean + (i+1) * std);
			
			cfd_looping_fitter-> SetParameter (1, mean);
			
			cfd_looping_fitter-> SetParameter (2, std);
		
			int fitChecker = cfd_hist_looping-> Fit (cfd_looping_fitter, "Q 0 R");
		
			if( fitChecker )
			{	
				if(verboseLevel < 1)
				{
					std::cout << std::endl;
					std::cout << "bad fit" << std::endl;
					std::cout << std::endl;
				}			
			}
			
			else
			{
				mean = cfd_looping_fitter-> GetParameter (1);
		
				std = cfd_looping_fitter-> GetParameter (2);
			}
		
			delete cfd_looping_fitter;
		
			delete cfd_hist_looping;
		}
		
		if (i > 10 && i < 20) 
		{
			cfd_hist_looping = new TH1D ("cfd_hist_looping", "looping cfd histogram", nbin, mean - 5.0 * std, mean + 5.0 * std);
		
			loadTree->Project("cfd_hist_looping", Form("cfd%d[%d] - cfd%d[20]", DUT, cfd_per, Trig), cuts);
			
			TF1 *cfd_looping_fitter = new TF1 ("cfd_looping_fit", "gaus", mean - 5.0 * std, mean + 5.0 * std);
			
			cfd_looping_fitter-> SetParameter (1, mean);
			
			cfd_looping_fitter-> SetParameter (2, std);
		
			cfd_hist_looping-> Fit (cfd_looping_fitter, "Q 0 R");
		
			mean = cfd_looping_fitter-> GetParameter (1);
		
			std = cfd_looping_fitter-> GetParameter (2);
		
			delete cfd_looping_fitter;
		
			delete cfd_hist_looping;
		}
	}
	
	TH1D *cfd_his_fit = new TH1D("cfd_his_fit","intermediate process", nbin, mean - 5.0 * std, mean + 5.0 * std);
	
	loadTree->Project("cfd_his_fit", Form("cfd%d[%d] - cfd%d[20]", DUT, cfd_per, Trig), cuts);
	
	TF1 *cfd_inter_fitter = new TF1("cfd_fit", "gaus", mean - 5.0 * std, mean + 5.0 * std);
	
	cfd_inter_fitter-> SetParameter (1, mean);
			
	cfd_inter_fitter-> SetParameter (2, std);
	
	TCanvas *c2 = new TCanvas("c2");	
	c2->cd();
	cfd_his_fit-> SetDirectory(0);	
	cfd_his_fit->Fit(cfd_inter_fitter, "Q R");
	
	mean = cfd_inter_fitter->GetParameter(1);
	std = cfd_inter_fitter->GetParameter(2);
	
	//std::cout << "second mean " << mean << std::endl;
	//std::cout << "second std " << std << std::endl;
	
	//double Scott_width = 3.5*cfd_his_fit->GetStdDev(1)*std::pow(ent,-1.0/3.0);
	//int Scott_bin =  10.0 * std / Scott_width;
	//nbin = Scott_bin;
	
	double FD_width = 2.0 * 1.35 * cfd_his_fit->GetStdDev(1)*std::pow(ent,-1.0/3.0);
	int FD_bin =  10.0 * std / FD_width;
	nbin = FD_bin;

	TH1D *cfd_hist_o = new TH1D("cfd_o", "CFD difference", nbin, mean - 5.0 * std, mean + 5.0 * std);
	
	loadTree->Project("cfd_o", Form("cfd%d[%d] - cfd%d[20]", DUT, cfd_per, Trig), cuts);
	
	TF1 *cfd_post_fitter = new TF1("cfd_post_fit", "gaus", mean - 5.0 * std, mean + 5.0 * std);
	
	cfd_post_fitter-> SetParameter (1, mean);
			
	cfd_post_fitter-> SetParameter (2, std);
	
	TCanvas *c3 = new TCanvas("c3");	
	c3->cd();
	cfd_hist_o-> SetDirectory(0);	
	cfd_hist_o->Fit(cfd_post_fitter, "Q R");
	
	double Sigma = cfd_post_fitter->GetParameter(2);
	double Sigma_err = cfd_post_fitter->GetParError(2);
	
	std::cout << "Res Result: " << Sigma << " , " << Sigma_err << std::endl;
	
	if (Save_Mode)
	{
		std::string substr( in_file+4, strlen(in_file) );
		gSystem->ProcessEvents();
		TImage *img = TImage::Create();
		img->FromPad(c3);
		img->WriteImage(Form("%s_resolution[%d].png", substr.c_str(), cfd_per));
		delete img;
		
		delete cfd_post_fitter;
		delete cfd_hist_o;
		delete c3;
		delete c2;
		delete c1;
	}
	
	else
	{
		delete c3;
		delete c2;
		delete c1;
	}
	
	infile->Close();
	
	return std::make_pair( Sigma, Sigma_err );
}


void jitter_dvdt( const char *in_file, double Dut_tmin, double Dut_tmax, double Dut_pmin, double Dut_pmax, double Trig_tmin, double Trig_tmax, double Trig_pmin, double Trig_pmax, bool Batch_Mode, const char* addition_cuts, int DUT, int Trig, int per)
{

	TFile *infile = TFile::Open(in_file);
	TTree *loadTree = (TTree*) infile->Get("wfm");
	
	const char* Dut_tcut = Form("tmax%d[0] - cfd%d[20]", DUT, Trig);
	const char* Trig_tcut = Form("tmax%d[0] - cfd%d[20]", Trig, Trig);
		
	const char* Dut_pcut = Form("pmax%d[0]", DUT);
	const char* Trig_pcut = Form("pmax%d[0]", Trig);
	
	int numBin = 100;
		
		
	TCut cuts = Form(" %s > %f && %s < %f && "
				
					" %s > %f && %s < %f && "
		
					" %s > %f && %s < %f && "
						
					" %s > %f && %s < %f %s ",
						  
					  Dut_tcut, Dut_tmin, Dut_tcut, Dut_tmax, 
						  
					  Dut_pcut, Dut_pmin, Dut_pcut, Dut_pmax,
						  
					  Trig_tcut, Trig_tmin, Trig_tcut, Trig_tmax, 
						  
					  Trig_pcut, Trig_pmin, Trig_pcut, Trig_pmax, addition_cuts );
	
	TCanvas *oCanvas = new TCanvas("oCanvas", "", 1620, 2000);
	oCanvas->Divide( 1, 4 );
	gStyle->SetOptFit();
	gStyle->SetOptStat();
	
	
	TH1D *prepHistogram = new TH1D( "prepHistogram", Form( "Jitter at CFD %d%% (Preparation)", per ), numBin, 1, 1);
	loadTree->Project( "prepHistogram", Form( "rms%d[0]/dvdt%d[%d]",DUT, DUT, per ), cuts );
	oCanvas->cd( 1 );
	prepHistogram->SetDirectory( 0 );
	prepHistogram->Draw();
	//delete prepHistogram;
	
	double sampleMean = prepHistogram->GetMean( 1 );
	double sampleSigma = prepHistogram->GetStdDev( 1 );
	
	TH1D *interHistogram = new TH1D("interHistogram", Form( "Jitter at CFD %d%% (Intermediate)", per ), numBin, sampleMean - 5.0 * sampleSigma, sampleMean + 20.0 * sampleSigma);
	
	loadTree->Project( "interHistogram", Form( "rms%d[0]/dvdt%d[%d]",DUT, DUT, per ), cuts );

	TF1 *fitter = new TF1( "fitter", "landau" );
	fitter->SetParameter( 1, prepHistogram->GetXaxis()->GetBinCenter(prepHistogram->GetMaximumBin()) );
	fitter->SetParameter( 2, sampleSigma );
	
	oCanvas->cd(2);
	interHistogram->SetDirectory( 0 );
	int emptyFit = interHistogram->Fit( fitter, "Q" );

	double lowerBound, upperBound;
	
	if( emptyFit || fitter->GetParameter(1) < 0.0 )
	{
		std::cout << "empty fit, reset" << std::endl;
		lowerBound = -20;
		upperBound = 300;
		numBin = 200;
	}
	else
	{
		lowerBound = fitter->GetParameter(1) - 5.0 * fitter->GetParameter(2);
		upperBound = fitter->GetParameter(1) + 20.0 * fitter->GetParameter(2);
	}	
	
	TH1D *interHistogram2 = new TH1D("interHistogram2", Form( "Jitter at CFD %d%% (Intermediate2)", per ), numBin, lowerBound, upperBound);
	
	loadTree->Project("interHistogram2", Form("rms%d[0]/dvdt%d[%d]",DUT, DUT, per), cuts);
	
	fitter->SetParameter( 1, sampleMean );
	//jitter_fitter->SetParameter( 2, );
	oCanvas->cd(3);
	interHistogram2->SetDirectory( 0 );
	interHistogram2->Fit( fitter, "Q" );
	interHistogram2->Draw();

	TH1D *postHistogram = new TH1D("postHistogram", Form( "Jitter at CFD %d%% (Finalized)", per ), numBin, fitter->GetParameter(1) - 5.0 * fitter->GetParameter(2), fitter->GetParameter(1) + 20.0 * fitter->GetParameter(2));
	
	loadTree->Project("postHistogram", Form("rms%d[0]/dvdt%d[%d]", DUT, DUT, per), cuts);
	
	oCanvas->cd(4);
	postHistogram->SetDirectory( 0 );
	postHistogram->Fit(fitter,"Q");

	std::cout << "Jitter Result: " << fitter->GetParameter(1) << " , " << fitter->GetParError(1) << std::endl;
	
	if (Batch_Mode)
	{
		gSystem->ProcessEvents();
		TImage *img = TImage::Create();
		img->FromPad(oCanvas);
		img->WriteImage(Form("%s_jitter.png", in_file));
		delete img;
		
		delete fitter;
		delete prepHistogram;
		delete interHistogram;
		delete interHistogram2;
		delete postHistogram;
		delete oCanvas;
	}
}


std::pair<double,double> fineRes( const char *in_file, double Dut_tmin, double Dut_tmax, double Dut_pmin, double Dut_pmax, double Trig_tmin, double Trig_tmax, double Trig_pmin, double Trig_pmax, bool Save_Mode, const char* addition_cuts, int DUT, int Trig, int cfd_per)
{
	TFile *infile = TFile::Open(in_file);
	
	TTree *loadTree = (TTree*) infile->Get("wfm");
	
	const char* Dut_tcut = Form("tmax%d[0] - fineCFDRise%d[100]", DUT, Trig);
	const char* Trig_tcut = Form("tmax%d[0] - fineCFDRise%d[100]", Trig, Trig);
		
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
	
	TH1D *cfd_his_pre = new TH1D("cfd_his_pre", "prepare cfd histogram",100,1,1);
	
	loadTree->Project("cfd_his_pre", Form("fineCFDRise%d[%d] - fineCFDRise%d[100]", DUT, cfd_per, Trig), cuts);
	
	double arithmicMean = cfd_his_pre->GetMean(1);
	double arithmicStd  = cfd_his_pre->GetStdDev(1);
	int ent = cfd_his_pre->GetEntries();
	int nbin = sqrt(ent);
	
	double mean = 0.0, std = 0.0;
	
	TF1 *cfd_pre_fitter = new TF1 ("cfd_pre_fit", "gaus");
	
	cfd_pre_fitter-> SetParameter (1, arithmicMean);
			
	cfd_pre_fitter-> SetParameter (2, arithmicStd);
	
	TCanvas *c1 = new TCanvas("c1");	
	c1->cd();
	cfd_his_pre-> SetDirectory(0);	
	int firstFitChecker = cfd_his_pre->Fit(cfd_pre_fitter, "Q");
	
	if( firstFitChecker )
	{	
		std::cout << std::endl;
		std::cout << "bad fit" << std::endl;
		std::cout << "using arithmic mean and std" << std::endl;
		std::cout << std::endl;
		
		mean = cfd_pre_fitter->GetParameter(1);
		std = cfd_pre_fitter->GetParameter(2);
	}
	
	else
	{
		mean = cfd_pre_fitter->GetParameter(1);
		std = cfd_pre_fitter->GetParameter(2);
	}
	
	delete cfd_his_pre;
	//std::cout << "first mean " << mean << std::endl;
	//std::cout << "first std " << std << std::endl;
	//std::cout << "entries " << ent << std::endl;
	
	int verboseLevel = 2;
	
	for(int i = 0; i < 10; i++)   //looping multiple cfd histrogram to ensure range.
	{
		delete gROOT-> FindObject("cfd_hist_looping");
		
		TH1D *cfd_hist_looping;
		
		if (i < 5) 
		{
			cfd_hist_looping = new TH1D ("cfd_hist_looping", "looping cfd histogram", nbin, mean - (i+1) * std, mean + (i+1) * std);
		
			loadTree->Project("cfd_hist_looping", Form("fineCFDRise%d[%d] - fineCFDRise%d[100]", DUT, cfd_per, Trig), cuts);
			
			TF1 *cfd_looping_fitter = new TF1 ("cfd_looping_fit", "gaus", mean - (i+1) * std, mean + (i+1) * std);
			
			cfd_looping_fitter-> SetParameter (1, mean);
			
			cfd_looping_fitter-> SetParameter (2, std);
		
			int fitChecker = cfd_hist_looping-> Fit (cfd_looping_fitter, "Q 0 R");
		
			if( fitChecker )
			{	
				if(verboseLevel < 1)
				{
					std::cout << std::endl;
					std::cout << "bad fit" << std::endl;
					std::cout << std::endl;
				}			
			}
			
			else
			{
				mean = cfd_looping_fitter-> GetParameter (1);
		
				std = cfd_looping_fitter-> GetParameter (2);
			}
		
			delete cfd_looping_fitter;
		
			delete cfd_hist_looping;
		}
		
		if (i > 10 && i < 20) 
		{
			cfd_hist_looping = new TH1D ("cfd_hist_looping", "looping cfd histogram", nbin, mean - 5.0 * std, mean + 5.0 * std);
		
			loadTree->Project("cfd_hist_looping", Form("fineCFDRise%d[%d] - fineCFDRise%d[100]", DUT, cfd_per, Trig), cuts);
			
			TF1 *cfd_looping_fitter = new TF1 ("cfd_looping_fit", "gaus", mean - 5.0 * std, mean + 5.0 * std);
			
			cfd_looping_fitter-> SetParameter (1, mean);
			
			cfd_looping_fitter-> SetParameter (2, std);
		
			cfd_hist_looping-> Fit (cfd_looping_fitter, "Q 0 R");
		
			mean = cfd_looping_fitter-> GetParameter (1);
		
			std = cfd_looping_fitter-> GetParameter (2);
		
			delete cfd_looping_fitter;
		
			delete cfd_hist_looping;
		}
	}
	
	TH1D *cfd_his_fit = new TH1D("cfd_his_fit","intermediate process", nbin, mean - 5.0 * std, mean + 5.0 * std);
	
	loadTree->Project("cfd_his_fit", Form("fineCFDRise%d[%d] - fineCFDRise%d[100]", DUT, cfd_per, Trig), cuts);
	
	TF1 *cfd_inter_fitter = new TF1("cfd_fit", "gaus", mean - 5.0 * std, mean + 5.0 * std);
	
	cfd_inter_fitter-> SetParameter (1, mean);
			
	cfd_inter_fitter-> SetParameter (2, std);
	
	TCanvas *c2 = new TCanvas("c2");	
	c2->cd();
	cfd_his_fit-> SetDirectory(0);	
	cfd_his_fit->Fit(cfd_inter_fitter, "Q R");
	
	mean = cfd_inter_fitter->GetParameter(1);
	std = cfd_inter_fitter->GetParameter(2);
	
	//std::cout << "second mean " << mean << std::endl;
	//std::cout << "second std " << std << std::endl;
	
	//double Scott_width = 3.5*cfd_his_fit->GetStdDev(1)*std::pow(ent,-1.0/3.0);
	//int Scott_bin =  10.0 * std / Scott_width;
	//nbin = Scott_bin;
	
	double FD_width = 2.0 * 1.35 * cfd_his_fit->GetStdDev(1)*std::pow(ent,-1.0/3.0);
	int FD_bin =  10.0 * std / FD_width;
	nbin = FD_bin;

	TH1D *cfd_hist_o = new TH1D("cfd_o", "CFD difference", nbin, mean - 5.0 * std, mean + 5.0 * std);
	
	loadTree->Project("cfd_o", Form("fineCFDRise%d[%d] - fineCFDRise%d[100]", DUT, cfd_per, Trig), cuts);
	
	TF1 *cfd_post_fitter = new TF1("cfd_post_fit", "gaus", mean - 5.0 * std, mean + 5.0 * std);
	
	cfd_post_fitter-> SetParameter (1, mean);
			
	cfd_post_fitter-> SetParameter (2, std);
	
	TCanvas *c3 = new TCanvas("c3");	
	c3->cd();
	cfd_hist_o-> SetDirectory(0);	
	cfd_hist_o->Fit(cfd_post_fitter, "Q R");
	
	double Sigma = cfd_post_fitter->GetParameter(2);
	double Sigma_err = cfd_post_fitter->GetParError(2);
	
	std::cout << "Res Result: " << Sigma << " , " << Sigma_err << std::endl;
	
	if (Save_Mode)
	{
		std::string substr( in_file+4, strlen(in_file) );
		gSystem->ProcessEvents();
		TImage *img = TImage::Create();
		img->FromPad(c3);
		img->WriteImage(Form("%s_FineResolution[%d].png", substr.c_str(), cfd_per));
		delete img;
		
		delete cfd_post_fitter;
		delete cfd_hist_o;
		delete c3;
		delete c2;
		delete c1;
	}
	
	else
	{
		delete c3;
		delete c2;
		delete c1;
	}
	
	infile->Close();
	
	return std::make_pair( Sigma, Sigma_err );
}



