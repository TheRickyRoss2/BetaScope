#include "../hpp/betaAnalysis.h"

void singleBetaRun( const char* ifile )
{
	betaAnalysis singleRun( ifile );
	//singleRun.set_parallelStatus( true );
	singleRun.set_iBranchType('v');
	betaAnalyzer( (void*) &singleRun );
}

int main( int argc, char **argv )
{
	if( argc != 2 ){std::cout << "invalid parameter" << std::endl; return 1; }
	else singleBetaRun( argv[1] );
	return 0;
}
