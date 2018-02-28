#include"../hpp/edgeRes.h"

void thresholdResScan(const char *in_file, double Dut_tmin, double Dut_tmax, double Dut_pmin, double Dut_pmax, double Trig_tmin, double Trig_tmax, double Trig_pmin, double Trig_pmax, std::string SensorName, std::string Bias, std::string Fluence, std::string Temperature )
{
	//std::ofstream ofile;
	bool header = true;
	std::string ifile = in_file;
	
	gROOT->SetBatch(true);
	
	for(int scan = 0; scan < 20; scan++)
	{
		std::pair<double,double> Res = resAtThreshold(in_file, Dut_tmin, Dut_tmax, scan, Dut_pmax, Trig_tmin, Trig_tmax, Trig_pmin, Trig_pmax, scan, Form( "&&pmax2[0]>%d", scan ) );
		
		if (true) //saving file as txt
      	{
      		std::ofstream ofile;
      		ofile.open ( Form("%s_res_threhold_scan.txt", ifile.substr(10,ifile.length() - 5).c_str()), std::ios::app);
      	 	
      		if(header)
      		{						
				if(ofile.is_open())
				{
					printf("Output file is OK. \n");
				}
				  	
				else 
				{
				  	printf("Something wrong with the output file. \n");
				  	return void();
				}
      						      		
      			ofile << "Sensor: " << SensorName << std::endl;
      			ofile << "Bias: " << Bias << std::endl;
      			ofile << "Fluence: " << Fluence << std::endl;
      			ofile << "Temperature: " << Temperature << std::endl;
      			ofile << "Threshold" << " " << "Sigma [ps]" << " " << "Error [ps]" << std::endl;
      			header = false;
      		}
      		
      		ofile << scan << " " << Res.first << " " << Res.second << std::endl;
      		
      		ofile.close();
      	}
	}
	
	gROOT->SetBatch(false);
}
