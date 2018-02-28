#include "../hpp/betaAnalysis.h"

void *betaAnalyzer( void *iarg )
{
	betaAnalysis *inputArg = (betaAnalysis*) iarg;
	
	inputArg->set_objectLocation("betaAnalyzer");
	//std::cout<< "get_objectLocation " << inputArg->get_objectLocation() << std::endl;
	
	TThread::Lock();
	std::cout << std::string( 100, '=' ) << std::endl;
	std::cout << "processing " << inputArg->get_ifileName() << std::endl;
	std::cout << "setting branches " << std::endl;
	TThread::UnLock();

	inputArg->setBranch();
	
	TThread::Lock();
	std::cout << "Number of Events = " << inputArg->get_numEvent() << std::endl;
	std::cout << "Number of points = " << inputArg->get_npoint() << std::endl;
	std::cout << "start filling data " << std::endl; 
	TThread::UnLock();

	inputArg->fillData();
	
	TThread::Lock();
	std::cout << "done. saving file..." <<std::endl;
	TThread::UnLock();
	
	inputArg->saveFile();
	
	TThread::Lock();
	std::cout << "Export to " << inputArg->get_ofileName() <<std::endl;
	TThread::UnLock();
	
	std::cout << std::string( 100, '=' ) << std::endl;
}

void parallelBetaAnalyzer( std::vector<betaAnalysis> argVector )
{	
	TStopwatch StopWatch;
	
	StopWatch.Start();	
	
	TThread::Initialize();
	
	ROOT::EnableThreadSafety();
	
	int size = argVector.size();
	
	TThread *thread_array [size];
		
	for (int loop = 0; loop < size; loop++)
	{
		thread_array [loop] = new TThread ( "", betaAnalyzer, (void*) &argVector.at(loop) );
	}
	
	for (int loop = 0; loop < size; loop++)
	{
		thread_array [loop] -> Initialize();
	}
		
	for (int loop = 0; loop < size; loop++)
	{
		thread_array [loop] -> Run();
	}
	
	TThread::Ps();
	
	for (int loop = 0; loop < size; loop++)
	{
		thread_array [loop] -> Join();
	}
	
	for (int loop = 0; loop < size; loop++)
	{
		delete thread_array [loop];
		//thread_array[loop]->Delete();
	}
		
	TThread::Ps();
	StopWatch.Stop();
	
	std::cout << " Time : " << StopWatch.RealTime() << "sec " << std::endl;
}





