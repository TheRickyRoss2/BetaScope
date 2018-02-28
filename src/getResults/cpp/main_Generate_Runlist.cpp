#include "../hpp/lib.h"

void Generate_Runlist_raw()
{
	gROOT->ProcessLine(".! ls stats_* > Runlist_raw.txt");
	printf(" \n");
	gROOT->ProcessLine(".! cat Runlist_raw.txt ");
	printf("\nRunlist_raw.txt is ready.\n");
	printf(" \n");
	gROOT->ProcessLine(".! cp Runlist_raw.txt Runlist_cut.txt");
}

void Generate_Runlist_cut(int numLine, double Tmin_dut, double Tmax_dut, double Pmin_dut, double Pmax_dut, double Tmin_trig, double Tmax_trig, double Pmin_trig, double Pmax_trig)
{	
	gROOT->ProcessLine( Form(".! sed -i '%ds/$/ %lf %lf %lf %lf %lf %lf %lf %lf/' Runlist_cut.txt", numLine, Tmin_dut, Tmax_dut, Pmin_dut, Pmax_dut, Tmin_trig, Tmax_trig, Pmin_trig, Pmax_trig));
	
	printf("\nCut is inserted!\n");
	printf(" \n");
	gROOT->ProcessLine(".! cat Runlist_cut.txt ");
	printf(" \n");
}





int main()
{
	Generate_Runlist_raw();
	return 0;
}
