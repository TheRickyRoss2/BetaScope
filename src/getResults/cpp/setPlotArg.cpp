#include "../hpp/getResults.h"

void getData::setPlotArg()
{
	std::vector<int> activeChannel;

	activeChannel.push_back( this->DUT+1 );
	activeChannel.push_back( this->Trigger+1 );

	for( unsigned int i = 0; i < activeChannel.size(); i++ )
	{
		int ch = activeChannel.at(i);

		plotArg[i].push_back( {
		                     { "PulseArea" },
		                     { Form( "pulse_area%d[0]/1E-15", ch ) },
		                     { "landau" }
		                                  } );

		plotArg[i].push_back( {
		                     { "Pmax" },
		                     { Form( "pmax%d[0]", ch ) },
		                     { "landau" }
		                                  } );

		plotArg[i].push_back( {
		                     { "RMS" },
		                     { Form( "rms%d[0]", ch ) },
		                     { "gaus" }
		                                  } );

        plotArg[i].push_back( {
		                     { "Rise Time" },
		                     { Form( "rise%d_1090[0]", ch ) },
		                     { "gaus" }
		                                  } );

		plotArg[i].push_back( {
		                     { "dvdt" },
		                     { Form( "dvdt%d[20]", ch ) },
		                     { "landau" }
		                                  } );

		plotArg[i].push_back( {
		                     { "FWHM" },
		                     { Form( "cfd%d_fall[50] - cfd%d[50]", ch, ch ) },
		                     { "gaus" }
		                                  } );
	}
}
