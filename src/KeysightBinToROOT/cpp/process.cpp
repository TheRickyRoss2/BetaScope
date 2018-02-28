#include "../hpp/KeysightBinFormat.h"

bool Process ( const char* folder, const char* name, int counter, KeysightBinFormat &Format, int channel[4], std::vector<float> Voltage[4], std::vector<float> Time[4])
{
	std::ifstream LoadFile;
	
	const char * path = "%s/%s%05d.bin";
	
	char PulseFile[1000];
	
	sprintf ( PulseFile, path, folder, name, counter);
	
	LoadFile.open ( PulseFile );
	
	float T;
	float V;
	
	int skipper;
	
	if (LoadFile.is_open())
	{
		LoadFile.ignore ( sizeof(Format.Cookie) );
		LoadFile.ignore ( sizeof(Format.Version) );
		LoadFile.ignore ( sizeof(Format.FileSize) );
		
		
		if (counter == 0) 
		{
			LoadFile.read ( (char*) ( & Format.NumOfWave ), sizeof ( Format.NumOfWave ) );
			
			Format.local_NumOfWave = Format.NumOfWave;
		
			std::cout<< "Total number of channels = "<< Format.NumOfWave << std::endl;
		
			LoadFile.read ( (char*) ( & Format.WfmHeaderSize ), sizeof ( Format.WfmHeaderSize ) );
		
			Format.local_WfmHeaderSize = Format.WfmHeaderSize;
		
			skipper = sizeof(Format.WfmType) + sizeof(Format.WfmBuffer);
			
			LoadFile.ignore ( sizeof(Format.WfmType) );
			LoadFile.ignore ( sizeof(Format.WfmBuffer) );
			
			LoadFile.read ( (char*) ( & Format.Points ), sizeof ( Format.Points ) );
			
			std::cout<< " number Points = "<< Format.Points << std::endl;	
			
			LoadFile.ignore ( sizeof(Format.Count) );
			LoadFile.ignore ( sizeof(Format.XDisplayRange) );
			LoadFile.ignore ( sizeof(Format.XDisplayOrigin) );

			LoadFile.read ( (char*) ( & Format.XIncrement ), sizeof ( Format.XIncrement ) );
						
			Format.local_XIncrement = Format.XIncrement;
					
			LoadFile.read ( (char*) ( & Format.XOrigin ), sizeof ( Format.XOrigin ) );
			
			LoadFile.ignore ( sizeof(Format.XUnit) );
			LoadFile.ignore ( sizeof(Format.YUnit) );
			LoadFile.ignore ( sizeof(Format.Date) );
			LoadFile.ignore ( sizeof(Format.Time) );
			LoadFile.ignore ( sizeof(Format.Frame) );
			LoadFile.ignore ( sizeof(Format.WfmLabel) );
			LoadFile.ignore ( sizeof(Format.TimeTag) );
			LoadFile.ignore ( sizeof(Format.SegmentIndex) );
			
			LoadFile.read( (char*) ( & Format.WfmDataHeaderSize ), sizeof(Format.WfmDataHeaderSize) );
							
			Format.local_WfmDataHeaderSize = Format.WfmDataHeaderSize;
			
			LoadFile.read( (char*) ( & Format.BufferType ), sizeof(Format.BufferType) );
						
			LoadFile.read( (char*) ( & Format.BytesPerPoint ), sizeof(Format.BytesPerPoint) ); 
			
			std::cout << "Bytes per point = " << Format.BytesPerPoint<< std::endl;
			
			Format.local_BytesPerPoint = Format.BytesPerPoint;
			
			LoadFile.read( (char*) ( & Format.BufferSize ), sizeof(Format.BufferSize) ); 
			
			Format.local_BufferSize = Format.BufferSize;
					
			int startParse = Format.local_NumOfWave;
			
			for(int ch = 0; ch < 4; ch++)
			{
				if(channel[ch])
				{
					//V = 0.0;
					T = 0; 
					//cout << T << endl;
					//std::cout << "channel[ch] " << channel[ch] << " counter " << counter << std::endl;
					
					
					
					for (int i = 0; i < Format.local_BufferSize/sizeof(V); i++)
					{
						LoadFile.read( (char*) ( & V ), sizeof(V) );
						T = Format.XOrigin + i * Format.local_XIncrement;
				
						Voltage[ch].push_back(V);
						Time[ch].push_back(T);
						
						//std::cout << T << "  " << V << std::endl;
						startParse = startParse - 1;
					}
					
					if(startParse != Format.local_NumOfWave)
					{
						
						LoadFile.ignore ( Format.local_WfmHeaderSize + Format.local_WfmDataHeaderSize);
						
						startParse = startParse - 1;
					}
					
				}
			}
			
			return true;
		}
		
		else
		{	
			LoadFile.ignore ( sizeof(Format.NumOfWave) );
			
			LoadFile.ignore ( sizeof(Format.WfmHeaderSize) );
			LoadFile.ignore ( sizeof(Format.WfmType) );
			LoadFile.ignore ( sizeof(Format.WfmBuffer) );
			LoadFile.ignore ( sizeof(Format.Points) );
			LoadFile.ignore ( sizeof(Format.Count) );
			LoadFile.ignore ( sizeof(Format.XDisplayRange) );
			LoadFile.ignore ( sizeof(Format.XDisplayOrigin) );
			LoadFile.ignore ( sizeof(Format.XIncrement) );
			
			LoadFile.read ( (char*) ( & Format.XOrigin ), sizeof ( Format.XOrigin ) );
			
			//cout << "h  " << Format.XOrigin << " counter " << counter  << endl;
			
			LoadFile.ignore ( sizeof(Format.XUnit) );
			LoadFile.ignore ( sizeof(Format.YUnit) );
			LoadFile.ignore ( sizeof(Format.Date) );
			LoadFile.ignore ( sizeof(Format.Time) );
			LoadFile.ignore ( sizeof(Format.Frame) );
			LoadFile.ignore ( sizeof(Format.WfmLabel) );
			LoadFile.ignore ( sizeof(Format.TimeTag) );
			LoadFile.ignore ( sizeof(Format.SegmentIndex) );
			
			LoadFile.ignore ( Format.local_WfmDataHeaderSize );
			//LoadFile.ignore ( sizeof(Format.WfmDataHeaderSize) );
			//LoadFile.ignore ( sizeof(Format.BufferType) );
			//LoadFile.ignore ( sizeof(Format.BytesPerPoint) );
			//LoadFile.ignore ( sizeof(Format.BufferSize) );
			
			int startParse = Format.local_NumOfWave;
			
			for(int ch = 0; ch < 4; ch++)
			{
				if(channel[ch])
				{
					//V = 0.0;
					T = 0;
					//cout << " T " << T << endl;
					//std::cout << "channel[ch] " << channel[ch] << " counter " << counter << std::endl;
					
					//cout << "stop " << Format.local_BufferSize/sizeof(V) << endl;
					//int s;
					//std::cin >> s ;
					
					//cout << "stop " << Format.local_BufferSize/sizeof(V) << endl;
					
					
					for (int i = 0; i < Format.local_BufferSize/sizeof(V); i++)
					{
						LoadFile.read( (char*) ( & V ), sizeof(V) );
						T = Format.XOrigin  + i * Format.local_XIncrement;
				
						Voltage[ch].push_back(V);
						Time[ch].push_back(T);
						
						startParse = startParse - 1;
						
						//std::cout << T << "  " << V << std::endl;
						
						//cin.get();
					}
					
					if(startParse != Format.local_NumOfWave)
					{
						
						LoadFile.ignore ( Format.local_WfmHeaderSize + Format.local_WfmDataHeaderSize);
						
						startParse = startParse - 1;
					}
				
				}
			}
			
			return true;
		
		}
		
	}
	
	else 
	{
		std::cout << "no file or finished " << std::endl;
		std::cout << std::string(50, '*') << std::endl;
		return false;
	}
}

