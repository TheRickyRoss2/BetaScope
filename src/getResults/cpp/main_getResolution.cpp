#include "../hpp/lib.h"
#include "../hpp/getResolution.h"

void GetResolution(std::string RunList_wCut)
{
	std::string Line;
	std::ifstream ReadList ( RunList_wCut );
	
	gROOT->SetBatch(true);
	
	gROOT->ProcessLine(".! mkdir Results");
	
	//gROOT->ProcessLine(".! cd Tmax_Pmax_Plots");
	
	//gROOT->ProcessLine(".! pwd");
	
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
		//TEMP.magic_loop(iFile, dut_TminC, dut_TmaxC, dut_PminC, dut_PmaxC, trig_TminC, trig_TmaxC, trig_PminC, trig_PmaxC, 1, false);
		
		jitter_dvdt(iFile.c_str(), dut_TminC, dut_TmaxC, dut_PminC, dut_PmaxC, trig_TminC, trig_TmaxC, trig_PminC, trig_PmaxC);
		
		std::pair<double,double> Res = res(iFile.c_str(), dut_TminC, dut_TmaxC, dut_PminC, dut_PmaxC, trig_TminC, trig_TmaxC, trig_PminC, trig_PmaxC);	
			
	}
	
	gROOT->ProcessLine(".! mkdir Resolution_Folder");
	gROOT->ProcessLine(".! mv *.png Resolution_Folder");
	//gROOT->ProcessLine(".! mv *_results.txt Results");
	
	ReadList.close();
	
	gROOT->SetBatch(false);
}


int main(int argc, const char *argv[])
{
	if(argc < 2 or argc > 3) printf("Error! Check input parameters\n");
	else GetResolution(argv[1]);
}
