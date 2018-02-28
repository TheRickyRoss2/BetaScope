#include "../hpp/edgeRes.h"

int main (int argc, char **argv)
{
	if (argc == 2)
	{
		CreateDir(argv[1]);
		Create_Threshold_Scan_Runlist(argv[1]);
	}
	else if (argc == 1)
	{
		int exit = processingThresholdScan();
	}
	else
	{
		printf("Error! Check parameters \n"); return -1;
	}
	
	return 0;
}


