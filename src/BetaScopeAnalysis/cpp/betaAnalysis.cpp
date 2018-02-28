#include "../hpp/betaAnalysis.h"

betaAnalysis::betaAnalysis(const char* ipath)
{
	TThread::Lock();
	std::cout << std::string( 100, '*' ) << std::endl;
	std::cout << "calling the pre-set analysis" << std::endl;
	TThread::UnLock();
	
	this->path = ipath;
	
	this->iFile = new TFile( this->path );
	
	if( this->iFile->IsZombie() )
	{
		TThread::Lock();
		std::cout << this->path << " is bad" << std::endl;
		TThread::UnLock();
	}
	else
	{
		TThread::Lock();
		std::cout << this->path << " is good" << std::endl;
		TThread::UnLock();
	}
	
	std::string opath = ipath;
	this->ofileName = this->filePrefix += opath;
	this->ifileName = ipath;
	
	this->iTree = (TTree*) this->iFile->Get( this->iTreeName.c_str() );
	this->oFile = new TFile( this->ofileName.c_str(), "RECREATE", "", this->compressionLevel );
	this->oTree = new TTree( "wfm", "waveform analysis" );
	this->oTree->SetDirectory( this->oFile );
	
	this->numEvent = this->iTree->GetEntries();
	
	
	this->channel.push_back(2);
	this->invertPulse.push_back(1);
	
	this->channel.push_back(3);
	this->invertPulse.push_back(0);
	
	for( unsigned int i = 0; i < this->channel.size(); i++)
	{
		std::cout << "enable channel: " << this->channel.at(i) << " invert: " << this->invertPulse.at(i) << std::endl;
		this->cfd[i].reserve( 101 );
		this->cfd_fall[i].reserve( 101 );
		this->fineCFDRise[i].reserve( this->fineCFDStep );
		
		this->multiplePmax[i].reserve( this->lookForNumPeak );
		this->multipleTmax[i].reserve( this->lookForNumPeak );
		
		this->thTime[i].reserve( 50 );
		this->dvdt[i].reserve( 101 );
	}
}

betaAnalysis::betaAnalysis(const char* ipath, char iBranchType)
{
	TThread::Lock();
	std::cout << "setting up ifile and ofile" << std::endl;
	TThread::UnLock();
	
	this->path = ipath;
	
	this->iBranchType = iBranchType;
	
	this->iFile = new TFile( this->path );
	
	if( this->iFile->IsZombie() )
	{
		TThread::Lock();
		std::cout << this->path << " is bad" << std::endl;
		TThread::UnLock();
	}
	else
	{
		TThread::Lock();
		std::cout << this->path << " is good" << std::endl;
		TThread::UnLock();
	}
	
	std::string opath = ipath;
	this->ofileName = this->filePrefix += opath;
	
	this->iTree = (TTree*) this->iFile->Get( this->iTreeName.c_str() );
	this->oFile = new TFile( this->ofileName.c_str(), "RECREATE", "", this->compressionLevel );
	this->oTree = new TTree( "wfm", "waveform analysis" );
	this->oTree->SetDirectory( this->oFile );
}

