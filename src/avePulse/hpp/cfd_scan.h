#ifndef cfd_scan_v629
#define cfd_scan_v629

//-------c++----------------//
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <unistd.h>
//------ROOT----------------//
#include <TH2.h>
#include <TH1.h>
#include <TStyle.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <TTree.h>
#include <iomanip>
#include <TFile.h>
#include "TF1.h"
#include <TLeaf.h>
#include <TROOT.h>
#include <TCanvas.h>
#include <TCut.h>
#include <TSystem.h>
#include <TImage.h>
#include <TProfile.h>
#include <TLegend.h>

/*std::pair<double,double> cfd_min( std::pair<double,double> sigma[], int minimum_cfd)
{
	   	if(minimum_cfd)
   	{
   		double mini = sigma[0].first;
   		int percentage = 0;
   		for(int index = 0; index < 100; index++)
   		{
   			if(mini >= sigma[index].first)
   			{
   				mini = sigma[index].first;
   				percentage = index;
   			}
   		}
   		return std::make_pair(mini,percentage);  		
   	}

}*/ //uncomment this to get minimum timing resolution. also uncomment out (1)hello.



void cfd(const char* path, double Dut_tmin, double Dut_tmax, double Dut_pmin, double Dut_pmax, double Trig_tmin, double Trig_tmax, double Trig_pmin, double Trig_pmax, int csv_out=0, std::string sensor_name = "", std::string bias = "", std::string fluence ="", std::string temp ="", double gain = 0, std:: string file_user_name = "cfd_scan", int minimum_cfd = 0, int graph=0)
{

	TFile *LoadFile = TFile::Open(path);
	TTree* LoadTree = (TTree*) LoadFile->Get("wfm");

	const char* Dut_tcut = Form("tmax%d[0] - cfd%d[20]", 2, 3);
	const char* Trig_tcut = Form("tmax%d[0] - cfd%d[20]", 3, 3);
		
	const char* Dut_pcut = Form("pmax%d[0]", 2);
	const char* Trig_pcut = Form("pmax%d[0]", 3);
			
		
	TCut cuts = Form(" %s > %f && %s < %f && "
				
					" %s > %f && %s < %f && "
		
					" %s > %f && %s < %f && "
						
					" %s > %f && %s < %f  ",
						  
					  Dut_tcut, Dut_tmin, Dut_tcut, Dut_tmax, 
						  
					  Dut_pcut, Dut_pmin, Dut_pcut, Dut_pmax,
						  
					  Trig_tcut, Trig_tmin, Trig_tcut, Trig_tmax, 
						  
					  Trig_pcut, Trig_pmin, Trig_pcut, Trig_pmax );

	int image_count = 0;
	
	int NumOfBin = 0;
	
	double Mean = 0.0;
	double Sigma = 0.0;
	
	std::pair<double,double> SigmaArray[100]; //sigma and error holder
	
	for(int i  = 0; i<100; i++)
	{
			TH1D *cfd_hist_prep = new TH1D ( "cfd_hist_prep", "cfd_hist_prep", 100, 1, 1);
			 
			LoadTree->Project ( "cfd_hist_prep", Form ("cfd%d[%d]-cfd%d[20]", 2, i, 3), cuts );  
			
			Mean = cfd_hist_prep->GetMean(1);
			Sigma = cfd_hist_prep->GetStdDev(1);
			NumOfBin = sqrt ( cfd_hist_prep -> GetEntries() ); // using sqrt of total entries;
			
			delete cfd_hist_prep;
			
			//looping process to adjust the fitting windows
			
			for ( int k = 0; k < 7; k++ )
			{
				TH1D *cfd_hist_loop = new TH1D ( "cfd_hist_loop", "cfd_hist_loop", NumOfBin, Mean - (1.0 + 0.5 * k ) * Sigma, Mean + (1.0 + 0.5 * k ) * Sigma );
			
				LoadTree -> Project ( "cfd_hist_loop", Form ( "cfd%d[%d]-cfd%d[20]", 2, i, 3 ), cuts ); 
			
				TF1* cfd_fit_loop = new TF1 ( "cfd_fit_loop", "gaus" ); //histogram for setting up the range
				cfd_fit_loop -> SetParameter (1, Mean);
				cfd_fit_loop -> SetParameter (2, Sigma);
				
				cfd_hist_loop -> Fit ( cfd_fit_loop, "Q 0"); 
						
				Mean = cfd_fit_loop -> GetParameter(1);
				Sigma = cfd_fit_loop -> GetParameter(2);
				
				delete cfd_fit_loop;
				delete cfd_hist_loop;
			}
			
			// "slow down" process.
					
			TH1D *cfd_hist_post = new TH1D ( "cfd_hist_post", "cfd_hist_post", NumOfBin, Mean - 5.0 * Sigma, Mean + 5.0 * Sigma ); //new histogram with range,bin num from hist_pre
			
			LoadTree -> Project ( "cfd_hist_post", Form ( "cfd%d[%d]-cfd%d[20]", 2, i, 3), cuts);
			 
			TF1* cfd_fit_post = new TF1("cfd_fit_post","gaus");
			cfd_fit_post -> SetParameter (1, Mean);
			cfd_fit_post -> SetParameter (2, Sigma);
			
			gROOT->ProcessLine("gROOT->SetBatch()"); //no display when saving plots
			TCanvas *c = new TCanvas;
			c->cd();
				
			cfd_hist_post -> Fit ( cfd_fit_post, "Q 0" );
		 	
		 	SigmaArray[i] = std::make_pair ( cfd_fit_post -> GetParameter(2), cfd_fit_post -> GetParError(2) );
		
			if(graph) // save every histogram into png.
				{
					std::ostringstream image_os;
					TString img_st;
					image_os << std::to_string(image_count) << "cfd222.png";
					img_st = image_os.str();
					gSystem->ProcessEvents();
					TImage *img = TImage::Create();
					img->FromPad(c);
					img->WriteImage(img_st.Data());
   					image_count ++;
   					image_os.str("");
   				}	
			 		
			std::cout<< i <<" : " << cfd_fit_post -> GetParameter(2) << " +- " << cfd_fit_post -> GetParError(2) << std::endl;
			
			delete cfd_hist_post;
			delete cfd_fit_post;
			delete c;
	}
		
	if(csv_out) //save a csv file
   	{
   			std::ofstream file_o;
   			std::string file_in_name = path;
   			std::string file_o_name = file_user_name;
   			//file_o.open( file_o_name );
   			
   			if(std::ifstream(file_o_name + ".csv"))
   				{
   					std::cout << std::endl;
   					std::cout << "file with defaut name is already here, please give another name:" << std::endl;
   					std::cin >> file_o_name;
   					file_o.close();
   				}
   					
   			file_o.open( file_o_name+".csv");
   			
   			file_o<< sensor_name << std:: endl;
   			file_o<< "bias" << "," << bias << std::endl;
   			file_o<< "fluence" << "," << fluence << std::endl;
   			file_o<< "temperature" << "," <<temp << std::endl;
   			file_o<< "gain" << "," <<gain << std::endl;		
   			file_o<< "CFD Fraction"<< "," <<"Sigma (ps)" << "," << "Error" << std::endl;
   			for(int in = 0; in<100; in++)
   				{
   					file_o << in << "," << SigmaArray[in].first << "," << SigmaArray[in].second << std::endl;
   				} 
   			file_o.close();
   			std::cout << "finished" << std::endl;
   	}
   		/*std::pair<double,double> mm;
   		mm = cfd_min(sigma, minimum_cfd);
   		std::cout << "tstst " << mm.first << " hskfhksjh " << mm.second << std::endl;*/ //(1)hello

}


#endif







