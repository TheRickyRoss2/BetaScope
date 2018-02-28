#include "../hpp/lib.h"
#include "../hpp/getResults.h"

void GetResults(std::string RunList_wCut, bool Roo = false, bool logging = false)
{
	getData TEMP;
	
	std::string Line;
	std::ifstream ReadList ( RunList_wCut );
	
	gROOT->SetBatch(true);
		
	double dut_TminC = 0.0;
	double dut_TmaxC = 0.0;
	double dut_PminC = 0.0;
	double dut_PmaxC = 0.0;
	double trig_TminC = 0.0;
	double trig_TmaxC = 0.0;
	double trig_PminC = 0.0;
	double trig_PmaxC = 0.0;
	//char *str;
	
	std::string iFile;	
	
	while(ReadList >> iFile >> dut_TminC >> dut_TmaxC >> dut_PminC >> dut_PmaxC >> trig_TminC >> trig_TmaxC >> trig_PminC >> trig_PmaxC)
	{
		
		std::cout << iFile << std::endl;
		//printf("\n , %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf \n", dut_TminC, dut_TmaxC, dut_PminC, dut_PmaxC, trig_TminC, trig_TmaxC, trig_PminC, trig_PmaxC);
		
		//TEMP.magic_loop(iFile, dut_TminC, dut_TmaxC, dut_PminC, dut_PmaxC, trig_TminC, trig_TmaxC, trig_PminC, trig_PmaxC, 1, false);
		
		TEMP.magic3(iFile.c_str(), dut_TminC, dut_TmaxC, dut_PminC, dut_PmaxC, trig_TminC, trig_TmaxC, trig_PminC, trig_PmaxC, 1, true, logging, Roo);	
	}
	
	
	printf("Start dumping plots...\n");
	
	int dir_check;
	
	dir_check = mkdir("Results", ACCESSPERMS);
		
	if(dir_check == 0){printf("Directory is created. \n");}
	
	else{printf("Directory already exists! Previous data will be replaced...\n");}
		
	system("mv *.png Results");
	system("mv *_results.txt Results");
	
	printf("output is in the Results directory. \n");
	printf("\n");
	
	
	ReadList.close();
	
	gROOT->SetBatch(false);
}







int main(int argc, const char *argv[])
{
	//RooMsgService::instance().setSilentMode(true);
	//RooMsgService::instance().setGlobalKillBelow(RooFit::FATAL);
	
	if(argc == 1) printf("Error! Runlist Requires\n");
	if(argc == 2) GetResults(argv[1]);
	if(argc == 3) GetResults(argv[1], argv[2]);
	if(argc == 4) GetResults(argv[1], argv[2], argv[3]);
	if(argc > 4) printf("Error! Check the input arguments!");
}



