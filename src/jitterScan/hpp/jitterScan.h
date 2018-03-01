#ifndef jitterScan_H
#define jitterScan_H

#include "lib.h"

void jitterScan( const char *in_file, double Dut_tmin, double Dut_tmax, double Dut_pmin, double Dut_pmax, double Trig_tmin, double Trig_tmax, double Trig_pmin, double Trig_pmax, std::string SensorName = " ", std::string Bias = " ", std::string Fluence = " ", std::string Temperature = " ", std::string fitFunction = "landau" );

void CreateDir(const char* DirName);

void Create_Jitter_Scan_Runlist(std::string sendToDirName);

int processingJitterScan();

int processingDvdtScan();

void dvdtScan( const char *in_file, double Dut_tmin, double Dut_tmax, double Dut_pmin, double Dut_pmax, double Trig_tmin, double Trig_tmax, double Trig_pmin, double Trig_pmax, std::string SensorName = " ", std::string Bias = " ", std::string Fluence = " ", std::string Temperature = " ", std::string fitFunction = "landau" );


#endif
