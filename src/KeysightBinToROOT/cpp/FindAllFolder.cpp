#include "../hpp/KeysightBinFormat.h"

std::vector<std::string> FindAllFolder ( const char* ParentFolder ) 
{

	DIR *dir;
	struct dirent *ent;
	
	std::vector<std::string> SubFolders = {};
	
	if ((dir = opendir (ParentFolder)) != NULL) 
	{
  		/* print all the files and directories within directory */
  		while ((ent = readdir (dir)) != NULL) 
  		{
    		SubFolders.push_back( ent->d_name );
    		//file_counter += 1;
  		}
  	closedir (dir);
  	}
  	
  	for(int i = 0; i<SubFolders.size() ;i++)
  	{
  		std::cout << SubFolders[i] << std::endl;
  	}
  	
  	//dir-> ~DIR();
  	//ent-> ~dirent();
	return SubFolders;
}


