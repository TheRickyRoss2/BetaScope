#ifndef KeysightBinFormat_H
#define KeysightBinFormat_H

#include "lib.h"

struct KeysightBinFormat
{
	//File Header
	char Cookie[2]; //Two byte characters, AG, which indicates that the file is in the Keysight Binary Data file format.
	
	char Version[2];//	Two bytes that represent the file version.
	
	int FileSize; // An integer that is the number of bytes that are in the file.
	
	int NumOfWave; // An integer that is the number of waveforms stored in the file.

	//skip 8 bytes
	
	//Waveform Header
	
	int WfmHeaderSize; // An integer that is the number of bytes in the header.
	
	int WfmType; // An integer that is the type of waveform stored in the file.
	
	int WfmBuffer; // An integer that is the number of waveform buffers required to read the data.
	
	int Points; // An integer that is the number of points in the waveform.
	
	int Count; // An integer that is the number of hits at each time bucket in the waveform record when the waveform was created using an acquisition mode like averaging.
	
	float XDisplayRange; // A float that is the number of X display range columns (n) depends on the number of sources being stored. 
	
	double XDisplayOrigin; // A double that is the number of X display origin columns (n) depends on the number of sources being stored.
	
	double XIncrement; // A double that is the number of X increment columns (n) depends on the number of sources being store.
	
	double XOrigin;
	
	int XUnit; //1 volt 2 second...
	
	int YUnit;
	
	char Date[16];
	
	char Time[16];
	
	char Frame[24]; // A 24-character array that is the model number and serial number of the oscilloscope in the format of MODEL#:SERIAL#.
	
	char WfmLabel[16];//  16-character array that contains the label assigned to the waveform.
	
	double TimeTag; // A double that is the time tag value of the segments being saved.
	
	unsigned int SegmentIndex; //An unsigned integer that is the segment index of the data that follows the waveform data header.
	
	
	//Waveform data header;
	
	int WfmDataHeaderSize;
	
	short BufferType;
	
	short BytesPerPoint;
	
	int BufferSize;


	//local holder
		
	int local_NumOfWave;
	int local_WfmHeaderSize; 
	int local_Points;
	double local_XIncrement;
	int local_WfmDataHeaderSize;
	short local_BytesPerPoint;
	int local_BufferSize;
	
};

bool Process ( const char* folder, const char* name, int counter, KeysightBinFormat &Format, int channel[4], std::vector<float> Voltage[4], std::vector<float> Time[4]);

void ReadKeysight ( const char* folder, const char* name, int ch1, int ch2, int ch3, int ch4);

std::vector<std::string> FindAllFolder ( const char* ParentFolder );

void AutoRead ( const char* ParentFolder );


#endif


