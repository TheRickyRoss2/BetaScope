#include "../hpp/lib.h"
#include "../hpp/getResolution.h"

void Res_VS_CFD(const char *in_file, double Dut_tmin, double Dut_tmax, double Dut_pmin, double Dut_pmax, double Trig_tmin, double Trig_tmax, double Trig_pmin, double Trig_pmax, std::string SensorName = " ", std::string Bias = " ", std::string Fluence = " ", std::string Temperature = " ", int totalStep = 100, double toRealCFD = 1.0)
{
	//std::ofstream ofile;
	bool header = true;
	std::string ifile = in_file;
	
	gROOT->SetBatch(true);
	
	for(int scan = 0; scan < totalStep; scan++)
	{
		std::pair<double,double> Res; 
		
		if( totalStep == 100 )
		{
			Res = res(in_file, Dut_tmin, Dut_tmax, Dut_pmin, Dut_pmax, Trig_tmin, Trig_tmax, Trig_pmin, Trig_pmax, true, "", 2, 3, scan);
		}
		else if( int(totalStep * toRealCFD) == 100 )
		{
			Res = fineRes( in_file, Dut_tmin, Dut_tmax, Dut_pmin, Dut_pmax, Trig_tmin, Trig_tmax, Trig_pmin, Trig_pmax, true, "", 2, 3, scan);
		}
		else
		{
			std::cout << "totalStep * toRealCFD = " << int(totalStep * toRealCFD) << std::endl;
			return void();
		}
		
		if (true) //saving file as txt
      	{
      		std::ofstream ofile;
      		ofile.open ( Form("%s_res_scan.txt", ifile.substr(10,ifile.length() - 5).c_str()), std::ios::app);
      	 	
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
      			ofile << "CFD Fraction" << " " << "Sigma [ps]" << " " << "Error [ps]" << std::endl;
      			header = false;
      		}
      		
      		ofile << toRealCFD * scan << " " << Res.first << " " << Res.second << std::endl;
      		
      		ofile.close();
      	}
	}
	
	gROOT->SetBatch(false);
}

void CreateDir(const char* DirName)
{
	int dir_check;
	dir_check = mkdir(DirName, ACCESSPERMS);
	if(dir_check == 0){printf("Directory is created. \n");}
	else{printf("Directory already exists! Previous data will be replaced...\n");}
}

void Create_CFDScan_Runlist(std::string sendToDirName)
{
	//Reading the Runlist_cut and recreate another runlist for CFD Scan.
	std::string Line;
	std::ifstream ReadList;
	ReadList.open("Runlist_cut.txt");
	if(ReadList.is_open()) printf("Runlist is opened. \n");
	else printf("No Runlist! \n");
	
	std::vector<std::string> ifile;
	
	while(std::getline(ReadList, Line))
	{
		ifile.push_back (Line);
	}
	
	//Creating runlist for CFD scan.
	
	std::ofstream olist;
	olist.open("CFD_Runlist.txt");
	
	//header
	olist << "Sensor " << std::endl;
	olist << "Fluence " << std::endl;
	olist << "Temperature " << std::endl;
	
	for (unsigned int i = 0; i < ifile.size(); i++)
	{
		olist << "Bias ? " << ifile.at(i) << std::endl;
	}
	
	//moving them to the target directory.
	char NameBuffer[1024];
	sprintf (NameBuffer, "mv CFD_Runlist.txt %s/", sendToDirName.c_str());
	system(NameBuffer);	
}

void cdTo(std::string Destination)
{
	char GoTo[1024];
	snprintf (GoTo, sizeof(GoTo), "cd %s", Destination.c_str());
	std::cout << "going to : " << GoTo << std::endl;
	chdir ("cd ave_pulse/");
	system ("pwd");
}

int ProcessingCFDScan()
{
	//reading the CFD Runlist..
	printf("Start processing...\n");
	
	std::string Line;
	std::ifstream ReadList;
	ReadList.open("CFD_Runlist.txt");
	
	if(ReadList.is_open())printf("Runlist is opened \n");
	else {printf("Cannot open the Runlist.\n");return -1;}
	
	std::string placeholder;
	std::string SensorName;
	std::string Fluence;
	std::string Temperature;
	std::string Bias;
	std::string iFile;
	double dut_TminC = 0.0;
	double dut_TmaxC = 0.0;
	double dut_PminC = 0.0;
	double dut_PmaxC = 0.0;
	double trig_TminC = 0.0;
	double trig_TmaxC = 0.0;
	double trig_PminC = 0.0;
	double trig_PmaxC = 0.0;
	
	//storing the header information
	ReadList >> placeholder >> SensorName;
	ReadList >> placeholder >> Fluence;
	ReadList >> placeholder >> Temperature;
	
	while(ReadList >> placeholder >> Bias >> iFile >> dut_TminC >> dut_TmaxC >> dut_PminC >> dut_PmaxC >> trig_TminC >> trig_TmaxC >> trig_PminC >> trig_PmaxC)
	{
		printf("Working on %s \n", Bias.c_str());
	
		char ifile_location[1024];
		sprintf (ifile_location, " ../%s", iFile.c_str());
		Res_VS_CFD (ifile_location, dut_TminC, dut_TmaxC, dut_PminC, dut_PmaxC, trig_TminC, trig_TmaxC, trig_PminC, trig_PmaxC, SensorName, Bias, Fluence, Temperature, 500, 0.2);
		
		printf("finished. \n");
	}
	
	return 0;
}
	
	
	
	
	
	
int main (int argc, char **argv)
{
	if (argc == 2)
	{
		CreateDir(argv[1]);
		Create_CFDScan_Runlist(argv[1]);
	}
	else if (argc == 1)
	{
		ProcessingCFDScan();
	}
	else
	{
		printf("Error! Check parameters \n"); return -1;
	}
	
	return 0;
}
