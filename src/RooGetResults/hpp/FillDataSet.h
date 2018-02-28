//=====filling Roodataset==============

void Roo_AutoGet::FillDataSet (double Dut_tmin, double Dut_tmax, double Dut_pmin, double Dut_pmax, double Trig_tmin, double Trig_tmax, double Trig_pmin, double Trig_pmax)
{
	printf(" \n");
	printf("preparing filling process...\n");
	
	std::vector<double> local_w[4];
	std::vector<double> local_t[4];
	std::vector<double> local_Pmax[4];
	std::vector<double> local_Tmax[4];
	std::vector<double> local_Rms[4];
	std::vector<double> local_RiseTime[4];
	std::vector<double> local_PulseArea[4];
	std::vector<double> local_CFD[4];
	std::vector<double> local_Dvdt[4];
	std::vector<double> local_FWHM[4];
	
	printf(" \n");
	printf("selecting events base on cuts..\n");
	
	for(int i = 0; i < Entries; i++)
	{
		iTree->GetEntry(i);
		
		if	( (Tmax[Dut]->at(0)-CFD[Trig]->at(20)) > Dut_tmin &&
			  (Tmax[Dut]->at(0)-CFD[Trig]->at(20)) < Dut_tmax &&
			  Pmax[Dut]->at(0) > Dut_pmin &&
			  Pmax[Dut]->at(0) < Dut_pmax &&
			  
			  (Tmax[Trig]->at(0)-CFD[Trig]->at(20)) > Trig_tmin &&
			  (Tmax[Trig]->at(0)-CFD[Trig]->at(20)) < Trig_tmax &&
			  Pmax[Trig]->at(0) > Trig_pmin &&
			  Pmax[Trig]->at(0) < Trig_pmax )
		{
			local_Pmax[Dut].push_back ( Pmax[Dut]->at(0) );
			local_Tmax[Dut].push_back ( Tmax[Dut]->at(0) );
			local_Rms[Dut].push_back ( Rms[Dut]->at(0) );
			local_RiseTime[Dut].push_back ( RiseTime[Dut]->at(0) );
			local_PulseArea[Dut].push_back ( PulseArea[Dut]->at(0)/1.0E-15 );
			local_CFD[Dut].push_back ( CFD[Dut]->at(20) );
			local_Dvdt[Dut].push_back ( Dvdt[Dut]->at(20) );
			local_FWHM[Dut].push_back ( CFD_Fall[Dut]->at(50) - CFD[Dut]->at(50) );
			
			local_Pmax[Trig].push_back ( Pmax[Trig]->at(0) );
			local_Tmax[Trig].push_back ( Tmax[Trig]->at(0) );
			local_Rms[Trig].push_back ( Rms[Trig]->at(0) );
			local_RiseTime[Trig].push_back ( RiseTime[Trig]->at(0) );
			local_PulseArea[Trig].push_back ( PulseArea[Trig]->at(0)/1.0E-15 );
			local_CFD[Trig].push_back ( CFD[Trig]->at(20) );
			local_Dvdt[Trig].push_back ( Dvdt[Trig]->at(20) );
			local_FWHM[Trig].push_back ( CFD_Fall[Trig]->at(50) - CFD[Trig]->at(50) );
		}
		
		else
		{}
		
	}
	
	//===filling RooDataSet=================
	
	printf(" \n");
	printf("fillling data sets...\n");
	
	for (unsigned int i = 0; i < ch.size(); i++)
	{
		if (ch.at(i))
		{
			double rangeHoldermin, rangeHoldermax, middle;
			
			rangeHoldermin = *std::min_element(local_Pmax[ch.at(i)-1].begin(), local_Pmax[ch.at(i)-1].end());
			rangeHoldermax = *std::max_element(local_Pmax[ch.at(i)-1].begin(), local_Pmax[ch.at(i)-1].end());
			
			middle = abs( (rangeHoldermax + rangeHoldermin)/2.0 );
			
			RooPmax[ch.at(i)-1] = new RooRealVar( Form("pmax%d", ch.at(i)), Form("pmax%d", ch.at(i)), rangeHoldermin - VarFactor * middle, rangeHoldermax + VarFactor * middle );
			
			RooPmax_Set[ch.at(i)-1] = new RooDataSet( Form("pmax_set%d", ch.at(i)), Form("pmax_set%d", ch.at(i)), RooArgSet(*RooPmax[ch.at(i)-1]) );
			
			Pmax_range[ch.at(i)-1] = std::make_pair(rangeHoldermin, rangeHoldermax);
			
			
			
			rangeHoldermin = *std::min_element(local_Rms[ch.at(i)-1].begin(), local_Rms[ch.at(i)-1].end());
			rangeHoldermax = *std::max_element(local_Rms[ch.at(i)-1].begin(), local_Rms[ch.at(i)-1].end());
			
			middle = abs( (rangeHoldermax + rangeHoldermin)/2.0 );
			
			RooRms[ch.at(i)-1] = new RooRealVar( Form("rms%d", ch.at(i)), Form("rms%d", ch.at(i)), rangeHoldermin - VarFactor * middle, rangeHoldermax + VarFactor * middle);
			
			RooRms_Set[ch.at(i)-1] = new RooDataSet( Form("rms_set%d", ch.at(i)), Form("rms_set%d", ch.at(i)), RooArgSet( *RooRms[ch.at(i)-1] ) );
			
			Rms_range[ch.at(i)-1] = std::make_pair(rangeHoldermin, rangeHoldermax);
			
			
			
			rangeHoldermin = *std::min_element(local_Tmax[ch.at(i)-1].begin(), local_Tmax[ch.at(i)-1].end());
			rangeHoldermax = *std::max_element(local_Tmax[ch.at(i)-1].begin(), local_Tmax[ch.at(i)-1].end());
			
			middle = abs( (rangeHoldermax + rangeHoldermin)/2.0 );
			
			RooTmax[ch.at(i)-1] = new RooRealVar( Form("tmax%d", ch.at(i)), Form("tmax%d", ch.at(i)), rangeHoldermin - VarFactor * middle, rangeHoldermax + VarFactor * middle);
			
			RooTmax_Set[ch.at(i)-1] = new RooDataSet( Form("tmax_set%d", ch.at(i)), Form("tmax_set%d", ch.at(i)), RooArgSet( *RooTmax[ch.at(i)-1] ) );
			
			Tmax_range[ch.at(i)-1] = std::make_pair(rangeHoldermin, rangeHoldermax);
			
			
			rangeHoldermin = *std::min_element(local_RiseTime[ch.at(i)-1].begin(), local_RiseTime[ch.at(i)-1].end());
			rangeHoldermax = *std::max_element(local_RiseTime[ch.at(i)-1].begin(), local_RiseTime[ch.at(i)-1].end());	
			
			middle = abs( (rangeHoldermax + rangeHoldermin)/2.0 );	

			RooRiseTime[ch.at(i)-1] = new RooRealVar( Form("risetime%d", ch.at(i)), Form("risetime%d", ch.at(i)), rangeHoldermin - VarFactor * middle, rangeHoldermax + VarFactor * middle);
			
			RooRiseTime_Set[ch.at(i)-1] = new RooDataSet( Form("risetime_set%d", ch.at(i)), Form("risetime_set%d", ch.at(i)), RooArgSet( *RooRiseTime[ch.at(i)-1] ) );
			
			RiseTime_range[ch.at(i)-1] = std::make_pair(rangeHoldermin, rangeHoldermax);
			
			
			rangeHoldermin = *std::min_element(local_PulseArea[ch.at(i)-1].begin(), local_PulseArea[ch.at(i)-1].end());
			rangeHoldermax = *std::max_element(local_PulseArea[ch.at(i)-1].begin(), local_PulseArea[ch.at(i)-1].end());
			
			middle = abs( (rangeHoldermax + rangeHoldermin)/2.0 );
			
			RooPulseArea[ch.at(i)-1] = new RooRealVar( Form("pulseArea%d", ch.at(i)), Form("pulseArea%d", ch.at(i)), rangeHoldermin - VarFactor * middle, rangeHoldermax + VarFactor * middle);
			
			RooPulseArea_Set[ch.at(i)-1] = new RooDataSet( Form("pulasArea_set%d", ch.at(i)), Form("pulasArea_set%d", ch.at(i)), RooArgSet( *RooPulseArea[ch.at(i)-1] ) );
			
			PulseArea_range[ch.at(i)-1] = std::make_pair(rangeHoldermin, rangeHoldermax);
			
			
			
			rangeHoldermin = *std::min_element(local_CFD[ch.at(i)-1].begin(), local_CFD[ch.at(i)-1].end());
			rangeHoldermax = *std::max_element(local_CFD[ch.at(i)-1].begin(), local_CFD[ch.at(i)-1].end());
			
			middle = abs( (rangeHoldermax + rangeHoldermin)/2.0 );
			
			RooCFD[ch.at(i)-1] = new RooRealVar( Form("CFD[20]%d", ch.at(i)), Form("CFD[20]%d", ch.at(i)), rangeHoldermin - VarFactor * middle, rangeHoldermax + VarFactor * middle);
			
			RooCFD_Set[ch.at(i)-1] = new RooDataSet( Form("cfd_set%d", ch.at(i)), Form("cfd_set%d", ch.at(i)), RooArgSet( *RooCFD[ch.at(i)-1] ) );
			
			CFD_range[ch.at(i)-1] = std::make_pair(rangeHoldermin, rangeHoldermax);
			
			std::cout << " middle " <<  middle << std::endl;
			std::cout << "rangeHoldermin - VarFactor * middle " <<  rangeHoldermin - VarFactor * middle << std::endl;
			std::cout << "rangeHoldermix + VarFactor * middle " <<  rangeHoldermax + VarFactor * middle << std::endl;
			
			
			
			rangeHoldermin = *std::min_element(local_Dvdt[ch.at(i)-1].begin(), local_Dvdt[ch.at(i)-1].end());
			rangeHoldermax = *std::max_element(local_Dvdt[ch.at(i)-1].begin(), local_Dvdt[ch.at(i)-1].end());
			
			middle = abs( (rangeHoldermax + rangeHoldermin)/2.0 );
			
			RooDvdt[ch.at(i)-1] = new RooRealVar( Form("Dvdt[20]%d", ch.at(i)), Form("Dvdt[20]%d", ch.at(i)), rangeHoldermin - VarFactor * middle, rangeHoldermax + VarFactor * middle);
			
			
			RooDvdt_Set[ch.at(i)-1] = new RooDataSet( Form("dvdt_set%d", ch.at(i)), Form("dvdt_set%d", ch.at(i)), RooArgSet( *RooDvdt[ch.at(i)-1] ) );
			
			Dvdt_range[ch.at(i)-1] = std::make_pair(rangeHoldermin, rangeHoldermax);
			
			
			rangeHoldermin = *std::min_element(local_FWHM[ch.at(i)-1].begin(), local_FWHM[ch.at(i)-1].end());
			rangeHoldermax = *std::max_element(local_FWHM[ch.at(i)-1].begin(), local_FWHM[ch.at(i)-1].end());
			
			middle = abs( (rangeHoldermax + rangeHoldermin)/2.0 );
			
			RooFWHM[ch.at(i)-1] = new RooRealVar( Form("FWHM%d", ch.at(i)), Form("FWHM%d", ch.at(i)), rangeHoldermin - VarFactor * middle, rangeHoldermax + VarFactor * middle);
			
			RooFWHM_Set[ch.at(i)-1] = new RooDataSet( Form("FWHM_set%d", ch.at(i)), Form("FWHM_set%d", ch.at(i)), RooArgSet( *RooFWHM[ch.at(i)-1] ) );
			
			FWHM_range[ch.at(i)-1] = std::make_pair(rangeHoldermin, rangeHoldermax);
			
			
			//====filling===========
			for (unsigned int k = 0; k < local_Pmax[Dut].size(); k++)
			{
				if(k==0)
				{
					std::cout << "entry: " << local_Pmax[Dut].size()<< std::endl;
					coincident_Events = local_Pmax[Dut].size();
				}
				
				RooPmax[ch.at(i)-1]->setVal(local_Pmax[ch.at(i)-1].at(k));
				RooRms[ch.at(i)-1]->setVal(local_Rms[ch.at(i)-1].at(k));
				RooRiseTime[ch.at(i)-1]->setVal(local_RiseTime[ch.at(i)-1].at(k));
				RooTmax[ch.at(i)-1]->setVal(local_Tmax[ch.at(i)-1].at(k));
				RooDvdt[ch.at(i)-1]->setVal(local_Dvdt[ch.at(i)-1].at(k));
				RooCFD[ch.at(i)-1]->setVal(local_CFD[ch.at(i)-1].at(k));
				RooPulseArea[ch.at(i)-1]->setVal(local_PulseArea[ch.at(i)-1].at(k));
				RooFWHM[ch.at(i)-1]->setVal(local_FWHM[ch.at(i)-1].at(k));
				
				RooPmax_Set[ch.at(i)-1]->add(RooArgSet(*RooPmax[ch.at(i)-1]));
				RooRms_Set[ch.at(i)-1]->add(RooArgSet(*RooRms[ch.at(i)-1]));
				RooRiseTime_Set[ch.at(i)-1]->add(RooArgSet(*RooRiseTime[ch.at(i)-1]));
				RooTmax_Set[ch.at(i)-1]->add(RooArgSet(*RooTmax[ch.at(i)-1]));
				RooDvdt_Set[ch.at(i)-1]->add(RooArgSet(*RooDvdt[ch.at(i)-1]));
				RooCFD_Set[ch.at(i)-1]->add(RooArgSet(*RooCFD[ch.at(i)-1]));
				RooPulseArea_Set[ch.at(i)-1]->add(RooArgSet(*RooPulseArea[ch.at(i)-1]));
				RooFWHM_Set[ch.at(i)-1]->add(RooArgSet(*RooFWHM[ch.at(i)-1]));
			}	
				//RooPmax_Set[ch.at(i)-1]->Print("v");cout << endl;
				//RooRms_Set[ch.at(i)-1]->Print("v");cout << endl;
				//RooRiseTime_Set[ch.at(i)-1]->Print("v");cout << endl;
				//RooTmax_Set[ch.at(i)-1]->Print("v");cout << endl;
				//RooDvdt_Set[ch.at(i)-1]->Print("v");cout << endl;
				//RooCFD_Set[ch.at(i)-1]->Print("v");cout << endl;
				//RooPulseArea_Set[ch.at(i)-1]->Print("v");cout << endl;
				//RooFWHM_Set[ch.at(i)-1]->Print("v");cout << endl;
				
		}
	}
	
	printf(" \n");
	printf("Data sets are ready...\n");
	//===============================


}

