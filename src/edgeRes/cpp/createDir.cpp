#include "../hpp/edgeRes.h"

void CreateDir(const char* DirName)
{
	int dir_check;
	dir_check = mkdir(DirName, ACCESSPERMS);
	if(dir_check == 0){printf("Directory is created. \n");}
	else{printf("Directory already exists! Previous data will be replaced...\n");}
}

void Create_Threshold_Scan_Runlist(std::string sendToDirName)
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
	olist.open("Threhold_Runlist.txt");
	
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
	sprintf (NameBuffer, "mv Threhold_Runlist.txt %s/", sendToDirName.c_str());
	system(NameBuffer);	
}


