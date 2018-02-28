//=====Roofit version================

class Roo_AutoGet
{
	std::vector<double> *w[4] = {};
	std::vector<double> *t[4] = {};
	std::vector<double> *Pmax[4] = {};
	std::vector<double> *Tmax[4] = {};
	std::vector<double> *Rms[4] = {};
	std::vector<double> *RiseTime[4] = {};
	std::vector<double> *PulseArea[4] = {};
	std::vector<double> *CFD[4] = {};
	std::vector<double> *Dvdt[4] = {};
	std::vector<double> *CFD_Fall[4] = {};
	
	int VarFactor = 10;
	int coincident_Events;
	
	std::string ifile_name;
	public:
	
	
	std::pair<double, double> Pmax_range[4];
	std::pair<double, double> Tmax_range[4];
	std::pair<double, double> Rms_range[4];
	std::pair<double, double> RiseTime_range[4];
	std::pair<double, double> PulseArea_range[4];
	std::pair<double, double> CFD_range[4];
	std::pair<double, double> Dvdt_range[4];
	std::pair<double, double> FWHM_range[4];
	
	RooRealVar *RooPmax[4];
	RooRealVar *RooTmax[4];
	RooRealVar *RooRms[4];
	RooRealVar *RooRiseTime[4];
	RooRealVar *RooPulseArea[4];
	RooRealVar *RooCFD[4];
	RooRealVar *RooDvdt[4];
	RooRealVar *RooFWHM[4];
	
	RooDataSet *RooPmax_Set[4];
	RooDataSet *RooTmax_Set[4];
	RooDataSet *RooRms_Set[4];
	RooDataSet *RooRiseTime_Set[4];
	RooDataSet *RooPulseArea_Set[4];
	RooDataSet *RooCFD_Set[4];
	RooDataSet *RooDvdt_Set[4];
	RooDataSet *RooFWHM_Set[4];
	
	TFile* iFile;
	TTree* iTree; 
	
	int Dut = 1;
	int Trig = 2;
	
	int Entries;
	
	std::vector<int> ch = {0, 2, 3, 0};

	
	//public:
	
	Roo_AutoGet (const char*);
	~Roo_AutoGet ();
	
	
	//===================methods===================
	void SetBranch ( );
	
	void FillDataSet (double Dut_tmin, double Dut_tmax, double Dut_pmin, double Dut_pmax, double Trig_tmin, double Trig_tmax, double Trig_pmin, double Trig_pmax);
	
	std::pair<double, double> LandauFit (std::string WhichPar, int Channel, RooDataSet * DataSet, RooRealVar *Var, std::pair<double, double> range, bool SaveImg = true);
	
	std::pair<double, double> GaussianFit (std::string WhichPar, int Channel, RooDataSet * DataSet, RooRealVar *Var, std::pair<double, double> range, bool SaveImg = true);
	
	std::pair<double, double> LxG(std::string WhichPar, int Channel, RooDataSet * DataSet, RooRealVar *Var, std::pair<double, double> range, bool SaveImg = true);
	
	std::pair<double, double> CrystallBallFit(std::string WhichPar, int Channel, RooDataSet * DataSet, RooRealVar *Var, std::pair<double, double> range, bool SaveImg = true);
	
	std::pair<double, double> RooBifurGaussianFit(std::string WhichPar, int Channel, RooDataSet * DataSet, RooRealVar *Var, std::pair<double, double> range, bool SaveImg = true);
	
	std::pair<double, double> GaussAddCB(std::string WhichPar, int Channel, RooDataSet * DataSet, RooRealVar *Var, std::pair<double, double> range, bool SaveImg = true);
	
	std::pair<double, double> GaussAddGauss(std::string WhichPar, int Channel, RooDataSet * DataSet, RooRealVar *Var, std::pair<double, double> range, bool SaveImg = true);
	
	std::pair<double, double> GaussAddGauss_splot(std::string WhichPar, int Channel, RooDataSet * DataSet, RooDataSet * DataSet2, RooRealVar *Var, RooRealVar *Var2, std::pair<double, double> range, std::pair<double, double> range2, bool SaveImg = true);
	
	void Result_Generator ( std::string flag1 = "", std::string flag2 = "");
	
	
};

Roo_AutoGet::Roo_AutoGet (const char* ipath)
{
	iFile = TFile::Open(ipath);
	iTree = (TTree*) iFile->Get("wfm");
	Entries = iTree->GetEntries();
	ifile_name = ipath;
	if(iFile->IsZombie()) {printf("file is bad.\n");}
	else {printf("%s is loaded... \n", ipath);}
}

Roo_AutoGet::~Roo_AutoGet(void)
{
	printf("Deleting the object...\n");
	
	for(int i = 0; i < 4; i++)
	{
		if(ch.at(i))
		{
			delete w[i];
			delete t[i];
			delete Pmax[i];
			delete Tmax[i];
			delete Rms[i];
			delete RiseTime[i];
			delete PulseArea[i];
			delete CFD[i];
			delete Dvdt[i];
			delete CFD_Fall[i];

		/*
			delete RooPmax[i];
			delete RooTmax[i];
			delete RooRms[i];
			delete RooRiseTime[i];
			delete RooPulseArea[i];
			delete RooCFD[i];
			delete RooDvdt[i];
			delete RooFWHM[i];
			
			delete RooPmax_Set[i];
			delete RooTmax_Set[i];
			delete RooRms_Set[i];
			delete RooRiseTime_Set[i];
			delete RooPulseArea_Set[i];
			delete RooCFD_Set[i];
			delete RooDvdt_Set[i];
			delete RooFWHM_Set[i];*/
		}
	}
	
	//delete iFile;
	//delete iTree;	
		
	printf("Done.\n");	
}


















