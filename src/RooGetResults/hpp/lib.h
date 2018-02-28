#ifndef lib_h
#define lib_h

//-------c++----------------//
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
//------ROOT----------------//
#include <TH2.h>
#include <TH1.h>
#include <TStyle.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <TTree.h>
#include <iomanip>
#include <TFile.h>
#include "TF1.h"
#include <TLeaf.h>
#include <TROOT.h>
#include <TCanvas.h>
#include <TCut.h>
#include <TSystem.h>
#include <TImage.h>
#include <RooFit.h>
#include <RooDataHist.h>
#include <RooDataSet.h>
#include <RooRealVar.h>
#include <RooPlot.h>
#include <RooGaussian.h>
#include <RooLandau.h>
#include <RooFFTConvPdf.h>
#include <RooExtendPdf.h>
#include <RooCBShape.h>
#include <RooBifurGauss.h>
#include <RooAddPdf.h>
#include <RooAbsPdf.h>
#include <RooStats/SPlot.h>
#include <RooWorkspace.h>
#include <RooProdPdf.h>
#include <RooExponential.h>

namespace RooFit {}
using namespace RooFit;

namespace RooFit {}
using namespace RooFit;
//using RooAbsRealValue::operator=;

using namespace RooStats;

#endif
