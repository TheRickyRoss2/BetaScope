#ifndef getResults_h
#define getResults_h

#include "lib.h"

class getData
{
	int entries;
		
	int corr_ch;
	
	int coin_events;
	
	int DUT     = 1;
	int Trigger = 2;
	
	double tmax_min, tmax_max, pmax_min, pmax_max;
	int ini_bin = 60;
	
	int pmax_bin[4]      = {ini_bin, ini_bin, ini_bin, ini_bin};
	int rise_1090_bin[4] = {ini_bin, ini_bin, ini_bin, ini_bin};
	int charge_bin[4]    = {ini_bin, ini_bin, ini_bin, ini_bin};
	int dvdt_bin[4]      = {ini_bin, ini_bin, ini_bin, ini_bin};
	int rms_bin[4]       = {ini_bin, ini_bin, ini_bin, ini_bin};
	int FWHM_bin[4]      = {ini_bin, ini_bin, ini_bin, ini_bin};
	
	double pmax_range[4][2] = {{1,1},{1,1},{1,1},{1,1}};
	double rise1090_range[4][2]= {{1,1},{1,1},{1,1},{1,1}};
	double charge_range[4][2]= {{1,1},{1,1},{1,1},{1,1}};
	double dvdt_range[4][2]= {{1,1},{1,1},{1,1},{1,1}};
	double rms_range[4][2]= {{1,1},{1,1},{1,1},{1,1}};
	double FWHM_range[4][2]= {{1,1},{1,1},{1,1},{1,1}};
	
	bool auto_adjust = true;
	
	//std::string cuts;

	std::string current_file;
	
	//--------8 11 2017-------//
	
	int landau_left  = 5;
	int landau_right = 20;

	//------------------------//

	public:
		std::string public_cuts = "no cuts yet";
		
		void rebin( int bins );

		void rebin( std::string name,
		            int         ch, 
		            int         bins );

		void range( std::string name, 
		            int         ch, 
		            double      min, 
		            double      max );

		void max( const char* path,
		          int         dut, 
		          int         trig, 
		          TCut        cuts     = "", 
		          bool        saveFile = false, 
		          double      Ymin     = -1.0, 
		          double      Ymax     = -1.0, 
		          double      Xmax     = -1.0 );

		void magic( const char* path, 
		            double      tmax2_min, 
		            double      tmax2_max, 
		            double      pmax2_min, 
		            double      pmax2_max, 
		            double      tmax3_min, 
		            double      tmax3_max, 
		            double      pmax3_min, 
		            double      pmax3_max );

		void set_ch (int dut = 2, int trigger = 3);

		void reset();

		void adjust_hist (bool on_off);
		
		TCut setcuts (double Dut_tmin,  double Dut_tmax,  double Dut_pmin,  double Dut_pmax,
		
		              double Trig_tmin, double Trig_tmax, double Trig_pmin, double Trig_pmax,
		              
		              int DUT, int Trigger);
		
		std::pair<double,double> Make_Histogram( const char* path,
		                                         std::string HistName, 
		                                         std::string expression, 
		                                         std::string FitFunc, 
		                                         TCut        cuts, 
		                                         int         Channel, 
		                                         int         &binNum, 
		                                         double      &range_min, 
		                                         double      &range_max, 
		                                         int         draw     = 1, 
		                                         bool        saveFile = false, 
		                                         bool        Roo      = false,
		                                         bool        confine  = false );
		
		void pulses( const char* path,
                     TCut        cuts, 
                     int         DUT, 
                     int         Trigger, 
                     int         draw = 1 );
		
		void magic2( const char* path, 
		             double Dut_tmin,  double Dut_tmax,  double Dut_pmin,  double Dut_pmax, 
		             double Trig_tmin, double Trig_tmax, double Trig_pmin, double Trig_pmax, 
		             int  draw      = 1, 
		             bool saveFile = false, 
		             bool logging  = true, 
		             bool Roo      = false );
		
		void magic3( const char* path, 
		             double Dut_tmin,  double Dut_tmax,  double Dut_pmin,  double Dut_pmax, 
		             double Trig_tmin, double Trig_tmax, double Trig_pmin, double Trig_pmax,
		             int  draw     = 1,
		             bool saveFile = true, 
		             bool logging  = false, 
		             bool Roo      = false);
		
		void see_status ();
		
		void landau_hist_factor ( int left, int right );
	
		//void single_hist ( const char* path, std::string hist_type );
		
		void magic_loop( const char* path, 
		                 double Dut_tmin,  double Dut_tmax,  double Dut_pmin,  double Dut_pmax, 
		                 double Trig_tmin, double Trig_tmax, double Trig_pmin, double Trig_pmax, 
		                 int  draw     = 0, 
		                 bool saveFile = false, 
		                 bool logging  = true, 
		                 bool Roo      = false);

};

#endif
