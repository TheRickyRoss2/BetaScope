#include "../hpp/lib.h"
#include "../hpp/getResults.h"

void GetMaxPlot(std::string RunList, double Ymin = -1.0, double Ymax = -1.0, double Xmax = -1.0)
{
	getData TEMP;
	
	std::string Line;
	std::ifstream ReadList ( RunList );
	
	gROOT->SetBatch(true);
	
	gROOT->ProcessLine(".! mkdir Tmax_Pmax_Plots");
	
	//gROOT->ProcessLine(".! cd Tmax_Pmax_Plots");
	
	//gROOT->ProcessLine(".! pwd");
	
	while(getline (ReadList, Line))
	{
		TEMP.max( Line.c_str(), 2, 3, "", true, Ymin, Ymax, Xmax);
	}
	
	gROOT->ProcessLine(".! mv *.png Tmax_Pmax_Plots");
	
	gROOT->SetBatch(false);
	
	printf("Complete!\n");
}



int main(int argc, const char *argv[])
{
	if(argc == 1)printf("Error! Runlist Requires\n");
	if(argc == 2)GetMaxPlot(argv[1]);
	if(argc == 3)printf("Invalid input!\n");
	if(argc == 4)GetMaxPlot(argv[1],atof(argv[2]),atof(argv[3]));
	if(argc == 5)GetMaxPlot(argv[1],atof(argv[2]),atof(argv[3]),atof(argv[4]));
	if(argc > 5) printf("Error! Check the input arguments!");
	
	return 0;
}


