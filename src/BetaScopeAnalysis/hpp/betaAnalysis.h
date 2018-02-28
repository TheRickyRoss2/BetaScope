    ///////////////////////////////
   //                           //
  // Argument class header     //
 //                           //
///////////////////////////////

#ifndef betaAnalysis_H
#define betaAnalysis_H

#include "lib.h"

class betaAnalysis
{
	TFile *iFile = new TFile;
	TTree *iTree = new TTree;
	
	TFile *oFile = new TFile;
	TTree *oTree = new TTree;
	
	const char* path;
	
	static const int numCh = 4;
	std::vector<int> channel = {};
	std::vector<int> invertPulse = {};
	
	int numEvent;
	int npoint;
	
	std::string filePrefix = "stats_";
	std::string ofileName;
	std::string ifileName;
	
	std::string iTreeName = "wfm";
	int compressionLevel = 8;
	
	double TimeZoomRange = 1000.0;
	int TimeZoomRange_Switch = 0;
	
	std::vector<float> *wf_v[numCh] = {};
	std::vector<float> *tf_v[numCh] = {};
	
	std::string sampleLeaf = "w2";
	static const int arrayDataBuffer = 1002;
	double wf_a[numCh][arrayDataBuffer];
	double tf_a[numCh][arrayDataBuffer];
	
	int m_inoise;
	
	double voltageScalar = 1000.0;
	double timeScalar = 1.0e12;
	
	char iBranchType = 'v';
	
	int trig = 3;
	
	int fineCFDStep = 500;
	
	int lookForNumPeak = 4;
	
	bool parallelStatus = false;
	
	std::string objectLocation = "default";
	
	std::vector<double> w[numCh];
	std::vector<double> t[numCh];
	std::vector<double>  wbase[numCh];
	std::vector<double>  b[numCh];
	std::vector<double>  pmax[numCh];
	std::vector<double>  multiplePmax[numCh];
	std::vector<double>  multipleTmax[numCh];
	//std::vector<double>  pmax_fit[numCh];
	std::vector<double>  tmax[numCh];
	std::vector<double>  rise_1090[numCh];
	std::vector<double>  pulse_area_fill[numCh];
	std::vector<double>  rms[numCh];
	std::vector<double>  dvdt[numCh];
	std::vector<double>  cfd[numCh];
	std::vector<double>  cfd_fall[numCh];
	//std::vector<double>  dvdt_fit[4];
	std::vector<double> thTime[numCh];
	std::vector<double> fineCFDRise[numCh];
	std::vector<int> singlePeak[numCh];
	
	
	std::pair<double,double> noise2, noise3;
	std::pair<double,double> baseline2, baseline3;
	
	public:
	
		betaAnalysis(const char* ipath);
		betaAnalysis(const char* ipath, char iBranchType);
		//betaAnalysis(const betaAnalysis &copy){ TThread::Lock(); std::cout << "copy constructor " << this << std::endl; TThread::UnLock(); this->objectLocation = "copy version"; }
		~betaAnalysis()
		{
			TThread::Lock();
			std::cout << this << " call destructor at " << this->objectLocation <<std::endl;
			TThread::UnLock();
			
			//delete this->iTree;
			//delete this->iFile;
			//delete this->oTree;
			//delete this->oFile;
		}
		
		void setBranch();
		void clearBuffer();
		void fillData();
		void saveFile();
		
		
		void base_line( int npoint, std::vector<double> &w, int m_inoise );
		
		bool pulse_baseline(std::vector<double> w, std::pair<double, double>& baseline, std::pair<double, double>& noise_rms, int m_inoise);
		
		std::pair <double, int> multiplePeaks( int npoints, 
		                                       std::vector<double> w, 
		                                       std::vector<double> t, 
		                                       int StartIndex, 
		                                       int TimeRestriction_OnOff = 0, 
		                                       double TimeRestrictionRange = 1000.0 );
		
		double TimeAtIndex( std::vector<double> t, 
		                    int index );
		
		std::pair<double, int> pulse_max_with_index_fixed( const int npoints, 
		                                                   std::vector<double> w, 
		                                                   std::vector<double> t, 
		                                                   const int TimeRestriction_OnOff = 0, 
		                                                   const double TimeRestrictionRange = 1000.0 );
        
        std::pair<double, int> pulse_max_with_index( const int npoints, std::vector<double> w, std::vector<double> t, const int TimeRestriction_OnOff = 0, const double TimeRestrictionRange1 = 1000.0, const double TimeRestrictionRange2 = 1000.0 );  
                                             
        double time_max( const int npoints, 
                         std::vector<double> w, 
                         std::vector<double> t, 
                         const std::pair<double,int> pmax_holder, 
                         const int TimeRestriction_OnOff = 0, 
                         const double TimeRestrictionRange = 1000.0 );
                 
        
       double pulse_area( const int npoints, 
                          std::vector<double> w, 
                          std::vector<double> t, 
                          const std::pair<double,int> pmax_holder, 
                          const int TimeRestriction_OnOff = 0, 
                          const double TimeRestrictionRange = 1000.0 );
                   
       double rise_int ( const double x1, 
                         const double y1, 
                         const double x2, 
                         const double y2, 
                         const double y3 );
                  
       double cfd_index ( const int npoints, 
                          const double fraction, 
                          std::vector<double> w, 
                          std::vector<double> t, 
                          const std::pair<double,int> pmax_holder, 
                          const int TimeRestriction_OnOff, 
                          const double TimeRestrictionRange );
       
       double pulse_min( int npoints, std::vector<double> w);
                   
       double rise_time( const int  npoints, 
                         std::vector<double> w, 
                         std::vector<double> t, 
                         const std::pair<double,int> pmax_holder, 
                         double bottom, 
                         double top, 
                         const int TimeRestriction_OnOff = 0, 
                         const double TimeRestrictionRange  = 1200.0);
                  
                  
       double noise_rms( const int npoints,
                         std::vector<double> w, 
                         const int inoise );
                  
                  
      double pulse_dvdt_cfd( const int npoints, 
                             const int fraction, 
                             const int ndif, 
                             std::vector<double> w, 
                             std::vector<double> t, 
                             const std::pair<double,int> pmax_holder, 
                             const int TimeRestriction_OnOff = 0, 
                             const double TimeRestrictionRange  = 1000.0 );
                       
                                         
      double cfd_index_falling( const int npoints, 
                                const int fraction, 
                                std::vector<double> w, 
                                std::vector<double> t, 
                                const std::pair<double,int> pmax_holder, 
                                const int TimeRestriction_OnOff = 0, 
                                const double TimeRestrictionRange  = 1000.0  );
      
      double xlinearInter( const double x1,
                           const double y1,
                           const double x2,
                           const double y2,
                           const double y );
                     
      double getThresholdTime( const int           npoints,
                               const double        thresholdLevel,
                               std::vector<double> voltage,
                               std::vector<double> time,
                               const std::pair<double,int> pmax_holder,
                               const int           TimeRestriction_OnOff = 0,
                               const double        TimeRestrictionRange  = 1000.0);
		
		
		double dvdt_linear(int ndata_pt, int npoints, int fraction, std::vector<double> w, std::vector<double> t, int inoise, int TimeRestriction_OnOff = 0, double TimeRestrictionRange = 1000.0);
		
		double fit_pmax ( int npoints, std::vector<double> w, std::vector<double> t, int inoise, int range, int TimeRestriction_OnOff = 0, double TimeRestrictionRange = 1200.0);
		
		
	
		int get_numEvent(){ return this->numEvent; }
		std::string get_path(){ std::string str = this->path; return str; }
		
		int get_npoint(){ return this->npoint; }
		
		double get_voltageScalar(){ return this->voltageScalar; }
		void set_voltageScalar(double iValue){ this->voltageScalar = iValue; }
		
		double get_timeScalar(){ return this->timeScalar; }
		void set_timeScalar(double iValue){ this->timeScalar = iValue; }
		
		std::string get_iTreeName(){ return this->iTreeName; }
		void set_iTreeName( std::string iValue ){ this->iTreeName = iValue; }
		
		int get_compressionLevel(){ return this->compressionLevel; }
		void set_compressionLevel( int iValue ){ this->compressionLevel = iValue; }
		
		std::string get_sampleLeaf(){ return this->sampleLeaf; }
		void set_sampleLeaf( std::string iValue ){ this->sampleLeaf = iValue; }
		
		std::string get_filePrefix(){ return this->filePrefix; }
		void set_filePrefix( std::string iValue ){ this->filePrefix = iValue; }
		
		std::string get_ofileName(){ return this->ofileName; }
		std::string get_ifileName(){ return this->ifileName; }
		
		bool get_parallelStatus(){ return parallelStatus; }
		void set_parallelStatus( bool iValue ){ this->parallelStatus = iValue; }
		
		char get_iBranchType(){ return iBranchType; }
		void set_iBranchType( char iValue ){ this->iBranchType = iValue; }


		double get_TimeZoomRange(){ return this->TimeZoomRange; }
		void set_TimeZoomRange(double iValue){ this->TimeZoomRange = iValue; }
		
		std::string get_objectLocation(){ return this->objectLocation; }
		void set_objectLocation(std::string iValue){ this->objectLocation = iValue; }
		
		int get_TimeZoomRange_Switch(){ return this->TimeZoomRange_Switch; }
		void set_TimeZoomRange_Switch(int iValue){ this->TimeZoomRange_Switch = iValue; }
		
		void enableChannel(int whichCh, int invert){ this->channel.push_back( whichCh ); this->invertPulse.push_back( invert ); std::cout << "channel " << whichCh << "is enable. Invert pulses = " << invert << " (0=off, 1=on)" << std::endl; }
			
};


void *betaAnalyzer( void *iarg );
void parallelBetaAnalyzer( std::vector<betaAnalysis> argVector );




#endif // #ifdef anal_cxx
