#ifndef ploty_v718
#define ploty_v718


class plotty
{
	TH1D* hist_array[256];
	int hist_num = 0;
	TLegend* leg;
	
	public:
	
		void legend(const char* header = "HPK 50D 1e15 neq/cm^2 at -30C")
		{
			leg = new TLegend(0.2,0.2,0.4,0.4);
			//gStyle->SetLegendBorderSize(0);
			//gStyle->SetLegendFillColor(0);
			leg->SetTextSize(0.032);
			leg->SetTextFont(42);
			leg->SetHeader(header);
			leg->SetTextSize(0.032);
			//return leg;
		}

		void add(TH1D * hist, const char* name)
		{
			leg->AddEntry(hist, name);
			hist_array[hist_num] = hist;
			hist_num += 1;
		}


		void plotting(int hist_num)
		{
			TCanvas *c = new TCanvas();
			c->cd();
	
			for(int i = 0; i < hist_num; i++)
			{
				if(i==0) 
				{
					hist_array[i]->Draw("C");
				}
				else
				{
					hist_array[i]->Draw("same");
				}
			}
		}

		void plot_pulse()
		{
			plotting(hist_num);
			leg->Draw("same");
		}

};

#endif











