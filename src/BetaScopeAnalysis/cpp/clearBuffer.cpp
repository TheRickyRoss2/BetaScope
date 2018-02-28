#include "../hpp/betaAnalysis.h"

void betaAnalysis::clearBuffer()
{
	for(unsigned int clean = 0; clean < this->channel.size(); clean++)
	{        
		if( this->channel.at(clean) )
		{       
			this->w[clean].clear();
				
			this->t[clean].clear();
				
			this->pmax[clean].clear();
				
			//this->pmax_fit[clean].clear();
				
			this->tmax[clean].clear();
			
			this->multiplePmax[clean].clear();
				
			this->multipleTmax[clean].clear();
				
			this->cfd[clean].clear();
				
			this->cfd_fall[clean].clear();
				
			this->rms[clean].clear();
				
			this->pulse_area_fill[clean].clear();
			
			this->rise_1090[clean].clear();
				
			this->dvdt[clean].clear();
				
			//this->dvdt_fit[clean].clear();
				
			this->wbase[clean].clear();
				
			this->thTime[clean].clear();
			
			this->fineCFDRise[clean].clear();
			
			this->singlePeak[clean].clear();
		}
	}
}


