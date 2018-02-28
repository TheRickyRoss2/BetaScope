#include "../hpp/KeysightBinFormat.h"

void ReadKeysight ( const char* folder, const char* name, int ch1, int ch2, int ch3, int ch4)
{
	TFile *NewFile = new TFile ( Form("%s_parse.root",name), "RECREATE", "8");
	
	TTree *NewTree = new TTree ("wfm", "Keysight Waveform");
	
	int channel[4] = {ch1, ch2, ch3, ch4};
	
	std::vector<float> Voltage[4] = {};
    
    std::vector<float> Time[4] = {};
    
     for (int ch = 0; ch < 4; ch++)
    {
    	if (channel[ch])
		{
    		NewTree-> Branch( Form("w%d", channel[ch] ),"vector<float>", & Voltage[ch] );
    		
    		NewTree-> Branch( Form("t%d", channel[ch] ),"vector<float>", & Time[ch] );
		}    
    }
    
    KeysightBinFormat Format;
    
    int counter = 0;
    
    bool check = true;
    
    //int follower = 0;
    
    while(check)
    {	
    	check = Process ( folder, name, counter, Format, channel, Voltage, Time);
    	
    	if(counter%100==0) std::cout<< "parsed: " << counter << std::endl; 
    	
    	if(check) 
    	{
    		NewTree-> Fill();
    		counter = counter + 1;
    		
    		//for(int i=0; i< Voltage[1].size();i++)cout<<Voltage[1].at(i)<<endl;
    		
    		for(int ch =0; ch<4;ch++)
    		{
    			Voltage[ch].clear();
    			Time[ch].clear();
    		}
    		
    		//if(follower == counter) cout<< "parsed: " << follower; follower=follower+100;
    	}
    	else break;
    }
    
    NewFile-> Write();
    NewFile->Close();
    delete NewFile;    
}

