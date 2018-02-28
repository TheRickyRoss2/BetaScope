#include "../hpp/betaAnalysis.h"

void betaAnalysis::setBranch()
{
	for( unsigned int b = 0; b < this->channel.size(); b++ )
	{
		this->oTree->Branch( Form("w%d", this->channel.at(b)), &this->w[b] );
			
		this->oTree->Branch( Form("wbase%d", this->channel.at(b)), &wbase[b] ); 

		this->oTree->Branch( Form("t%d", this->channel.at(b)), &t[b] );

		this->oTree->Branch( Form("cfd%d", this->channel.at(b)), &cfd[b] );
		
		this->oTree->Branch( Form("cfd%d_fall", this->channel.at(b)), &cfd_fall[b] );

		this->oTree->Branch( Form("rms%d", this->channel.at(b)), &rms[b] );

		this->oTree->Branch( Form("pmax%d", this->channel.at(b)), &pmax[b] );

		this->oTree->Branch( Form("tmax%d", this->channel.at(b)), &tmax[b] );
		
		this->oTree->Branch( Form("multiplePmax%d", this->channel.at(b)), "vector<double>", &multiplePmax[b] );
			
		this->oTree->Branch( Form("multipleTmax%d", this->channel.at(b)), "vector<double>", &multipleTmax[b] );

		this->oTree->Branch( Form("pulse_area%d", this->channel.at(b)), &pulse_area_fill[b] );

		this->oTree->Branch( Form("rise%d_1090", this->channel.at(b)), &rise_1090[b] );

		this->oTree->Branch( Form("dvdt%d", this->channel.at(b)), &dvdt[b] );

		//this->oTree->Branch( Form("dvdt%d_fit", this->channel.at(b)), &dvdt_fit[b] );
			
		//this->oTree->Branch( Form("pmax%d_fit", this->channel.at(b)), &pmax_fit[b] );
			
		this->oTree->Branch( Form("thTime%d", this->channel.at(b)), &this->thTime[b] );
		
		this->oTree->Branch( Form("fineCFDRise%d", this->channel.at(b)), &this->fineCFDRise[b] );
		
		this->oTree->Branch( Form("singlePeak%d", this->channel.at(b)), "vector<int>", &this->singlePeak[b] );
		
		if( this->iBranchType == 'v' )
		{
			this->iTree->SetBranchAddress( Form("w%d", this->channel.at(b)), &this->wf_v[b] );
			this->iTree->SetBranchAddress( Form("t%d", this->channel.at(b)), &this->tf_v[b] );
			
			if ( b == 0 )
			{
				this->iTree->GetEntry(1);
				this->npoint = this->wf_v[b]->size();
				this->m_inoise = 0.25 * this->npoint;
				if ( this->npoint == 0 ){ std::cout << "there is no npoints, check your file" << std::endl; return void();}
			}
		}
		else
		{
			this->npoint = this->iTree->GetLeaf( this->sampleLeaf.c_str() )->GetLen();
			this->m_inoise = 0.25 * this->npoint;
			this->iTree->SetBranchAddress( Form("w%d", this->channel.at(b)), &this->wf_a[b] );
			this->iTree->SetBranchAddress( Form("t%d", this->channel.at(b)), &this->tf_a[b] );
		}
	}
}





