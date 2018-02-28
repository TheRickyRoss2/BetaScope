//======Set and Link Branches==========

void Roo_AutoGet::SetBranch ()
{
	printf(" \n");
	printf("Linking branches... \n");
	
	for(unsigned int i = 0; i < ch.size(); i++)
	{
		if (ch.at(i))
		{
			iTree->SetBranchAddress ( Form("w%d", ch.at(i)), &w[i] );
			iTree->SetBranchAddress ( Form("t%d", ch.at(i)), &t[i] );
			iTree->SetBranchAddress ( Form("pmax%d", ch.at(i)), &Pmax[i] );
			iTree->SetBranchAddress ( Form("tmax%d", ch.at(i)), &Tmax[i] );
			iTree->SetBranchAddress ( Form("rms%d", ch.at(i)), &Rms[i] );
			iTree->SetBranchAddress ( Form("rise%d_1090", ch.at(i)), &RiseTime[i] );
			iTree->SetBranchAddress ( Form("pulse_area%d", ch.at(i)), &PulseArea[i] );
			iTree->SetBranchAddress ( Form("cfd%d", ch.at(i)), &CFD[i] );
			iTree->SetBranchAddress ( Form("cfd%d_fall", ch.at(i)), &CFD_Fall[i] );
			iTree->SetBranchAddress ( Form("dvdt%d", ch.at(i)), &Dvdt[i] );
		}
	}
	
	printf("Branches are ready. \n");
	printf(" \n");
}
