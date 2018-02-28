#ifndef getResolution_h
#define getResolution_h

#include "lib.h"

std::pair<double,double> res( const char *in_file, double Dut_tmin, double Dut_tmax, double Dut_pmin, double Dut_pmax, double Trig_tmin, double Trig_tmax, double Trig_pmin, double Trig_pmax, bool Save_Mode = true, const char* addition_cuts = "" ,int DUT = 2, int Trig = 3, int cfd_per = 20);

void jitter_dvdt( const char *in_file, double Dut_tmin, double Dut_tmax, double Dut_pmin, double Dut_pmax, double Trig_tmin, double Trig_tmax, double Trig_pmin, double Trig_pmax, bool Batch_Mode = true, const char* addition_cuts = "" ,int DUT = 2, int Trig = 3, int per = 20);

std::pair<double,double> fineRes( const char *in_file, double Dut_tmin, double Dut_tmax, double Dut_pmin, double Dut_pmax, double Trig_tmin, double Trig_tmax, double Trig_pmin, double Trig_pmax, bool Save_Mode = true, const char* addition_cuts = "" ,int DUT = 2, int Trig = 3, int cfd_per = 20);


#endif
