########## defaut ##########

CC      = g++
DEBUG   = -g
CFLAGS  = -Wall -c $(DEBUG)
LFLAGS  = -Wall

########## ROOT Libs and links ##########

ROOT_LINKS   = `root-config --cflags --libs`
ROOFIT       = -lCore -lRooFit -lHtml -lMinuit -lRooFitCore -lRooStats
ROOT_LIBS    = -L$ROOTSYS/lib 
ROOT_INCLUDE = -I$ROOTSYS/include

########## Directory names ##########

BUILD = build/
SRC   = src/
BIN   = bin/
CPP   = cpp/
HPP   = hpp/

GETRESULTS     = getResults/
EDGE_RES       = edgeRes/
JITTER_SCAN    = jitterScan/
AVE_PULSE      = avePulse/
ROOGETRESULTS  = RooGetResults/

BETA_SCOPE_ANALYSIS = BetaScopeAnalysis/
KEYSIGHT_BIN_TO_ROOT = KeysightBinToROOT/


########## Make exe ##########

all:$(BIN)RooGetResults          \
	$(BIN)getResults             \
	$(BIN)GetTmaxPmax            \
	$(BIN)GenerateRunlist        \
	$(BIN)getResolution          \
	$(BIN)PulseShape             \
	$(BIN)getEdgeResScan         \
	$(BIN)getJitterScan          \
	$(BIN)singleBetaRun          \
	$(BIN)KeysightBinToROOT      \
	$(BIN)keysightApplyBetaScope \
	$(BIN)CFDscan_FixTrig        \
	$(BIN)fineCFDScan_FixTrig    \

#================================================================================================
#================================================================================================

#========= Beta scope analysis

$(BUILD)$(BETA_SCOPE_ANALYSIS)betaAnalysis.o: $(SRC)$(BETA_SCOPE_ANALYSIS)$(CPP)betaAnalysis.cpp
	$(CC) $(DEBUG) $(CFLAGS) $(ROOT_LINKS) $(ROOT_LIBS) $(ROOT_INCLUDE) $< -o $@
	
$(BUILD)$(BETA_SCOPE_ANALYSIS)methods.o: $(SRC)$(BETA_SCOPE_ANALYSIS)$(CPP)methods.cpp
	$(CC) $(DEBUG) $(CFLAGS) $(ROOT_LINKS) $(ROOT_LIBS) $(ROOT_INCLUDE) $< -o $@
	
$(BUILD)$(BETA_SCOPE_ANALYSIS)setBranch.o: $(SRC)$(BETA_SCOPE_ANALYSIS)$(CPP)setBranch.cpp
	$(CC) $(DEBUG) $(CFLAGS) $(ROOT_LINKS) $(ROOT_LIBS) $(ROOT_INCLUDE) $< -o $@
	
$(BUILD)$(BETA_SCOPE_ANALYSIS)fillData.o: $(SRC)$(BETA_SCOPE_ANALYSIS)$(CPP)fillData.cpp
	$(CC) $(DEBUG) $(CFLAGS) $(ROOT_LINKS) $(ROOT_LIBS) $(ROOT_INCLUDE) $< -o $@
	
$(BUILD)$(BETA_SCOPE_ANALYSIS)clearBuffer.o: $(SRC)$(BETA_SCOPE_ANALYSIS)$(CPP)clearBuffer.cpp
	$(CC) $(DEBUG) $(CFLAGS) $(ROOT_LINKS) $(ROOT_LIBS) $(ROOT_INCLUDE) $< -o $@
	
$(BUILD)$(BETA_SCOPE_ANALYSIS)saveFile.o: $(SRC)$(BETA_SCOPE_ANALYSIS)$(CPP)saveFile.cpp
	$(CC) $(DEBUG) $(CFLAGS) $(ROOT_LINKS) $(ROOT_LIBS) $(ROOT_INCLUDE) $< -o $@
	
$(BUILD)$(BETA_SCOPE_ANALYSIS)betaAnalyzer.o: $(SRC)$(BETA_SCOPE_ANALYSIS)$(CPP)betaAnalyzer.cpp
	$(CC) $(DEBUG) $(CFLAGS) $(ROOT_LINKS) $(ROOT_LIBS) $(ROOT_INCLUDE) $< -o $@

betaScope_dep := $(BUILD)$(BETA_SCOPE_ANALYSIS)betaAnalysis.o
betaScope_dep += $(BUILD)$(BETA_SCOPE_ANALYSIS)methods.o
betaScope_dep += $(BUILD)$(BETA_SCOPE_ANALYSIS)setBranch.o
betaScope_dep += $(BUILD)$(BETA_SCOPE_ANALYSIS)fillData.o
betaScope_dep += $(BUILD)$(BETA_SCOPE_ANALYSIS)clearBuffer.o
betaScope_dep += $(BUILD)$(BETA_SCOPE_ANALYSIS)saveFile.o
betaScope_dep += $(BUILD)$(BETA_SCOPE_ANALYSIS)betaAnalyzer.o

singleBetaRun_cpp := $(SRC)$(BETA_SCOPE_ANALYSIS)$(CPP)main_singleBetaRun.cpp
$(BIN)singleBetaRun: $(singleBetaRun_cpp) $(betaScope_dep)
	$(CC) $(DEBUG) $(LFLAGS) $(ROOT_LINKS) $(ROOT_LIBS) $(ROOT_INCLUDE) $^ -o $@
	
#================================================================================================
#================================================================================================
	
#========= Keysight Binary Parser 

$(BUILD)$(KEYSIGHT_BIN_TO_ROOT)process.o: $(SRC)$(KEYSIGHT_BIN_TO_ROOT)$(CPP)process.cpp
	$(CC) $(DEBUG) $(CFLAGS) $(ROOT_LINKS) $(ROOT_LIBS) $(ROOT_INCLUDE) $< -o $@
	
$(BUILD)$(KEYSIGHT_BIN_TO_ROOT)FindAllFolder.o: $(SRC)$(KEYSIGHT_BIN_TO_ROOT)$(CPP)FindAllFolder.cpp
	$(CC) $(DEBUG) $(CFLAGS) $(ROOT_LINKS) $(ROOT_LIBS) $(ROOT_INCLUDE) $< -o $@
	
$(BUILD)$(KEYSIGHT_BIN_TO_ROOT)ReadKeysight.o: $(SRC)$(KEYSIGHT_BIN_TO_ROOT)$(CPP)ReadKeysight.cpp
	$(CC) $(DEBUG) $(CFLAGS) $(ROOT_LINKS) $(ROOT_LIBS) $(ROOT_INCLUDE) $< -o $@

keysightBinToROOT_dep += $(BUILD)$(KEYSIGHT_BIN_TO_ROOT)process.o
keysightBinToROOT_dep += $(BUILD)$(KEYSIGHT_BIN_TO_ROOT)FindAllFolder.o
keysightBinToROOT_dep += $(BUILD)$(KEYSIGHT_BIN_TO_ROOT)ReadKeysight.o

keysightBinToROOT_cpp := $(SRC)$(KEYSIGHT_BIN_TO_ROOT)$(CPP)main_KeysightBinToROOT.cpp
$(BIN)KeysightBinToROOT: $(keysightBinToROOT_cpp) $(keysightBinToROOT_dep)
	$(CC) $(DEBUG) $(LFLAGS) $(ROOT_LINKS) $(ROOT_LIBS) $(ROOT_INCLUDE) $^ -o $@
	
keysightApplyBetaScope_cpp := $(SRC)$(KEYSIGHT_BIN_TO_ROOT)$(CPP)main_KeysightApplyBetaScope.cpp
$(BIN)keysightApplyBetaScope: $(keysightApplyBetaScope_cpp) $(keysightBinToROOT_dep) $(betaScope_dep)
	$(CC) $(DEBUG) $(LFLAGS) $(ROOT_LINKS) $(ROOT_LIBS) $(ROOT_INCLUDE) $^ -o $@
	

#================================================================================================
#================================================================================================

#========= get Results and make histograms

$(BUILD)$(GETRESULTS)getResults.o: $(SRC)$(GETRESULTS)$(CPP)getResults.cpp
	$(CC) $(DEBUG) $(CFLAGS) $(ROOT_LINKS) $(ROOT_LIBS) $(ROOT_INCLUDE) $< -o $@
	
$(BUILD)$(GETRESULTS)getResolution.o: $(SRC)$(GETRESULTS)$(CPP)getResolution.cpp
	$(CC) $(DEBUG) $(CFLAGS) $(ROOT_LINKS) $(ROOT_LIBS) $(ROOT_INCLUDE) $< -o $@
	
getResults_dep += $(BUILD)$(GETRESULTS)getResults.o
getResults_dep += $(BUILD)$(GETRESULTS)getResolution.o

getResults_cpp := $(SRC)$(GETRESULTS)$(CPP)main_GetResults.cpp
$(BIN)getResults: $(getResults_cpp) $(getResults_dep)
	$(CC) $(LFLAGS) $(ROOT_LINKS) $(ROOT_LIBS) $(ROOT_INCLUDE) $^ -o $@

#========= get tmax vs pma scatter plots
	
GetTmaxPmax_cpp := $(SRC)$(GETRESULTS)$(CPP)main_GetTmaxPmax.cpp
$(BIN)GetTmaxPmax: $(GetTmaxPmax_cpp) $(getResults_dep)
	$(CC) $(LFLAGS) $(ROOT_LINKS) $(ROOT_LIBS) $(ROOT_INCLUDE) $^ -o $@

#========= get runlist for users to put cuts

GenerateRunlist_cpp := $(SRC)$(GETRESULTS)$(CPP)main_Generate_Runlist.cpp
$(BIN)GenerateRunlist: $(GenerateRunlist_cpp) $(getResults_dep)
	$(CC) $(LFLAGS) $(ROOT_LINKS) $(ROOT_LIBS) $(ROOT_INCLUDE) $^ -o $@
	
#========= get resolution and jitter at CFD 20%

getResolution_cpp := $(SRC)$(GETRESULTS)$(CPP)main_getResolution.cpp
$(BIN)getResolution: $(getResolution_cpp) $(getResults_dep)
	$(CC) $(LFLAGS) $(ROOT_LINKS) $(ROOT_LIBS) $(ROOT_INCLUDE) $^ -o $@

#========= get cfd scan with 1% step

CFDscan_FixTrig_cpp := $(SRC)$(GETRESULTS)$(CPP)main_CFDscan_FixTrig.cpp
$(BIN)CFDscan_FixTrig: $(CFDscan_FixTrig_cpp) $(getResults_dep)
	$(CC) $(LFLAGS) $(ROOT_LINKS) $(ROOT_LIBS) $(ROOT_INCLUDE) $^ -o $@
	
#========= get fine cfd scan with 0.2% step

fineCFDScan_FixTrig_cpp := $(SRC)$(GETRESULTS)$(CPP)main_fineCFDScan_FixTrig.cpp
$(BIN)fineCFDScan_FixTrig: $(fineCFDScan_FixTrig_cpp) $(getResults_dep)
	$(CC) $(LFLAGS) $(ROOT_LINKS) $(ROOT_LIBS) $(ROOT_INCLUDE) $^ -o $@



#================================================================================================
#================================================================================================

#========= jitter scan

$(BUILD)$(JITTER_SCAN)createDir.o: $(SRC)$(JITTER_SCAN)$(CPP)createDir.cpp
	$(CC) $(DEBUG) $(CFLAGS) $(ROOT_LINKS) $(ROOT_LIBS) $(ROOT_INCLUDE) $< -o $@
	
$(BUILD)$(JITTER_SCAN)jitterScan.o: $(SRC)$(JITTER_SCAN)$(CPP)jitterScan.cpp
	$(CC) $(DEBUG) $(CFLAGS) $(ROOT_LINKS) $(ROOT_LIBS) $(ROOT_INCLUDE) $< -o $@
	
$(BUILD)$(JITTER_SCAN)processingJitterScan.o: $(SRC)$(JITTER_SCAN)$(CPP)processingJitterScan.cpp
	$(CC) $(DEBUG) $(CFLAGS) $(ROOT_LINKS) $(ROOT_LIBS) $(ROOT_INCLUDE) $< -o $@
	
jitterScan_dep += $(BUILD)$(JITTER_SCAN)createDir.o
jitterScan_dep += $(BUILD)$(JITTER_SCAN)jitterScan.o
jitterScan_dep += $(BUILD)$(JITTER_SCAN)processingJitterScan.o

getJitterScan_cpp := $(SRC)$(JITTER_SCAN)$(CPP)main_getJitterScan.cpp
$(BIN)getJitterScan: $(getJitterScan_cpp) $(jitterScan_dep)
	$(CC) $(LFLAGS) $(ROOT_LINKS) $(ROOT_LIBS) $(ROOT_INCLUDE) $^ -o $@

	
#================================================================================================
#================================================================================================

#========= get Reslution at different edge threshold

$(BUILD)$(EDGE_RES)createDir.o: $(SRC)$(EDGE_RES)$(CPP)createDir.cpp
	$(CC) $(DEBUG) $(CFLAGS) $(ROOT_LINKS) $(ROOT_LIBS) $(ROOT_INCLUDE) $< -o $@
	
$(BUILD)$(EDGE_RES)processingThresholdScan.o: $(SRC)$(EDGE_RES)$(CPP)processingThresholdScan.cpp
	$(CC) $(DEBUG) $(CFLAGS) $(ROOT_LINKS) $(ROOT_LIBS) $(ROOT_INCLUDE) $< -o $@
	
$(BUILD)$(EDGE_RES)thresholdResScan.o: $(SRC)$(EDGE_RES)$(CPP)thresholdResScan.cpp
	$(CC) $(DEBUG) $(CFLAGS) $(ROOT_LINKS) $(ROOT_LIBS) $(ROOT_INCLUDE) $< -o $@
	
$(BUILD)$(EDGE_RES)resAtThreshold.o: $(SRC)$(EDGE_RES)$(CPP)resAtThreshold.cpp
	$(CC) $(DEBUG) $(CFLAGS) $(ROOT_LINKS) $(ROOT_LIBS) $(ROOT_INCLUDE) $< -o $@
	
getEdgeResScan_dep += $(BUILD)$(EDGE_RES)createDir.o
getEdgeResScan_dep += $(BUILD)$(EDGE_RES)processingThresholdScan.o
getEdgeResScan_dep += $(BUILD)$(EDGE_RES)thresholdResScan.o
getEdgeResScan_dep += $(BUILD)$(EDGE_RES)resAtThreshold.o

getEdgeResScan_cpp := $(SRC)$(EDGE_RES)$(CPP)main_getEdgeResScan.cpp
$(BIN)getEdgeResScan: $(getEdgeResScan_cpp) $(getEdgeResScan_dep)
	$(CC) $(LFLAGS) $(ROOT_LINKS) $(ROOT_LIBS) $(ROOT_INCLUDE) $^ -o $@

#================================================================================================
#================================================================================================

#========= get results with roofit

$(BIN)RooGetResults: $(SRC)$(ROOGETRESULTS)$(CPP)GetResults_Roo.cpp 
	$(CC) $(LFLAGS) $(ROOT_LINKS) $(ROOT_LIBS) $(ROOT_INCLUDE) $(ROOFIT) $^ -o $@



#================================================================================================
#================================================================================================

#========= get pulse shapes ==========

$(BIN)PulseShape: $(SRC)$(AVE_PULSE)$(CPP)main_PulseShape.cpp 
	$(CC) $(LFLAGS) $(ROOT_LINKS) $(ROOT_LIBS) $(ROOT_INCLUDE) $^ -o $@	

		
#clean:
#	rm -f *.o



