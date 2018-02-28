#include "../hpp/KeysightBinFormat.h"

void AutoRead ( const char* ParentFolder )
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
}

int main (int argc, char** argv)
{
	if(argc == 2) AutoRead(argv[1]);
	else AutoRead("ParentFolder");
}


