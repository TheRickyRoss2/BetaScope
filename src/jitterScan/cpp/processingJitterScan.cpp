#include "../hpp/jitterScan.h"

int processingJitterScan()
{
	//reading the CFD Runlist..
	printf("Start processing...\n");
	
	std::string Line;
	std::ifstream ReadList;
	ReadList.open("Jitter_Runlist.txt");
	
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
		
		jitterScan(ifile_location, dut_TminC, dut_TmaxC, dut_PminC, dut_PmaxC, trig_TminC, trig_TmaxC, trig_PminC, trig_PmaxC, SensorName, Bias, Fluence, Temperature);
		
		printf("finished. \n");
	}
	
	return 0;
}
