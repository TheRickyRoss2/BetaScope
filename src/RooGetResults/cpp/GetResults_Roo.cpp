#include "../hpp/lib.h"
#include "../hpp/AutoClass.h"
#include "../hpp/SetBranch.h"
#include "../hpp/FillDataSet.h"
#include "../hpp/Fit.h"
#include "../hpp/ResultGenerator.h"

void GetResults(std::string Runlist_wCut, std::string lxg_flag = "", std::string cb_flag = "")
{	
	//reading the runlist....
	
	std::string Line;
	std::ifstream ReadList;
	ReadList.open(Runlist_wCut);
	 
	if(ReadList.is_open()) {printf("Runlist is loaded...\n");}
	else {printf("unreadable runlist...\n"); return void();}
	
	double dut_TminC = 0.0;
	double dut_TmaxC = 0.0;
	double dut_PminC = 0.0;
	double dut_PmaxC = 0.0;
	double trig_TminC = 0.0;
	double trig_TmaxC = 0.0;
	double trig_PminC = 0.0;
	double trig_PmaxC = 0.0;
	
	std::string iFile;	
	
	int counter = 1;
	
	while(ReadList >> iFile >> dut_TminC >> dut_TmaxC >> dut_PminC >> dut_PmaxC >> trig_TminC >> trig_TmaxC >> trig_PminC >> trig_PmaxC)
	{
		printf(" \n");
		printf("%s\n", std::string(90, '=').c_str());
		printf("Reading file %d \n", counter);
		
		Roo_AutoGet Tmp ( iFile.c_str() );
		Tmp.SetBranch();
		Tmp.FillDataSet (dut_TminC, dut_TmaxC, dut_PminC, dut_PmaxC, trig_TminC, trig_TmaxC, trig_PminC, trig_PmaxC);
		Tmp.Result_Generator (lxg_flag, cb_flag);
		printf("finished file %d \n", counter);
		printf("%s\n", std::string(90, '=').c_str());
		printf(" \n");
		counter++;		
	}
	
	printf("%s\n", std::string(90, '=').c_str());
	printf("Start dumping plots...\n");
	
	int dir_check;
	
	dir_check = mkdir("Roo_Results", ACCESSPERMS);
		
	if(dir_check == 0){printf("Directory is created. \n");}
	
	else{printf("Directory already exists! Previous data will be replaced...\n");}
		
	system("mv *.png Roo_Results");
	system("mv *_RooResults.txt Roo_Results");
	
	printf("output is in the Roo_Results directory. \n");
	printf("\n");
	
	
	ReadList.close();
	
}


int main (int argc, const char **argv)
{
	int verboseLevel = 2;
	if(verboseLevel > 1)
	{
		RooMsgService::instance().setSilentMode(true);
		RooMsgService::instance().setGlobalKillBelow(RooFit::FATAL);
	}
	
	if(argc == 2 ) 
	{
		GetResults ( argv[1] );
		printf("Finished!\n"); 
		exit (EXIT_SUCCESS);
	}
	
	else if (argc == 3)
	{
		if( (strcmp(argv[2], "-lxg") == 0) || (strcmp(argv[2], "-cb") == 0) )
		{
			GetResults ( argv[1], argv[2] );
		}
		else
		{
			printf("invalid flag.\n");
			exit (EXIT_FAILURE);
		}
	}
	
	else if (argc == 4)
	{
		if( ( (strcmp(argv[2], "-lxg") == 0) || (strcmp(argv[2], "-cb") == 0) ) && ((strcmp(argv[3], "-lxg") == 0) || (strcmp(argv[3], "-cb") == 0) ) )
		{
			GetResults ( argv[1], argv[2], argv[3] );
		}
		else
		{
			printf("invalid flag.\n");
			exit (EXIT_FAILURE);
		}
	}
	
	else 
	{
		printf("input parameter error!\n");
		exit (EXIT_FAILURE);
	}
}



