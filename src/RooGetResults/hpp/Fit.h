
std::pair<double, double> Roo_AutoGet::LandauFit (std::string WhichPar, int Channel, RooDataSet * DataSet, RooRealVar *Var, std::pair<double, double> range, bool SaveImg)
{
	double VarRange_Mean = DataSet->mean(*Var);
	double VarRange_Min = DataSet->mean(*Var) - 10.0 * DataSet->sigma(*Var);
	double VarRange_Max = DataSet->mean(*Var) + 10.0 * DataSet->sigma(*Var); 

	RooRealVar lmean("lmean", "landau mean", VarRange_Mean, VarRange_Min, VarRange_Max);
	RooRealVar lsigma("lsigma", "landau sigma", DataSet->sigma(*Var), -5.0 * DataSet->sigma(*Var), 5.0 * DataSet->sigma(*Var));
	
	//RooRealVar gmean("gmean", "gaus mean", (range.second-range.first)/2, range.first, range.second);
	//RooRealVar gsigma("gsigma", "gaus sigma", (range.second-range.first)/2, range.first, range.second);
	
	//cout<< range.first << endl;
	//cout<< (range.second-range.first)/2 << endl;
	//cout<< range.second << endl;
	
	RooRealVar n("n","number of events", 1, 0 , coincident_Events);
	
	RooLandau landau("landau", "landau", *Var, lmean, lsigma);
	//RooExtendPdf landauExt("landauExt", "landauExt", landau, n);
	
	landau.fitTo(*DataSet);
	//landauExt.fitTo(*DataSet);//, Range(range.first, range.second));
	
	RooPlot* frame = Var->frame ( lmean.getVal() - 8.0 * lsigma.getVal(),  lmean.getVal() + lsigma.getVal() * 20.0 );
	
	/*std::cout<<"landau"<<std::endl;
	std::cout<< mean.getVal() - 3 * sigma.getVal() <<std::endl;
	std::cout<< mean.getVal() + 15*sigma.getVal() <<std::endl;
	std::cout<<"landau"<<std::endl;*/
	
	//RooPlot* frame = Var->frame();
	
	//RooDataHist DataHist("DataHist", "DataHist", *Var, *DataSet);
	
	
	DataSet->plotOn (frame, Normalization(coincident_Events, RooAbsReal::NumEvent));
	DataSet->statOn(frame,Layout(0.45,0.90,0.8));
	//DataHist.plotOn (frame, Binning(n.getVal()), Normalization(10000, RooAbsReal::NumEvent));
	
	
	//landauExt.plotOn(frame);
	//landauExt.paramOn(frame,Layout(0.50, 0.90, 0.5));
	
	landau.plotOn(frame);
	landau.paramOn(frame,Layout(0.45, 0.90, 0.5));
	
	//landau.plotOn (frame);
	TCanvas *oCanvas;
	
	
	if (SaveImg)
	{
		gROOT->SetBatch(true);
		oCanvas = new TCanvas("oCanvas");
		oCanvas->cd();
		
		frame->Draw();
		
		gSystem->ProcessEvents();
      	TImage *img = TImage::Create();
      	img->FromPad (oCanvas);
      	img->WriteImage( Form("%s_%s_ch%d_Roo.png", ifile_name.c_str(), WhichPar.c_str(), Channel) );
      	delete img;
      	delete frame;
      	delete oCanvas;
	}
	
	else 
	{
		frame->Draw();
	}
	
	
	//std::cout << n.getVal() << std::endl;
	//std::cout << coincident_Events << std::endl;
	//std::cout << mean.getVal() << std::endl;
	
	return std::make_pair( lmean.getVal(), lmean.getAsymErrorHi());
}


std::pair<double, double> Roo_AutoGet::GaussianFit(std::string WhichPar, int Channel, RooDataSet * DataSet, RooRealVar *Var, std::pair<double, double> range, bool SaveImg)
{

	double VarRange_Mean = DataSet->mean(*Var);
	double VarRange_Min = DataSet->mean(*Var) - 10.0 * DataSet->sigma(*Var);
	double VarRange_Max = DataSet->mean(*Var) + 10.0 * DataSet->sigma(*Var);  
	
	RooRealVar gmean("gmean", "gaus mean", VarRange_Mean, VarRange_Min, VarRange_Max);
	RooRealVar gsigma("gsigma", "gaus sigma", DataSet->sigma(*Var), -5.0 * DataSet->sigma(*Var), 5.0 * DataSet->sigma(*Var));
	
	RooGaussian gaus("gaus", "gaus", *Var, gmean, gsigma);
	
	gaus.fitTo(*DataSet);
	
	RooPlot* frame = Var->frame( gmean.getVal() - 5.0 * gsigma.getVal(), gmean.getVal() + 5.0 * gsigma.getVal());
	
	/*std::cout<<"gaus"<<std::endl;
	std::cout<< mean.getVal() <<std::endl;
	std::cout<< sigma.getVal() <<std::endl;
	std::cout<<"gaus"<<std::endl;*/
	
	DataSet->plotOn (frame);
	DataSet->statOn(frame,Layout(0.45,0.90,0.8));
	gaus.plotOn(frame);
	gaus.paramOn(frame,Layout(0.45, 0.90, 0.5));

	TCanvas *oCanvas;
	if (SaveImg)
	{
		gROOT->SetBatch(true);
		oCanvas = new TCanvas("oCanvas");
		oCanvas->cd();
		
		frame->Draw();
		
		gSystem->ProcessEvents();
      	TImage *img = TImage::Create();
      	img->FromPad (oCanvas);
      	img->WriteImage( Form("%s_%s_ch%d_Roo.png", ifile_name.c_str(), WhichPar.c_str(), Channel) );
      	delete img;
      	delete frame;
      	delete oCanvas;
	}
	
	else 
	{
		frame->Draw();
	}
	
	return std::make_pair( gmean.getVal(), gmean.getAsymErrorHi());
}

std::pair<double, double> Roo_AutoGet::LxG (std::string WhichPar, int Channel, RooDataSet * DataSet, RooRealVar *Var, std::pair<double, double> range, bool SaveImg)
{

	double VarRange_Mean = DataSet->mean(*Var);
	double VarRange_Min = DataSet->mean(*Var) - 10.0 * DataSet->sigma(*Var);
	double VarRange_Max = DataSet->mean(*Var) + 10.0 * DataSet->sigma(*Var);  

	RooRealVar gmean("gmean", "gaus mean", VarRange_Mean, VarRange_Min, VarRange_Max);
	RooRealVar gsigma("gsigma", "gaus sigma", DataSet->sigma(*Var), -5.0 * DataSet->sigma(*Var), 5.0 * DataSet->sigma(*Var));
	
	RooRealVar lmean("lmean", "landau mean", VarRange_Mean, VarRange_Min, VarRange_Max);
	RooRealVar lsigma("lsigma", "landau sigma", DataSet->sigma(*Var), -5.0 * DataSet->sigma(*Var), 5.0 * DataSet->sigma(*Var));
	
	RooGaussian gaus("gaus", "gaus", *Var, gmean, gsigma);
	RooLandau landau("landau", "landau", *Var, lmean, lsigma);
	
	Var->setBins(10000, "cache");
	RooFFTConvPdf lxg ("lxg", "landau (x) gaus", *Var, landau, gaus);
	
	//==================================================
	
	lxg.fitTo(*DataSet);
	
	TF1 * f = lxg.asTF ( RooArgList(*Var) );
	double mpv = f->GetMaximumX();
	
	
	RooPlot* frame = Var->frame ( mpv - 1.0 * DataSet->mean(*Var), mpv + 2.5 * DataSet->mean(*Var));
	
	DataSet->plotOn (frame);
	DataSet->statOn(frame,Layout(0.45,0.90,0.8));
	lxg.plotOn(frame);
	lxg.paramOn(frame,Layout(0.45, 0.90, 0.5));

	TCanvas *oCanvas;
	if (SaveImg)
	{
		gROOT->SetBatch(true);
		oCanvas = new TCanvas("oCanvas");
		oCanvas->cd();
		
		frame->Draw();
		
		gSystem->ProcessEvents();
      	TImage *img = TImage::Create();
      	img->FromPad (oCanvas);
      	img->WriteImage( Form("%s_%s_ch%d_Roo_lxg.png", ifile_name.c_str(), WhichPar.c_str(), Channel) );
      	delete img;
      	delete frame;
      	delete oCanvas;
	}
	
	else 
	{
		frame->Draw();
	}
	
	
	return std::make_pair(mpv, lmean.getAsymErrorHi());
}


std::pair<double, double> Roo_AutoGet::CrystallBallFit(std::string WhichPar, int Channel, RooDataSet * DataSet, RooRealVar *Var, std::pair<double, double> range, bool SaveImg)
{

	double VarRange_Mean = DataSet->mean(*Var);
	double VarRange_Min = DataSet->mean(*Var) - 10.0 * DataSet->sigma(*Var);
	double VarRange_Max = DataSet->mean(*Var) + 10.0 * DataSet->sigma(*Var);  
	
	RooRealVar m("m", "m", VarRange_Mean, VarRange_Min, VarRange_Max);
	RooRealVar s("s", "s", DataSet->sigma(*Var), -5.0 * DataSet->sigma(*Var), 5.0 * DataSet->sigma(*Var));
	
	RooRealVar a("a", "a", -1.0);
	RooRealVar n("n", "n", 1.0);
	
	RooCBShape cb("cb", "cb", *Var, m, s, a, n);
	
	cb.fitTo(*DataSet);
	
	RooPlot* frame = Var->frame( DataSet->mean(*Var) - 5.0 * DataSet->sigma(*Var), DataSet->mean(*Var) + 5.0 * DataSet->sigma(*Var));
	
	/*std::cout<<"gaus"<<std::endl;
	std::cout<< mean.getVal() <<std::endl;
	std::cout<< sigma.getVal() <<std::endl;
	std::cout<<"gaus"<<std::endl;*/
	
	DataSet->plotOn (frame);
	//DataSet->statOn(frame,Layout(0.45,0.90,0.8));
	cb.plotOn(frame);
	//cb.paramOn(frame,Layout(0.45, 0.90, 0.5));

	TCanvas *oCanvas;
	if (SaveImg)
	{
		gROOT->SetBatch(true);
		oCanvas = new TCanvas("oCanvas");
		oCanvas->cd();
		
		frame->Draw();
		
		gSystem->ProcessEvents();
      	TImage *img = TImage::Create();
      	img->FromPad (oCanvas);
      	img->WriteImage( Form("%s_%s_ch%d_Roo_cb.png", ifile_name.c_str(), WhichPar.c_str(), Channel) );
      	delete img;
      	delete frame;
      	delete oCanvas;
	}
	
	else 
	{
		frame->Draw();
	}
	
	return std::make_pair( m.getVal(), m.getAsymErrorHi());
}



std::pair<double, double> Roo_AutoGet::RooBifurGaussianFit(std::string WhichPar, int Channel, RooDataSet * DataSet, RooRealVar *Var, std::pair<double, double> range, bool SaveImg)
{

	double VarRange_Mean = DataSet->mean(*Var);
	double VarRange_Min = DataSet->mean(*Var) - 10.0 * DataSet->sigma(*Var);
	double VarRange_Max = DataSet->mean(*Var) + 10.0 * DataSet->sigma(*Var);  
	
	RooRealVar bgmean("bgmean", "bifurcated gaus mean", VarRange_Mean, 0, VarRange_Max);
	RooRealVar blsigma("blsigma", "bifurcated gaus L sigma", DataSet->sigma(*Var), 0, 5.0 * DataSet->sigma(*Var));
	RooRealVar brsigma("brsigma", "bifurcated gaus R sigma", DataSet->sigma(*Var), 0, 5.0 * DataSet->sigma(*Var));
	
	RooBifurGauss rbgaus("rbgaus", "rbgaus", *Var, bgmean, blsigma, brsigma);
	
	rbgaus.fitTo(*DataSet);
	
	RooPlot* frame = Var->frame( bgmean.getVal() - 10.0 * abs(blsigma.getVal()), bgmean.getVal() + 5.0 * abs(brsigma.getVal()));
	
	/*std::cout<<"gaus"<<std::endl;
	std::cout<< mean.getVal() <<std::endl;
	std::cout<< sigma.getVal() <<std::endl;
	std::cout<<"gaus"<<std::endl;*/
	
	DataSet->plotOn (frame);
	DataSet->statOn(frame,Layout(0.45,0.90,0.8));
	rbgaus.plotOn(frame);
	rbgaus.paramOn(frame,Layout(0.45, 0.90, 0.5));

	TCanvas *oCanvas;
	if (SaveImg)
	{
		gROOT->SetBatch(true);
		oCanvas = new TCanvas("oCanvas");
		oCanvas->cd();
		
		frame->Draw();
		
		gSystem->ProcessEvents();
      	TImage *img = TImage::Create();
      	img->FromPad (oCanvas);
      	img->WriteImage( Form("%s_%s_ch%d_Roo_rbg.png", ifile_name.c_str(), WhichPar.c_str(), Channel) );
      	delete img;
      	delete frame;
      	delete oCanvas;
	}
	
	else 
	{
		frame->Draw();
	}
	
	return std::make_pair( bgmean.getVal(), bgmean.getAsymErrorHi());
}




std::pair<double, double> Roo_AutoGet::GaussAddCB(std::string WhichPar, int Channel, RooDataSet * DataSet, RooRealVar *Var, std::pair<double, double> range, bool SaveImg)
{

	double VarRange_Mean = DataSet->mean(*Var);
	double VarRange_Min = DataSet->mean(*Var) - 10.0 * DataSet->sigma(*Var);
	double VarRange_Max = DataSet->mean(*Var) + 10.0 * DataSet->sigma(*Var);  
	
	RooRealVar gmean("gmean", "gaus mean", VarRange_Mean, 0, VarRange_Max);
	RooRealVar gsigma("gsigma", "gaus sigma", DataSet->sigma(*Var), 0, 5.0 * DataSet->sigma(*Var));
	RooGaussian gaus("gaus", "gaus", *Var, gmean, gsigma);
	
	RooRealVar m("m", "m", VarRange_Mean, 0, VarRange_Max);
	RooRealVar s("s", "s", DataSet->sigma(*Var), -5.0 * DataSet->sigma(*Var), 5.0 * DataSet->sigma(*Var));
	RooRealVar a("a", "a", -1.0, -3.0, 0.0);
	RooRealVar n("n", "n", 1.0, 0.0, 3.0);
	RooCBShape cb("cb", "cb", *Var, m, s, a, n);
	
	RooRealVar coeff("coeff", "coeff", 0.1, 0.0, 1.0);
		
	RooAddPdf gausAddCB("gausAddCB", "gausAddCB", gaus, cb, coeff);
	
	
	
	gausAddCB.fitTo(*DataSet);
	
	RooPlot* frame = Var->frame( gmean.getVal() - 5.0 * abs(gsigma.getVal()), gmean.getVal() + 5.0 * abs(gsigma.getVal()));
	
	/*std::cout<<"gaus"<<std::endl;
	std::cout<< mean.getVal() <<std::endl;
	std::cout<< sigma.getVal() <<std::endl;
	std::cout<<"gaus"<<std::endl;*/
	
	DataSet->plotOn (frame);
	//DataSet->statOn(frame,Layout(0.45,0.90,0.8));
	gausAddCB.plotOn(frame);
	//gausAddCB.paramOn(frame,Layout(0.45, 0.90, 0.5));

	TCanvas *oCanvas;
	if (SaveImg)
	{
		gROOT->SetBatch(true);
		oCanvas = new TCanvas("oCanvas");
		oCanvas->cd();
		
		frame->Draw();
		
		gSystem->ProcessEvents();
      	TImage *img = TImage::Create();
      	img->FromPad (oCanvas);
      	img->WriteImage( Form("%s_%s_ch%d_Roo_gauss+CB.png", ifile_name.c_str(), WhichPar.c_str(), Channel) );
      	delete img;
      	delete frame;
      	delete oCanvas;
	}
	
	else 
	{
		frame->Draw();
	}
	
	return std::make_pair( gmean.getVal(), gmean.getAsymErrorHi());
}




std::pair<double, double> Roo_AutoGet::GaussAddGauss(std::string WhichPar, int Channel, RooDataSet * DataSet, RooRealVar *Var, std::pair<double, double> range, bool SaveImg)
{

	double VarRange_Mean = DataSet->mean(*Var);
	double VarRange_Min = DataSet->mean(*Var) - 10.0 * DataSet->sigma(*Var);
	double VarRange_Max = DataSet->mean(*Var) + 10.0 * DataSet->sigma(*Var);  
	
	RooRealVar gain_gmean("gain_gmean", "gain gaus mean", VarRange_Mean, 0, VarRange_Max);
	RooRealVar gain_gsigma("gain_gsigma", "gain gaus sigma", DataSet->sigma(*Var), 0, 5.0 * DataSet->sigma(*Var));
	RooGaussian gain_gaus("gain_gaus", "gain gaus", *Var, gain_gmean, gain_gsigma);
	
	RooRealVar pin_gmean("pin_gmean", "pin gaus mean", VarRange_Mean, 0, VarRange_Max);
	RooRealVar pin_gsigma("pin_gsigma", "pin gaus sigma", DataSet->sigma(*Var), 0, 5.0 * DataSet->sigma(*Var));
	RooGaussian pin_gaus("pin_gaus", "pin gaus", *Var, pin_gmean, pin_gsigma);

	
	RooRealVar coeff1("coeff1", "coeff1", 0.1, 0.0, 5.0);
	RooRealVar coeff2("coeff2", "coeff2", 0.1, 0.0, 5.0);
	
	RooArgList coeff_set(coeff1, coeff2);
		
	RooAddPdf gausAddgaus("gausAddgaus", "gausAddgaus", RooArgList(pin_gaus, gain_gaus), coeff_set);
	
	
	
	gausAddgaus.fitTo(*DataSet);
	
	RooPlot* frame = Var->frame( gain_gmean.getVal() - 5.0 * abs(gain_gsigma.getVal()), gain_gmean.getVal() + 20.0 * abs(gain_gsigma.getVal()));
	
	/*std::cout<<"gaus"<<std::endl;
	std::cout<< mean.getVal() <<std::endl;
	std::cout<< sigma.getVal() <<std::endl;
	std::cout<<"gaus"<<std::endl;*/
	
	DataSet->plotOn (frame);
	DataSet->statOn(frame,Layout(0.45,0.90,0.8));
	gausAddgaus.plotOn(frame);
	//gausAddgaus.paramOn(frame,Layout(0.45, 0.90, 0.5));

	TCanvas *oCanvas;
	if (SaveImg)
	{
		gROOT->SetBatch(true);
		oCanvas = new TCanvas("oCanvas");
		oCanvas->cd();
		
		frame->Draw();
		
		gSystem->ProcessEvents();
      	TImage *img = TImage::Create();
      	img->FromPad (oCanvas);
      	img->WriteImage( Form("%s_%s_ch%d_Roo_gauss+gauss.png", ifile_name.c_str(), WhichPar.c_str(), Channel) );
      	delete img;
      	delete frame;
      	delete oCanvas;
	}
	
	else 
	{
		frame->Draw();
	}
	
	return std::make_pair( gain_gmean.getVal(), gain_gmean.getAsymErrorHi());
}

//==========================================================================

std::pair<double, double> Roo_AutoGet::GaussAddGauss_splot(std::string WhichPar, int Channel, RooDataSet * DataSet, RooDataSet *DataSet2, RooRealVar *Var, RooRealVar *Var2, std::pair<double, double> range, std::pair<double, double> range2, bool SaveImg)
{
	RooRealVar localVar(*Var);
	RooRealVar localVar2(*Var2);
	
	localVar.Print();
	localVar2.Print();
	
	RooDataSet localDataSet(*DataSet);
	RooDataSet localDataSet2 (*DataSet2 );
	RooDataSet *cdata = new RooDataSet("cdata", "cdata", RooArgSet(localVar), Import(localDataSet) );
	
	std::cout <<"merggggggggggggggggggggggggggg\n";
	cdata->Print();
	cdata->merge(&localDataSet);
	cdata->Print();
	cdata->merge(&localDataSet2);
	cdata->Print();
	
	localDataSet.Print();
	localDataSet2.Print();
	
	//RooDataSet *cdata = new RooDataSet( "cdata", "cdata", RooArgSet(localVar, localVar2), Import(localDataSet), Import(localDataSet2) );
	
	//cdata->Print();
	
	double VarRange_Mean = abs( localDataSet.mean(localVar) );
	double VarRange_Min = localDataSet.mean(localVar) - 10.0 * abs( localDataSet.sigma(localVar) );
	double VarRange_Max = localDataSet.mean(localVar) + 10.0 * abs( localDataSet.sigma(localVar) );
	
	double VarRange_Mean2 = abs( localDataSet2.mean(localVar2) );
	double VarRange_Min2 = localDataSet2.mean(localVar2) - 10.0 * abs( localDataSet2.sigma(localVar2) );
	double VarRange_Max2 = localDataSet2.mean(localVar2) + 10.0 * abs( localDataSet2.sigma(localVar2) );
	
	//RooRealVar gain_lmean("gain_lmean", "gain_landau mean", VarRange_Mean2, VarRange_Min2, VarRange_Max2);
	//RooRealVar gain_lsigma("gain_lsigma", "gain_landau sigma", localDataSet2.sigma(localVar2), -5.0 * localDataSet2.sigma(localVar2), 5.0 * localDataSet2.sigma(localVar2));
	//RooLandau gain_landau("gain_landau", "gain_landau", localVar2, gain_lmean, gain_lsigma);
	
	//RooRealVar pin_lmean("pin_lmean", "pin_landau mean", VarRange_Mean2, VarRange_Min2, VarRange_Max2);
	//RooRealVar pin_lsigma("pin_lsigma", "pin_landau sigma", localDataSet2.sigma(localVar2), -5.0 * localDataSet2.sigma(localVar2), 5.0 * localDataSet2.sigma(localVar2));
	//RooLandau pin_landau("pin_landau", "pin_landau", localVar2, pin_lmean, pin_lsigma);   
	
	RooRealVar gain_gmean("gain_gmean", "gain gaus mean", VarRange_Mean, 0, VarRange_Max);
	RooRealVar gain_gsigma("gain_gsigma", "gain gaus sigma", localDataSet.sigma(localVar), 0, 5.0 * localDataSet.sigma(localVar) );
	RooGaussian gain_gaus("gain_gaus", "gain gaus", localVar, gain_gmean, gain_gsigma);
	
	//RooRealVar ccc("c","c", -1, -5.0, 5.0);
	//RooExponential expo("expo","expo", localVar, ccc);
	
	
	/*RooRealVar bgmean("bgmean", "bifurcated gaus mean", 400, 300, 1500);
	RooRealVar blsigma("blsigma", "bifurcated gaus L sigma", localDataSet.sigma(localVar), 10, 5.0 * localDataSet.sigma(localVar));
	RooRealVar brsigma("brsigma", "bifurcated gaus R sigma", localDataSet.sigma(localVar), 10, 5.0 * localDataSet.sigma(localVar));
	RooBifurGauss rbgaus("rbgaus", "rbgaus", localVar, bgmean, blsigma, brsigma);*/
	
	
	
	/*
	RooRealVar pin_gmean("pin_gmean", "pin gaus mean", VarRange_Mean, 0, VarRange_Max);
	RooRealVar pin_gsigma("pin_gsigma", "pin gaus sigma", localDataSet.sigma(localVar), 0, 5.0 * localDataSet.sigma(localVar) );
	RooGaussian pin_gaus("pin_gaus", "pin gaus", localVar, pin_gmean, pin_gsigma);*/
	
	
	RooRealVar m("m", "m", VarRange_Mean, 10, VarRange_Max);
	RooRealVar s("s", "s", localDataSet.sigma(localVar), -5.0 * localDataSet.sigma(localVar), 5.0 * localDataSet.sigma(localVar));
	RooRealVar a("a", "a", -1.0, -10.0, 10.0);
	RooRealVar n("n", "n", 1.0, 0.0, 10.0);
	RooCBShape cb("cb", "cb", localVar, m, s, a, n);

	//RooProdPdf gainModel("gainModel", "gainModel", RooArgSet(gain_gaus, gain_landau) ); 
	
	//RooProdPdf pinModel("pinModel", "pinModel", RooArgSet(pin_gaus, pin_landau) );
	
	//RooProdPdf riseModel("riseModel", "riseModel", RooArgSet(gain_gaus, pin_gaus) );

	
	RooRealVar* pinCoeff = new RooRealVar("pinCoeff", "pinCoeff", 500, 0., 1000.0);
	RooRealVar* gainCoeff = new RooRealVar("gainCoeff", "gainCoeff", 500, 0., 1000.0);
	
	pinCoeff->Print();
	gainCoeff->Print();
	
	RooRealVar* pinCoeff_temp = new RooRealVar("pinCoeff_temp", "pinCoeff_temp", 500, 0., 1000.0);
	RooRealVar* gainCoeff_temp = new RooRealVar("gainCoeff_temp", "gainCoeff_temp", 500, 0., 1000.0);
	
	//RooArgList coeff_set(gainCoeff, pinCoeff);
		
	//RooAddPdf gain_pin_model("gain_pin_model", "gain_pin_model", RooArgList(gain_gaus, pin_gaus), RooArgList(*gainCoeff, *pinCoeff));
	RooAddPdf gain_pin_model("gain_pin_model", "gain_pin_model", RooArgList(gain_gaus, cb), RooArgList(*gainCoeff, *pinCoeff));
	//RooAddPdf gain_pin_model("gain_pin_model", "gain_pin_model", RooArgList(gain_gaus, rbgaus), RooArgList(*gainCoeff, *pinCoeff));
	//RooAddPdf gain_pin_model("gain_pin_model", "gain_pin_model", RooArgList(gain_gaus, expo), RooArgList(*gainCoeff, *pinCoeff));
	
	//RooAddPdf gain_pin_model_temp("gain_pin_model_temp", "gain_pin_model_temp", RooArgList(gain_gaus, pin_gaus), RooArgList(*gainCoeff_temp, *pinCoeff_temp));
	
	RooAddPdf gain_pin_model_temp("gain_pin_model_temp", "gain_pin_model_temp", RooArgList(gain_gaus, cb), RooArgList(*gainCoeff_temp, *pinCoeff_temp));
	//RooAddPdf gain_pin_model_temp("gain_pin_model_temp", "gain_pin_model_temp", RooArgList(gain_gaus, rbgaus), RooArgList(*gainCoeff_temp, *pinCoeff_temp));
	//RooAddPdf gain_pin_model_temp("gain_pin_model_temp", "gain_pin_model_temp", RooArgList(gain_gaus, expo), RooArgList(*gainCoeff_temp, *pinCoeff_temp));
	
	//gain_pin_model_temp.fitTo(localDataSet);//, Extended() );
	//gain_pin_model_temp.fitTo(*cdata);//, localVar.GetName());
	std::cout << "XDDDDDDDDDDDDDDDDDDD  " << gainCoeff_temp->getVal() << std::endl;
	
	
	RooWorkspace *ws = new RooWorkspace("myWS");
	
	ws->import(gain_pin_model);
	
	RooAbsPdf *sModel = ws->pdf("gain_pin_model");
	//RooAbsPdf *ssModel = ws->pdf("gain_pin_model");
	
	
	//cdata->Print();
	
	//sModel->fitTo(*cdata);//, Extended(true), SumW2Error(kTRUE) );
	gain_pin_model.fitTo(*cdata, Extended() );
	//sModel->fitTo(localDataSet, Extended() );
	
	std::cout<<"AAAAAAAAAAAAAAAAAA\n";
	
	//pin_lmean.setConstant();
	//pin_lsigma.setConstant();
	//gain_lmean.setConstant();
	//gain_lsigma.setConstant();
	gain_gmean.Print();
	std::cout<<"BBBBBBBBBBBBBBBBBBB\n";
	gain_gmean.setConstant();
	gain_gsigma.setConstant();
	//blsigma.setConstant();
	//brsigma.setConstant();
	//bgmean.setConstant();
	//pin_gmean.setConstant();
	//pin_gsigma.setConstant();
	
	pinCoeff->Print();
	gainCoeff->Print();
	//pinCoeff->setConstant();
	//gainCoeff->setConstant();
	//sModel->updateCoefficients()
	
	//auto ii = gain_pin_model.coefList()
	
	
	std::cout << "helleleellelelelelel" << pinCoeff->getValV() << std::endl;
	//pinCoeff->setVal(gainCoeff_temp->getVal());
	//gainCoeff->setVal(pinCoeff_temp->getVal());
	
	//pinCoeff->setVal(pinCoeff_temp->getVal());
	//gainCoeff->setVal(gainCoeff_temp->getVal());
	
	//pinCoeff->setVal(200);
	//gainCoeff->setVal(500);
	
	pinCoeff->Print();
	gainCoeff->Print();
	
	
	
	
	//RooStats::SPlot *sData = new RooStats::SPlot("sData", "sData", *cdata, sModel, RooArgList(*gainCoeff, *pinCoeff));//, RooArgSet(), kTRUE, kTRUE );
	RooStats::SPlot *sData = new RooStats::SPlot("sData", "sData", *cdata, &gain_pin_model, RooArgList(*gainCoeff, *pinCoeff));//, RooArgSet(), kTRUE, kTRUE );
	
	pinCoeff->Print();
	gainCoeff->Print();
	gain_gmean.Print();
	std::cout<<"CCCCCCCCCCCCC\n";
	
	//ws->import(cdata, Rename("data_w"));
	
	//RooDataSet* data_w = (RooDataSet*) ws->data("data_w");
	
	//RooAbsPdf *ssModel = ws->pdf("gain_pin_model");
	
	//data_w->Print();
	
	//RooAbsPdf *ssModel = ws->pdf("gain_pin_model");
	
	//sModel->fitTo(*data_w, Extended() );
	
	//sModel->selfNormalized();
	//sModel->fitTo( *data_w, Extended() );
	//sModel->fitTo(localDataSet, Extended() );
	
	
	//ssModel->fitTo( *data_w, Extended() );
	sModel->fitTo( *cdata);//, Extended(true), SumW2Error(kTRUE) );
	//gain_pin_model.fitTo(*cdata, Extended() );
	//sModel->fitTo(localDataSet, Extended() );
		
	//gausAddgaus.fitTo(*DataSet);
	
	//RooDataSet *data_pin = new RooDataSet( data_w->GetName(), data_w->GetTitle(), data_w, *data_w->get(), 0, "pinCoeff_sw" );
	
	//RooDataSet *data_gain = new RooDataSet( data_w->GetName(), data_w->GetTitle(), data_w, *data_w->get(), 0, "gainCoeff_sw" );
	
	RooDataSet *data_pin = new RooDataSet( cdata->GetName(), cdata->GetTitle(), cdata, *cdata->get(), 0, "pinCoeff_sw" );
	
	data_pin->Print();
	
	RooDataSet *data_gain = new RooDataSet( cdata->GetName(), cdata->GetTitle(), cdata, *cdata->get(), 0, "gainCoeff_sw" );
	
	data_gain->Print();
	
	//RooPlot* frame2 = localVar.frame( gain_gmean.getVal() - 5.0 * abs(gain_gsigma.getVal()), gain_gmean.getVal() + 5.0 * abs(gain_gsigma.getVal()));
	RooPlot* frame2 = localVar.frame(0,1500,200);
	//RooPlot* frame = Var2->frame( -1000, 1000);
	//RooPlot* frame = localVar2.frame();
	RooPlot* frame = localVar2.frame(0, 50, 100);
	
	//RooPlot* frame = localVar.frame();
	
	/*std::cout<<"gaus"<<std::endl;
	std::cout<< mean.getVal() <<std::endl;
	std::cout<< sigma.getVal() <<std::endl;
	std::cout<<"gaus"<<std::endl;*/
	
	//localDataSet.plotOn (frame2 );
	//data_w->plotOn(frame2 );
	//data_w->plotOn(frame );
	//localDataSet.statOn(frame,Layout(0.45,0.90,0.8));
	//sModel->plotOn(frame2 );
	cdata->plotOn(frame2);
	//localDataSet.plotOn(frame2);
	//sModel->plotOn(frame2 );
	//sModel->plotOn(frame2 );
	gain_pin_model.plotOn(frame2 );
	gain_pin_model.plotOn(frame2, Components(gain_gaus ), LineColor(kGreen) );
	gain_pin_model.plotOn(frame2, Components(cb ), LineColor(kRed) );
	//cdata->plotOn(frame2);
	//, Normalization(1.0,RooAbsReal::RelativeExpected));
	//gausAddgaus.plotOn(frame);
	//gausAddgaus.paramOn(frame,Layout(0.45, 0.90, 0.5));
	
	
	//localDataSet2.plotOn(frame, Normalization(999,RooAbsReal::NumEvent));
	//cdata.plotOn(frame);

	data_gain->plotOn(frame, MarkerColor(kGreen), DataError(RooAbsData::SumW2) );
	data_pin->plotOn(frame, MarkerColor(kRed), DataError(RooAbsData::SumW2) );
	

	TCanvas *oCanvas;
	if (SaveImg)
	{
		gROOT->SetBatch(true);
		oCanvas = new TCanvas("oCanvas","", 1000, 1000);
		
		oCanvas->Divide(1,2);
		
		oCanvas->cd(1);
		frame->Draw();
		
		oCanvas->cd(2);
		sModel->paramOn(frame2,Layout(0.45,0.90,0.8));
		frame2->Draw();
		
		gSystem->ProcessEvents();
      	TImage *img = TImage::Create();
      	img->FromPad (oCanvas);
      	img->WriteImage( Form("%s_%s_ch%d_Roo_gauss+gauss_splot.png", ifile_name.c_str(), WhichPar.c_str(), Channel) );
      	delete img;
      	delete frame;
      	delete frame2;
      	delete oCanvas;
	}
	
	else 
	{
		frame->Draw();
	}
	
	return std::make_pair( gain_gmean.getVal(), gain_gmean.getAsymErrorHi());
}






