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

//(mean, mean err, binNum, range_min, range_max)
std::tuple<double,double,int,double,double> getData::Make_Histogram(const char* path, std::string HistName, std::string expression, std::string FitFunc, TCut cuts, int Channel, int binNum, double range_min, double range_max, int draw, bool saveFile, bool Roo, bool confine)
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
      	return std::make_tuple(-1,-1, -1, -1, -1);
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
      	range_min= -100.0;
      	range_max = 50000.0;
      	return std::make_tuple( sampleMean, sampleSigma/histo->GetEntries(), binNum, range_min, range_max );
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

      return std::make_tuple( Par, ParErr, binNum, range_min, range_max );
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

void getData::processHistogram(const char* path,
	                            double Dut_tmin,
															double Dut_tmax,
															double Dut_pmin,
															double Dut_pmax,
															double Trig_tmin,
															double Trig_tmax,
															double Trig_pmin,
															double Trig_pmax,
															bool saveFile,
															bool logging )
{
	gROOT->Reset();

	TCut temp_cuts = getData::setcuts(Dut_tmin, Dut_tmax, Dut_pmin, Dut_pmax, Trig_tmin, Trig_tmax, Trig_pmin, Trig_pmax, this->DUT, this->Trigger);

	public_cuts = temp_cuts;

	current_file = path;

	std::map<std::string,std::pair<double,double>> oData_dut;
	std::map<std::string,std::pair<double,double>> oData_trig;

	int fineAdjustmentTime = 0;

	for( int i = 0; i < this->plotArg[0].size(); i++ )
	{
		double par;
		double par_err;

		std::tuple<int,double,double> fitAdjust = {this->ini_bin, this->ini_rangeMin, this->ini_rangeMin};

		if( i == 3 )
		{
			std::string plotName = this->plotArg[0].at(i).at(0);

			while( fineAdjustmentTime < 5 )
			{
				std::tuple<double,double,int,double,double> temp =
					getData::Make_Histogram(path,
					                        this->plotArg[0].at(i).at(0),
					                        this->plotArg[0].at(i).at(1),
					                        this->plotArg[0].at(i).at(2),
					                        temp_cuts,
					                        DUT,
																	std::get<0>(fitAdjust),
																	std::get<1>(fitAdjust),
																	std::get<2>(fitAdjust),
					                        0,
					                        false,
					                        false,
					                        true);

				std::get<0>(fitAdjust) = std::get<2>(temp);
				std::get<1>(fitAdjust) = std::get<3>(temp);
				std::get<2>(fitAdjust) = std::get<4>(temp);

				fineAdjustmentTime ++;
			}

			std::tuple<double,double,int,double,double> out_tuple = getData::Make_Histogram(
																																	path,
																																	this->plotArg[0].at(i).at(0),
																																	this->plotArg[0].at(i).at(1),
																																	this->plotArg[0].at(i).at(2),
																																	temp_cuts,
																																	DUT,
																																	std::get<0>(fitAdjust),
																																	std::get<1>(fitAdjust),
																																	std::get<2>(fitAdjust),
																																	1,
																																	true,
																																	false,
																																	true );

				par = std::get<0>(out_tuple);
				par_err = std::get<1>(out_tuple);

			fineAdjustmentTime = 0;

			std::pair<double,double> output = std::make_pair( par, par_err );

			oData_dut.insert( std::pair<std::string,std::pair<double,double>>( plotName, output ) );
		}
		else
		{
			std::string plotName = this->plotArg[0].at(i).at(0);

			while( fineAdjustmentTime < 5 )
			{
				std::tuple<double,double,int,double,double> temp =
					getData::Make_Histogram(path,
																	this->plotArg[0].at(i).at(0),
																	this->plotArg[0].at(i).at(1),
																	this->plotArg[0].at(i).at(2),
																	temp_cuts,
																	DUT,
																	std::get<0>(fitAdjust),
																	std::get<1>(fitAdjust),
																	std::get<2>(fitAdjust),
																	0,
																	false,
																	false,
																  false);

				std::get<0>(fitAdjust) = std::get<2>(temp);
				std::get<1>(fitAdjust) = std::get<3>(temp);
				std::get<2>(fitAdjust) = std::get<4>(temp);

				fineAdjustmentTime ++;
			}

			std::tuple<double,double,int,double,double> out_tuple = getData::Make_Histogram(
																																	path,
																																	this->plotArg[0].at(i).at(0),
																																	this->plotArg[0].at(i).at(1),
																																	this->plotArg[0].at(i).at(2),
																																	temp_cuts,
																																	DUT,
																																	std::get<0>(fitAdjust),
																																	std::get<1>(fitAdjust),
																																	std::get<2>(fitAdjust),
																																	1,
																																	true,
																																	false,
																																	false );

				par = std::get<0>(out_tuple);
				par_err = std::get<1>(out_tuple);

			fineAdjustmentTime = 0;

			std::pair<double,double> output = std::make_pair( par, par_err );

			oData_dut.insert( std::pair<std::string,std::pair<double,double>>( plotName, output ) );
		}
	}

	for( int i = 0; i < this->plotArg[1].size(); i++ )
	{
		double par;
		double par_err;

		std::string plotName = this->plotArg[1].at(i).at(0);

		std::tuple<int,double,double> fitAdjust = {this->ini_bin, this->ini_rangeMin, this->ini_rangeMin};

		while( fineAdjustmentTime < 5 )
		{
			std::tuple<double,double,int,double,double> temp =
				getData::Make_Histogram(path,
																this->plotArg[1].at(i).at(0),
																this->plotArg[1].at(i).at(1),
																this->plotArg[1].at(i).at(2),
																temp_cuts,
																Trigger,
																std::get<0>(fitAdjust),
																std::get<1>(fitAdjust),
																std::get<2>(fitAdjust),
																0,
																false,
																false,
															  false);

			std::get<0>(fitAdjust) = std::get<2>(temp);
			std::get<1>(fitAdjust) = std::get<3>(temp);
			std::get<2>(fitAdjust) = std::get<4>(temp);

			fineAdjustmentTime ++;
		}
		
		std::tuple<double,double,int,double,double> out_tuple = getData::Make_Histogram(
																																path,
																																this->plotArg[1].at(i).at(0),
																																this->plotArg[1].at(i).at(1),
																																this->plotArg[1].at(i).at(2),
																																temp_cuts,
																																Trigger,
																																std::get<0>(fitAdjust),
																																std::get<1>(fitAdjust),
																																std::get<2>(fitAdjust),
																																1,
																																true,
																																false,
																																false );

			par = std::get<0>(out_tuple);
			par_err = std::get<1>(out_tuple);

		fineAdjustmentTime = 0;

		std::pair<double,double> output = std::make_pair( par, par_err );

		oData_trig.insert( std::pair<std::string,std::pair<double,double>>( plotName, output ) );
	}

	if(logging)
	{
		std::cout << "-------------" << Form("ch%d", this->DUT+1) <<"------------------"<< std::endl;

		std::cout << "PulseArea2" << ",,,,,,," << "pmax2" << "," << "rms2" << ",,,,," << "rise2_1090" << "," << "dvdt2" << ",,,,,,,,," << " FWHM2" << std::endl;
		std::cout << oData_dut["PulseArea"].first <<", "<< oData_dut["PulseArea"].second << ",,,,,,,"
				      << oData_dut["Pmax"].first      <<", "<< oData_dut["Pmax"].second      << ","
				      << oData_dut["RMS"].first       <<", "<< oData_dut["RMS"].second       << ",,,,,"
	 			      << oData_dut["Rise Time"].first <<", "<< oData_dut["Rise Time"].second << ","
				      << oData_dut["dvdt"].first      <<", "<< oData_dut["dvdt"].second      << ",,,,,,,,,"
				      << oData_dut["FWHM"].first      <<", "<< oData_dut["FWHM"].second      << std::endl;

		std::cout << "-------------" << Form("ch%d", this->Trigger+1) <<"------------------"<< std::endl;

		std::cout << "charPulseArea3" << ",,,,,,," << "pmax3" << "," << "rms3" << ",,,,," << "rise3_1090" << "," << "dvdt3" << ",,,,,,,,," << " FWHM3" << std::endl;
		std::cout << oData_trig["PulseArea"].first <<", "<< oData_trig["PulseArea"].second << ",,,,,,,"
				      << oData_trig["Pmax"].first      <<", "<< oData_trig["Pmax"].second      << ","
				      << oData_trig["RMS"].first       <<", "<< oData_trig["RMS"].second       << ",,,,,"
	 			      << oData_trig["Rise Time"].first <<", "<< oData_trig["Rise Time"].second << ","
				      << oData_trig["dvdt"].first      <<", "<< oData_trig["dvdt"].second      << ",,,,,,,,,"
				      << oData_trig["FWHM"].first      <<", "<< oData_trig["FWHM"].second      << std::endl;

		std::cout << std::endl;
		std::cout << std::endl;
	}

	if(saveFile)
	{
		std::ofstream ofile;

		std::string prefix = path;

		ofile.open ( "_results.txt", std::ios::app);

		ofile << "-------------" << Form("ch%d", this->DUT+1) <<"------------------"<< std::endl;

		ofile << "charge2" << ",,,,,,," << "pmax2" << "," << "rms2" << ",,,,," << "rise2_1090" << "," << "dvdt2" << ",,,,,,,,," << " FWHM2" << std::endl;
		ofile << oData_dut["PulseArea"].first <<", "<< oData_dut["PulseArea"].second << ",,,,,,,"
				      << oData_dut["Pmax"].first      <<", "<< oData_dut["Pmax"].second      << ","
				      << oData_dut["RMS"].first       <<", "<< oData_dut["RMS"].second       << ",,,,,"
	 			      << oData_dut["Rise Time"].first <<", "<< oData_dut["Rise Time"].second << ","
				      << oData_dut["dvdt"].first      <<", "<< oData_dut["dvdt"].second      << ",,,,,,,,,"
				      << oData_dut["FWHM"].first      <<", "<< oData_dut["FWHM"].second      << std::endl;

		ofile << "-------------" << Form("ch%d", this->Trigger+1) <<"------------------"<< std::endl;

		ofile << "charge3" << ",,,,,,," << "pmax3" << "," << "rms3" << ",,,,," << "rise3_1090" << "," << "dvdt3" << ",,,,,,,,," << " FWHM3" << std::endl;
		ofile << oData_trig["PulseArea"].first <<", "<< oData_trig["PulseArea"].second << ",,,,,,,"
				      << oData_trig["Pmax"].first      <<", "<< oData_trig["Pmax"].second      << ","
				      << oData_trig["RMS"].first       <<", "<< oData_trig["RMS"].second       << ",,,,,"
	 			      << oData_trig["Rise Time"].first <<", "<< oData_trig["Rise Time"].second << ","
				      << oData_trig["dvdt"].first      <<", "<< oData_trig["dvdt"].second      << ",,,,,,,,,"
				      << oData_trig["FWHM"].first      <<", "<< oData_trig["FWHM"].second      << std::endl;

		ofile << std::endl;
		ofile << std::endl;
	}


	else
	{}
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
