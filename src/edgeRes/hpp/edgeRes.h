#ifndef degeRes_H
#define degeRes_H

#include "lib.h"

std::pair<double,double> resAtThreshold( const char *in_file, double Dut_tmin, double Dut_tmax, double Dut_pmin, double Dut_pmax, double Trig_tmin, double Trig_tmax, double Trig_pmin, double Trig_pmax, int threshold = 2, const char* addition_cuts = "", bool Save_Mode = true ,int DUT = 2, int Trig = 3);

void thresholdResScan(const char *in_file, double Dut_tmin, double Dut_tmax, double Dut_pmin, double Dut_pmax, double Trig_tmin, double Trig_tmax, double Trig_pmin, double Trig_pmax, std::string SensorName = " ", std::string Bias = " ", std::string Fluence = " ", std::string Temperature = " ");

int processingThresholdScan();

void CreateDir(const char* DirName);

void Create_Threshold_Scan_Runlist(std::string sendToDirName);


#endif



