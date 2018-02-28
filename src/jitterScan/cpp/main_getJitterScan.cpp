#include "../hpp/jitterScan.h"

int main (int argc, char **argv)
{
	if (argc == 2)
	{
		CreateDir(argv[1]);
		Create_Jitter_Scan_Runlist(argv[1]);
	}
	else if (argc == 1)
	{
		int exit = processingJitterScan();
		std::cout << "exit: " << exit << std::endl;
		return exit;
	}
	else
	{
		printf("Error! Check parameters \n"); return -1;
	}
	
	return 0;
}


