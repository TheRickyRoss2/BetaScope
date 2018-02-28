#include "../hpp/cfd_scan.h"
#include "../hpp/average_pulse.h"
#include "../hpp/ploty.h"

void CreateDir(const char* DirName)
{
	int dir_check;
	dir_check = mkdir(DirName, ACCESSPERMS);
	if(dir_check == 0){printf("Directory is created. \n");}
	else{printf("Directory already exists! Previous data will be replaced...\n");}
}

void CreateRunlist(std::string sendToDirName)
{
	char NameBuffer[1024];
	sprintf (NameBuffer, "cp Runlist_cut.txt %s/Runlist_ave.txt", sendToDirName.c_str());
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

void GenerateAveragePulse(std::string location, std::string MakeList)
{
	std::cout << MakeList << std::endl;
	CreateDir(location.c_str());
	CreateRunlist(location);
}

void GenerateAveragePulse()
{
	//CreateDir(location);
	//CreateRunlist(location);
	//cdTo(location);

	std::string Line;
	std::ifstream ReadList;
	ReadList.open( "Runlist_ave.txt" );
	
	if(ReadList.is_open())printf("File is open \n");
	else printf("Cannot open the file.\n");
	
	std::string iFile;
	double dut_TminC = 0.0;
	double dut_TmaxC = 0.0;
	double dut_PminC = 0.0;
	double dut_PmaxC = 0.0;
	double trig_TminC = 0.0;
	double trig_TmaxC = 0.0;
	double trig_PminC = 0.0;
	double trig_PmaxC = 0.0;
	std::string HistName;
	int colors = 2;
	
	//header
	std::string oFile;
	int psec_pt;
	
	ReadList >> oFile >> psec_pt;
	
	while(ReadList >> iFile >> dut_TminC >> dut_TmaxC >> dut_PminC >> dut_PmaxC >> trig_TminC >> trig_TmaxC >> trig_PminC >> trig_PmaxC >> HistName)
	{
		average( Form("../%s",iFile.c_str()), oFile, dut_TminC, dut_TmaxC, dut_PminC, dut_PmaxC, trig_TminC, trig_TmaxC, trig_PminC, trig_PmaxC, HistName, psec_pt, colors, 1);
		
		average( Form("../%s",iFile.c_str()), oFile, dut_TminC, dut_TmaxC, dut_PminC, dut_PmaxC, trig_TminC, trig_TmaxC, trig_PminC, trig_PmaxC, HistName, psec_pt, colors, 0);
		
		colors ++;
	}	
}

int main (int argc, char** argv)
{
	if(argc == 3) GenerateAveragePulse(argv[1], argv[2]);
	else if(argc == 1) GenerateAveragePulse();
	else printf("Error! Check your parameters \n");
	
	return 0;
}









