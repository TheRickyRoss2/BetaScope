#ifndef average_pulses_v87
#define average_pulses_v87

void average(const char *path, std::string file_name ,double tmin2, double tmax2, double pmin2, double pmax2, double tmin3, double tmax3, double pmin3, double pmax3, TString hist_name, int psec_pt, int color, int normalized = 1, int channal = 2, int DUT = 2, int Trigger = 3, double range = 30000)
	{
		TFile *hfile1 = TFile::Open(path);
		TFile* new_file;
		
		if(normalized) new_file = new TFile( Form("normalized_%s", file_name.c_str()), "UPDATE", "8");
		else new_file = new TFile( file_name.c_str(), "UPDATE", "8");
		
		TProfile *w2p;
	
		int bins;
		
		const char* t_cut2 = Form("tmax%d[0] - cfd%d[20]", DUT, Trigger);
		const char* t_cut3 = Form("tmax%d[0] - cfd%d[20]", Trigger, Trigger);
		
		const char* p_cut2 = Form("pmax%d[0]", DUT);
		const char* p_cut3 = Form("pmax%d[0]", Trigger);
		
		
		TCut cut = Form(" %s > %f && %s < %f && %s > %f && %s < %f && "
		
						" %s > %f && %s < %f && %s > %f && %s < %f ",
						  
						  t_cut2, tmin2, t_cut2, tmax2, p_cut2, pmin2, p_cut2, pmax2,
						  
						  t_cut3, tmin3, t_cut3, tmax3, p_cut3, pmin3, p_cut3, pmax3 );
						  
		std::cout << std::endl;
		//std::cout << "your cuts " << std::endl;
		//std::cout << cut << std::endl;
		std::cout << std::endl;
		
		if(hfile1)
			{
				TTree *wfm = (TTree*) hfile1->Get("wfm");

				std::cout << "  ----------------"<< std::endl;
				std::cout << "  file loaded  "<< std::endl;
				std::cout << "  processing data  "<< std::endl;
				std::cout << "  ----------------"<< std::endl;
				
				if(channal)
					{
					//----------------------------pulse profile--------------------------------------------
						
						bins = 2*range/psec_pt;
						
						TH2F *ch2 = new TH2F("wave2", "wave2", bins, -range, range, 2000 , -500, 500);
						
						wfm->Project("wave2", Form("-w%d:t%d-cfd%d[20]", channal, channal, Trigger), cut);
						
						std::cout << Form("  channal %d loaded.", channal) << std::endl;
						
						w2p = ch2->ProfileX(hist_name,0, 2000,"");
						
						double pulse_max2 = w2p->GetBinContent(w2p->GetMaximumBin());

						if(normalized){w2p->Scale(1/pulse_max2);}
						
					//--------------------------------------------------------------------------------------
						
					//-----------getting the time at 20% of the pulse max ( pmax = 1 for normalized )-------
						
						double pmax_at_20 = 0.2*w2p->GetBinContent(w2p->GetMaximumBin());
						
						double v_top = 0; //1 bin above pmax_at_20.
						int v_top_index;
						
						double v_bot = w2p->GetBinContent(w2p->GetMaximumBin()); //1 bin below.
						int v_bot_index;
						
						for(int i = 0; i < w2p->GetMaximumBin(); i++ )
						{	
							if( v_bot > pmax_at_20)
							{
								v_bot = w2p->GetBinContent(w2p->GetMaximumBin()-i);
								v_bot_index = w2p->GetMaximumBin()-i;
							}
							if( v_bot <= pmax_at_20)
							{
								v_top = w2p->GetBinContent(w2p->GetMaximumBin()-i+1);
								v_top_index = w2p->GetMaximumBin()-i + 1;
								break;
							}
						}
						
						double time_at_20;
						
						time_at_20 = (psec_pt*(v_top_index-v_bot_index)) / (v_top-v_bot) * ( pmax_at_20 - v_bot) + (-range + v_bot_index*psec_pt); //linear interprelate
						
						//std::cout << "hello " << time_at_20 << std::endl;
						//std::cout << "hello2 " << v_bot_index << std::endl;
						//std::cout << "hello3 " << w2p->GetBinContent(w2p->GetMaximumBin()) << std::endl;
					
					//--------------------------------------------------------------------------------------
					
					//------------------------------------reset and refill----------------------------------
						w2p->Reset();
						
						wfm->Project("wave2", Form("-w%d:t%d-cfd%d[20]-%f",channal, channal, Trigger, time_at_20) , cut);
						
						w2p = ch2->ProfileX(hist_name,0, 2000,"");
						
						if(normalized){w2p->Scale(1/pulse_max2);}
						
						
						std::cout << Form("  channal %d is ready.", channal) << std::endl;
					
					//---------------------------------------------------------------------------------------
					
					//-----------------------------csv of the pulse------------------------------------------
					
						if(1) 
						{
							std::ofstream file_o;
							if(normalized)
							{
								file_o.open( hist_name += "_norm_pulse2.csv");
							}
							else
							{
								file_o.open( hist_name += "_pulse2.csv");
							}
							file_o << "time" << "," << "voltage" << "," << "Error" << std::endl;
							for(int bin_index = 0; bin_index < bins; bin_index++)
							{
								file_o << (-range + bin_index*psec_pt) << "," << w2p->GetBinContent(bin_index) << "," << w2p->GetBinError(bin_index) << std::endl;
							}
							file_o.close();
							std::cout << "  ch2 csv is ready. " << std::endl;
						}
					
					//---------------------------------------------------------------------------------------
						
					}
					
				/*if(channel3)
					{		
						bin3 = 2*range3/psec_pt3;
						
						TH2F *ch3 = new TH2F("wave3", "wave3", bin3, -range3, range3, 2000, -500, 500);
						
						wfm->Project("wave3","-w3:t3-cfd3[20]",cut);
						
						std::cout << "  channel 3 loaded."<< std::endl;
						
						w3p = ch3->ProfileX("trigger", 0, 2000,"");
						
						double pulse_max3 = w3p->GetBinContent(w3p->GetMaximumBin());

						if(normalized){w3p->Scale(1/pulse_max3);}
						
						std::cout << "  channel 3 is ready."<< std::endl;
					}*/
				
				std::cout << "  ----------------"<< std::endl;
				std::cout << "  processing plots"<< std::endl;
				
				w2p->SetTitle("Average pulse Shapes");
				w2p->SetLineColor(color);
				w2p->SetMarkerColor(color);
				w2p->SetLineWidth(3);
				w2p->SetMarkerSize(0);
				w2p->SetStats(0);				
				w2p->GetXaxis()->SetTitle("Time (ps)");
				
				if(normalized) w2p->GetYaxis()->SetTitle("Normalized Amplitude");
				else w2p->GetYaxis()->SetTitle("Amplitude (mV)");
				
				/*if(channel3)
				{
					w3p->SetLineColor(color+5);
					w3p->SetMarkerColor(color+5);
					w3p->SetLineWidth(2);
					w3p->SetMarkerSize(0);
					w3p->SetStats(0);
				}*/
				
				
				new_file->Write("");
				new_file->Close();
				
				std::cout << "  --------finished--------"<< std::endl;
				
			}
				
	}
	

void average(const char *path, std::string file_name ,double tmin, double tmax, double pmin, double pmax, TString hist_name, int psec_pt, int color, int normalized = 1, int channel = 2, double range = 30000)
{
		TFile *hfile1 = TFile::Open(path);
		TFile* new_file;
		
		if(normalized) new_file = new TFile( Form("raw_normalized_%s", file_name.c_str()), "UPDATE", "8");
		else new_file = new TFile(  Form("raw_%s", file_name.c_str()), "UPDATE", "8");
		
		TProfile *w2p;
	
		int bins;
		
		const char* t_cut = Form("tmax%d[0]", channel);
		
		const char* p_cut = Form("pmax%d[0]", channel);

		
		
		TCut cut = Form(" %s > %f && %s < %f && %s > %f && %s < %f && ",
						  
						  t_cut, tmin, t_cut, tmax, p_cut, pmin, p_cut, pmax);
						  
		std::cout << std::endl;
		//std::cout << "your cuts " << std::endl;
		//std::cout << cut << std::endl;
		std::cout << std::endl;
		
		if(hfile1)
			{
				TTree *wfm = (TTree*) hfile1->Get("wfm");

				std::cout << "  ----------------"<< std::endl;
				std::cout << "  file loaded  "<< std::endl;
				std::cout << "  processing data  "<< std::endl;
				std::cout << "  ----------------"<< std::endl;
				
				if(channel)
					{
					//----------------------------pulse profile--------------------------------------------
						
						bins = 2*range/psec_pt;
						
						TH2F *ch2 = new TH2F("wave2", "wave2", bins, -range, range, 2000 , -500, 500);
						
						wfm->Project("wave2", Form("-w%d:t%d", channel, channel), cut);
						
						std::cout << Form("  channal %d loaded.", channel) << std::endl;
						
						w2p = ch2->ProfileX(hist_name,0, 2000,"");
						
						double pulse_max2 = w2p->GetBinContent(w2p->GetMaximumBin());

						if(normalized){w2p->Scale(1/pulse_max2);}
						
					//--------------------------------------------------------------------------------------
						
					//-----------getting the time at 20% of the pulse max ( pmax = 1 for normalized )-------
						
						double pmax_at_20 = 0.2*w2p->GetBinContent(w2p->GetMaximumBin());
						
						double v_top = 0; //1 bin above pmax_at_20.
						int v_top_index;
						
						double v_bot = w2p->GetBinContent(w2p->GetMaximumBin()); //1 bin below.
						int v_bot_index;
						
						for(int i = 0; i < w2p->GetMaximumBin(); i++ )
						{	
							if( v_bot > pmax_at_20)
							{
								v_bot = w2p->GetBinContent(w2p->GetMaximumBin()-i);
								v_bot_index = w2p->GetMaximumBin()-i;
							}
							if( v_bot <= pmax_at_20)
							{
								v_top = w2p->GetBinContent(w2p->GetMaximumBin()-i+1);
								v_top_index = w2p->GetMaximumBin()-i + 1;
								break;
							}
						}
						
						double time_at_20;
						
						time_at_20 = (psec_pt*(v_top_index-v_bot_index)) / (v_top-v_bot) * ( pmax_at_20 - v_bot) + (-range + v_bot_index*psec_pt); //linear interprelate
						
						//std::cout << "hello " << time_at_20 << std::endl;
						//std::cout << "hello2 " << v_bot_index << std::endl;
						//std::cout << "hello3 " << w2p->GetBinContent(w2p->GetMaximumBin()) << std::endl;
					
					//--------------------------------------------------------------------------------------
					
					//------------------------------------reset and refill----------------------------------
						w2p->Reset();
						
						wfm->Project("wave2", Form("-w%d:t%d-%f", channel, channel, time_at_20) , cut);
						
						w2p = ch2->ProfileX(hist_name,0, 2000,"");
						
						if(normalized){w2p->Scale(1/pulse_max2);}
						
						
						std::cout << Form("  channal %d is ready.", channel) << std::endl;
					
					//---------------------------------------------------------------------------------------
					
					//-----------------------------csv of the pulse------------------------------------------
					
						if(1) 
						{
							std::ofstream file_o;
							if(normalized)
							{
								file_o.open( hist_name += "_raw_norm_pulse2.csv");
							}
							else
							{
								file_o.open( hist_name += "_raw_pulse2.csv");
							}
							file_o << "time" << "," << "voltage" << "," << "Error" << std::endl;
							for(int bin_index = 0; bin_index < bins; bin_index++)
							{
								file_o << (-range + bin_index*psec_pt) << "," << w2p->GetBinContent(bin_index) << "," << w2p->GetBinError(bin_index) << std::endl;
							}
							file_o.close();
							std::cout << "  ch2 csv is ready. " << std::endl;
						}
					
					//---------------------------------------------------------------------------------------
						
					}
					
				/*if(channel3)
					{		
						bin3 = 2*range3/psec_pt3;
						
						TH2F *ch3 = new TH2F("wave3", "wave3", bin3, -range3, range3, 2000, -500, 500);
						
						wfm->Project("wave3","-w3:t3-cfd3[20]",cut);
						
						std::cout << "  channel 3 loaded."<< std::endl;
						
						w3p = ch3->ProfileX("trigger", 0, 2000,"");
						
						double pulse_max3 = w3p->GetBinContent(w3p->GetMaximumBin());

						if(normalized){w3p->Scale(1/pulse_max3);}
						
						std::cout << "  channel 3 is ready."<< std::endl;
					}*/
				
				std::cout << "  ----------------"<< std::endl;
				std::cout << "  processing plots"<< std::endl;
				
				w2p->SetTitle("Average pulse Shapes");
				w2p->SetLineColor(color);
				w2p->SetMarkerColor(color);
				w2p->SetLineWidth(3);
				w2p->SetMarkerSize(0);
				w2p->SetStats(0);				
				w2p->GetXaxis()->SetTitle("Time (ps)");
				
				if(normalized) w2p->GetYaxis()->SetTitle("Normalized Amplitude");
				else w2p->GetYaxis()->SetTitle("Amplitude (mV)");
				
				/*if(channel3)
				{
					w3p->SetLineColor(color+5);
					w3p->SetMarkerColor(color+5);
					w3p->SetLineWidth(2);
					w3p->SetMarkerSize(0);
					w3p->SetStats(0);
				}*/
				
				
				new_file->Write("");
				new_file->Close();
				
				std::cout << "  --------finished--------"<< std::endl;
				
			}
				
	}	
	
#endif
