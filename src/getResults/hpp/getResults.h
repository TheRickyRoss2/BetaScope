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

	int    ini_bin = 60;
	double ini_rangeMin = 1;
	double ini_rangeMax = 1;

	bool auto_adjust = true;

	int recursionNum = 5;

	std::vector<std::vector<std::string>> plotArg[2] = {};

	std::string current_file;

	int landau_left  = 5;
	int landau_right = 20;

	public:
		std::string public_cuts = "no cuts yet";

		void setPlotArg();


		void max(
			const char* path,
			int         dut,
			int         trig,
			TCut        cuts     = "",
			bool        saveFile = false,
			double      Ymin     = -1.0,
			double      Ymax     = -1.0,
			double      Xmax     = -1.0
		);

		void magic(
			const char* path,
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

		TCut setcuts(
			double Dut_tmin,
			double Dut_tmax,
			double Dut_pmin,
			double Dut_pmax,
			double Trig_tmin,
			double Trig_tmax,
			double Trig_pmin,
			double Trig_pmax,
			int DUT,
			int Trigger
		);

		std::tuple<double,double,int,double,double> Make_Histogram(
			const char* path,
			std::string HistName,
			std::string expression,
			std::string FitFunc,
			TCut        cuts,
			int         Channel,
			int         binNum,
			double      range_min,
			double      range_max,
			bool        makePlot = true,
			bool        savePlot = false,
			bool        confine  = false
		);

		void pulses(
			const char* path,
			TCut        cuts,
			int         DUT,
			int         Trigger,
			int         draw = 1
		);

		void processHistogram(
			const char* path,
			double      Dut_tmin,
			double      Dut_tmax,
			double      Dut_pmin,
			double      Dut_pmax,
			double      Trig_tmin,
			double      Trig_tmax,
			double      Trig_pmin,
			double      Trig_pmax,
			bool        saveResult = false,
			bool        logging  = true
		);

		void see_status ();

		void landau_hist_factor ( int left, int right );
};

#endif
