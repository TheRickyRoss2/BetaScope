#include "../hpp/KeysightBinFormat.h"
#include "../../BetaScopeAnalysis/hpp/betaAnalysis.h"

void applyBetaScope ( const char* ParentFolder )
{
	std::vector<std::string> ListOfFolder =  FindAllFolder ( ParentFolder );
	
	const char *temp;
	
	std::vector<std::string> ListOfFileName = {};
	
	for(int i = 0; i < ListOfFolder.size(); i++)
	{
		if ( ListOfFolder[i].compare(".") == 0 ) std::cout << "ignore . file " << std::endl;
		else if ( ListOfFolder[i].compare("..") == 0 ) std::cout << "ignore .. file " << std::endl;
		else
		{
			std::cout << ListOfFolder[i].c_str() << std::endl;
		
			temp = Form ("%s/%s", ParentFolder, ListOfFolder[i].c_str() );
		
			ReadKeysight ( temp, ListOfFolder[i].c_str(), 0, 2, 3, 0);
			
			ListOfFileName.push_back (ListOfFolder[i]);
		}
	}
	
	//===============Importing the beta scope code==========================
	
	int NumOfThread = 4; //default is 1.
	
	std::vector<betaAnalysis> iarg = {};
	
	iarg.reserve(NumOfThread);
	
	for (int i = 0; i < ListOfFileName.size(); i++)
	{
		betaAnalysis input( Form ( "%s_parse.root", ListOfFileName.at(i).c_str() ) );
		
		input.set_parallelStatus( true );
		
		input.set_objectLocation("keysight loop");
			
		iarg.push_back ( input );
		
		input.set_objectLocation("after push");
		
		if (iarg.size() == NumOfThread) 
		{ 
			parallelBetaAnalyzer(iarg); 
			for(int k=0;k<iarg.size();k++){iarg.at(k).set_objectLocation("after loop");}
			iarg.clear();
		}
	}
	
	if (iarg.size() != 0 )
	{ 
		parallelBetaAnalyzer(iarg);
		for(int k=0;k<iarg.size();k++){iarg.at(k).set_objectLocation("outside loop");}
		iarg.clear();
	}
}

int main (int argc, char** argv)
{
	if(argc == 2) applyBetaScope(argv[1]);
	else applyBetaScope("ParentFolder");
}


