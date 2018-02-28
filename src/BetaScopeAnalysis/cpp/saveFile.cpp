#include "../hpp/betaAnalysis.h"

void betaAnalysis::saveFile()
{
	TThread::Lock();
	this->oFile->Write();
	this->oFile->Close();
	TThread::UnLock();
}

