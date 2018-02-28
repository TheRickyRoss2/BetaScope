void Roo_AutoGet::Result_Generator (std::string flag1, std::string flag2)
{
	printf("\n");
	printf("Start processing...\n");
	
	gROOT->SetBatch(true); //reinforce the batch mode.
	
	printf("\n");
	printf("Saving plots... \n");
		
	std::pair<double, double> pmax_dut_o = Roo_AutoGet::LandauFit ("Pmax", Dut, RooPmax_Set[Dut], RooPmax[Dut], Pmax_range[Dut]);  
	
	std::pair<double, double> pmax_trigger_o = Roo_AutoGet::LandauFit ("Pmax", Trig, RooPmax_Set[Trig], RooPmax[Trig], Pmax_range[Trig]);
	
	
	
	
	
	std::pair<double, double> charge_dut_o = Roo_AutoGet::LandauFit ("pulse_area", Dut, RooPulseArea_Set[Dut], RooPulseArea[Dut], PulseArea_range[Dut]); 
	
	std::pair<double, double> charge_trigger_o = Roo_AutoGet::LandauFit ("pulse_area", Trig, RooPulseArea_Set[Trig], RooPulseArea[Trig], PulseArea_range[Trig]); 
	
	
	
	
	
	std::pair<double, double> rms_dut_o = Roo_AutoGet::GaussianFit ("RMS", Dut, RooRms_Set[Dut], RooRms[Dut], Rms_range[Dut]); 
	
	std::pair<double, double> rms_trigger_o = Roo_AutoGet::GaussianFit ("RMS", Trig, RooRms_Set[Trig], RooRms[Trig], Rms_range[Trig]); 
	
	
	
	std::pair<double, double> dvdt_dut_o = Roo_AutoGet::LandauFit ("Dvdt", Dut, RooDvdt_Set[Dut], RooDvdt[Dut], Dvdt_range[Dut]);
	
	std::pair<double, double> dvdt_trigger_o = Roo_AutoGet::LandauFit ("Dvdt", Trig, RooDvdt_Set[Trig], RooDvdt[Trig], Dvdt_range[Trig]);
	
	
	
		
	std::pair<double, double> rise1090_dut_o = Roo_AutoGet::GaussianFit ("Rise Time", Dut, RooRiseTime_Set[Dut], RooRiseTime[Dut], RiseTime_range[Dut]); 
	
	std::pair<double, double> rise1090_trigger_o = Roo_AutoGet::GaussianFit ("Rise Time", Trig, RooRiseTime_Set[Trig], RooRiseTime[Trig], RiseTime_range[Trig]);
	
	std::pair<double, double> bifur_rise1090_dut_o = Roo_AutoGet::RooBifurGaussianFit ("Bifur Rise Time", Dut, RooRiseTime_Set[Dut], RooRiseTime[Dut], RiseTime_range[Dut]);
	
	std::pair<double, double> gausCB_rise1090_dut_o = Roo_AutoGet::GaussAddCB ("gauss+CB Rise Time", Dut, RooRiseTime_Set[Dut], RooRiseTime[Dut], RiseTime_range[Dut]);
	
	std::pair<double, double> gausAddgaus_rise1090_dut_o = Roo_AutoGet::GaussAddGauss ("gauss+gauss Rise Time", Dut, RooRiseTime_Set[Dut], RooRiseTime[Dut], RiseTime_range[Dut]);
	
	std::pair<double, double> splot_rise1090_dut_o = Roo_AutoGet::GaussAddGauss_splot ("gauss+gauss splot Rise Time", Dut, RooRiseTime_Set[Dut], RooPmax_Set[Dut], RooRiseTime[Dut] ,RooPmax[Dut], RiseTime_range[Dut], Pmax_range[Dut]);

	
	
	std::pair<double,double> FWHM_dut_o = Roo_AutoGet::GaussianFit ("FWHM", Dut, RooFWHM_Set[Dut], RooFWHM[Dut], FWHM_range[Dut]); 
	
	std::pair<double,double> FWHM_trigger_o = Roo_AutoGet::GaussianFit ("FWHM", Trig, RooFWHM_Set[Trig], RooFWHM[Trig], FWHM_range[Trig]);
	
	
	
	
	std::pair<double, double> pmax_dut_o_lxg, pmax_trigger_o_lxg;
	std::pair<double, double> charge_dut_o_lxg, charge_trigger_o_lxg;
	std::pair<double, double> dvdt_dut_o_lxg, dvdt_trigger_o_lxg;
	
	bool lxg_fit = false;
	bool cb_fit  = false;
	
	if (flag1.compare("-lxg") == 0 || flag2.compare("-lxg") == 0)
	{
		lxg_fit = true;
	}
	
	if (flag1.compare("-cb") == 0 || flag2.compare("-cb") == 0)
	{
		cb_fit = true;
	}
	
	
	if ( lxg_fit )
	{
		pmax_dut_o_lxg = Roo_AutoGet::LxG ("Pmax", Dut, RooPmax_Set[Dut], RooPmax[Dut], Pmax_range[Dut]);  
		
		pmax_trigger_o_lxg = Roo_AutoGet::LxG ("Pmax", Trig, RooPmax_Set[Trig], RooPmax[Trig], Pmax_range[Trig]); 
		
		
		charge_dut_o_lxg = Roo_AutoGet::LxG ("pulse_area", Dut, RooPulseArea_Set[Dut], RooPulseArea[Dut], PulseArea_range[Dut]); 
		
		charge_trigger_o_lxg = Roo_AutoGet::LxG ("pulse_area", Trig, RooPulseArea_Set[Trig], RooPulseArea[Trig], PulseArea_range[Trig]); 
		
		
		dvdt_dut_o_lxg = Roo_AutoGet::LxG ("Dvdt", Dut, RooDvdt_Set[Dut], RooDvdt[Dut], Dvdt_range[Dut]);
		
		dvdt_trigger_o_lxg = Roo_AutoGet::LxG ("Dvdt", Trig, RooDvdt_Set[Trig], RooDvdt[Trig], Dvdt_range[Trig]);
	}
	else
	{
		pmax_dut_o_lxg = std::make_pair (-1.0, -1.0); 		
		pmax_trigger_o_lxg = pmax_dut_o_lxg = std::make_pair (-1.0, -1.0);
		charge_dut_o_lxg = pmax_dut_o_lxg = std::make_pair (-1.0, -1.0); 
		charge_trigger_o_lxg = pmax_dut_o_lxg = std::make_pair (-1.0, -1.0);
		dvdt_dut_o_lxg = pmax_dut_o_lxg = std::make_pair (-1.0, -1.0);
		dvdt_trigger_o_lxg = pmax_dut_o_lxg = std::make_pair (-1.0, -1.0);
	}
	
	
	std::pair<double, double> rise1090_dut_o_cb, rise1090_trigger_o_cb;
	
	if ( cb_fit )
	{
		rise1090_dut_o_cb = Roo_AutoGet::CrystallBallFit ("Rise Time", Dut, RooRiseTime_Set[Dut], RooRiseTime[Dut], RiseTime_range[Dut]);
		
		rise1090_trigger_o_cb = Roo_AutoGet::CrystallBallFit ("Rise Time", Trig, RooRiseTime_Set[Trig], RooRiseTime[Trig], RiseTime_range[Trig]);
	}
	else
	{
		rise1090_dut_o_cb = std::make_pair (-1.0, -1.0);		
		rise1090_trigger_o_cb = std::make_pair (-1.0, -1.0);
	}
	
	
	
	
	std::ofstream ofile;
	
	std::string prefix = ifile_name;
	
	ofile.open ( "_RooResults.txt", std::ios::app);
	
	ofile << "-------------" << Form("%s_ch%d", prefix.c_str(), Dut+1) <<"------------------"<< std::endl;
	
	
	ofile << "charge2" << ",,,,,,," << "pmax2" << "," << "rms2" << ",,,,," << "rise2_1090" << "," << "dvdt2" << ",,,,,,,,," << " FWHM2" << ",,,,,,,,," << "charge2_lxg" << ",,,,,,,,," << "pmax2_lxg" << ",,,,,,,,," << "dvdt2_lxg" << std::endl;
	ofile << charge_dut_o.first <<", "<< charge_dut_o.second << ",,,,,,,"
			  << pmax_dut_o.first <<", "<< pmax_dut_o.second << ","  
			  << rms_dut_o.first <<", "<< rms_dut_o.second << ",,,,,"
 			  << rise1090_dut_o.first <<", "<< rise1090_dut_o.second << ","
			  << dvdt_dut_o.first <<", "<< dvdt_dut_o.second << ",,,,,,,,,"
			  << FWHM_dut_o.first <<", "<<  FWHM_dut_o.second << ",,,,,,,,,"
			  << charge_dut_o_lxg.first <<", "<< charge_dut_o_lxg.second << ",,,,,,,"
			  << pmax_dut_o_lxg.first <<", "<< pmax_dut_o_lxg.second << ","
			  << dvdt_dut_o_lxg.first <<", "<< dvdt_dut_o_lxg.second << std::endl;

	ofile << "-------------" << Form("%s_ch%d", prefix.c_str(), Trig+1) <<"------------------"<< std::endl;
	
	ofile << "charge3" << ",,,,,,," << "pmax3" << "," << "rms3" << ",,,,," << "rise3_1090" << "," << "dvdt3" << ",,,,,,,,," << " FWHM3" << ",,,,,,,,," << "charge3_lxg" << ",,,,,,,,," << "pmax3_lxg" << ",,,,,,,,," << "dvdt3_lxg" << std::endl;
	ofile << charge_trigger_o.first <<", "<< charge_trigger_o.second << ",,,,,,,"
			  << pmax_trigger_o.first <<", "<< pmax_trigger_o.second << ","  
			  << rms_trigger_o.first <<", "<< rms_trigger_o.second << ",,,,,"
 			  << rise1090_trigger_o.first <<", "<< rise1090_trigger_o.second << ","
			  << dvdt_trigger_o.first <<", "<< dvdt_trigger_o.second << ",,,,,,,,,"
			  << FWHM_trigger_o.first <<", "<<  FWHM_trigger_o.second << ",,,,,,,,,"
			  << charge_trigger_o_lxg.first <<", "<< charge_trigger_o_lxg.second << ",,,,,,,"
			  << pmax_trigger_o_lxg.first <<", "<< pmax_trigger_o_lxg.second << ","
			  << dvdt_trigger_o_lxg.first <<", "<< dvdt_trigger_o_lxg.second << std::endl;
	
	ofile << std::endl;
	ofile << std::endl;	
	
	printf("\n");
	printf("plots are saved... \n");
	printf("\n");
	gROOT->SetBatch(false);
}
