//c++
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <omp.h> 
#include <utility>      // std::pair, std::make_pair

//root cern
#include "TApplication.h"
#include "TROOT.h"
#include "TStopwatch.h"
#include "TFile.h"
#include "TTree.h"
#include "TThread.h"
#include "TSystem.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "TGraph.h"

//my
#include "EventMainCh.h"
//#include "TreeRaw.h"
//#include "CalcData.h"
//#include "TreeInfo.h"
//#include "TreeRaw.h"

using namespace std;

#define COUT(x) cout << #x " = " << x << endl;

//for more information
//https ://root.cern.ch/root/html/tutorials/tree/tree4.C.html

int main(int argc, char *argv[])
{
	TApplication theApp("theApp", &argc, argv);//let's add some magic! https://root.cern.ch/phpBB3/viewtopic.php?f=3&t=22972
	
	
	vector<unsigned short> ch_list = { 1, 2, 3, 4 };
	const double ns_per_point = 4;
	//map <unsigned short, unsigned short> ;

	gSystem->Load("libTree");// (to fix: no dictionary for class ttree is available) https://root.cern.ch/root/roottalk/roottalk04/1580.html
	
	string date = "190606";
	string subfolder_name = "f1_th80mV";
	
	string file_name_tree = "E:\\" + date + "\\" + date + "_caen_trees\\" + subfolder_name + ".root";
	TFile *f = new TFile(file_name_tree.c_str());
	if (!(f->IsOpen()))
	{
		cout << "could not open file: " << file_name_tree << endl;
		system("pause");
		exit(1);
	}
	else
	{
		cout << "File has been opened correctly: " << file_name_tree << endl;
	}


	TTree *tree = (TTree*)f->Get("TreeMain");

	EventMainCh *event = new EventMainCh(/*ch_list.size()*/);
	TBranch *branch = tree->GetBranch("EventMainCh");
	branch->SetAddress(&event);
	//tree->SetBranchAddress("EventMainCh", &event);

	//define hists
	vector<TH1F*> hist_ymin_v(ch_list.size(), NULL);
	vector<TH1F*> hist_ymax_v(ch_list.size(), NULL);
	vector<TH1F*> hist_baseline_mean_v(ch_list.size(), NULL);
	vector<TH1F*> hist_baseline_sigma_v(ch_list.size(), NULL);
	vector<TH1F*> hist_n_peaks_v(ch_list.size(), NULL);	
	vector<TH1F*> hist_peak_time_v(ch_list.size(), NULL);
	vector<TH1F*> hist_peak_amp_v(ch_list.size(), NULL);
	vector<TH1F*> hist_peak_area_v(ch_list.size(), NULL);
	for (int ch = 0; ch < ch_list.size(); ch++)
	{
		ostringstream hist_ymin_name;
		ostringstream hist_ymax_name;
		ostringstream hist_baseline_mean_name;
		ostringstream hist_baseline_sigma_name;
		ostringstream hist_n_peaks_name;
		ostringstream hist_peak_time_name;
		ostringstream hist_peak_amp_name;
		ostringstream hist_peak_area_name;
		//ostringstream gr_time_spectrum_name;
		
		hist_ymin_name << "hist_ymin_ch_" << ch_list[ch];
		hist_ymax_name << "hist_ymax_ch_" << ch_list[ch];
		hist_baseline_mean_name << "hist_baseline_mean_ch_" << ch_list[ch];
		hist_baseline_sigma_name << "hist_baseline_sigma_ch_" << ch_list[ch];
		hist_n_peaks_name << "hist_n_peaks_ch_" << ch_list[ch];
		hist_peak_time_name << "hist_peak_time_ch_" << ch_list[ch];
		hist_peak_amp_name << "hist_peak_amp_ch_" << ch_list[ch];
		hist_peak_area_name << "hist_peak_area_ch_" << ch_list[ch];
		//gr_time_spectrum_name << "gr_time_spectrum_ch_" << ch_list[ch];

		hist_ymin_v[ch] = new TH1F(hist_ymin_name.str().c_str(), hist_ymin_name.str().c_str(), 1000, -1100, 1100);
		hist_ymax_v[ch] = new TH1F(hist_ymax_name.str().c_str(), hist_ymax_name.str().c_str(), 1000, -1100, 1100);
		hist_baseline_mean_v[ch] = new TH1F(hist_baseline_mean_name.str().c_str(), hist_baseline_mean_name.str().c_str(), 1000, -1100, 1100);
		hist_baseline_sigma_v[ch] = new TH1F(hist_baseline_sigma_name.str().c_str(), hist_baseline_sigma_name.str().c_str(), 1000, 0, 100);
		hist_n_peaks_v[ch] = new TH1F(hist_n_peaks_name.str().c_str(), hist_n_peaks_name.str().c_str(), 100, 0, 100);
		hist_peak_time_v[ch] = new TH1F(hist_peak_time_name.str().c_str(), hist_peak_time_name.str().c_str(), 160, 0, 160000);
		hist_peak_amp_v[ch] = new TH1F(hist_peak_amp_name.str().c_str(), hist_peak_amp_name.str().c_str(), 110, 0, 2000);
		hist_peak_area_v[ch] = new TH1F(hist_peak_area_name.str().c_str(), hist_peak_area_name.str().c_str(), 2000, -10000, 500000);
		
	}

	//define graphs
	//vector<TGraph*> gr_time_spectrum_v(ch_list.size(), NULL);
	//vector< vector<double> > peak_time_v(ch_list.size());
	//vector< vector<double> > peak_amp_v(ch_list.size());
	//vector< vector<double> > peak_area_v(ch_list.size());

	COUT( tree->GetEntries() )
	//ev loop
	for (unsigned int ev = 0; ev < tree->GetEntries()/*300*/; ev++)
	{
		//read branch "EventMainCh"only
		if (ev % 1000 == 0)
			cout << "ev = " << ev << endl;
		
		branch->GetEntry(ev);

		vector<Peaks*> peaks = event->peaks;
		//cout << "\t" << "peaks.size() = " << peaks.size() << endl;

		//cout << "ev = " << ev << endl;

		//ch loop
		for (unsigned int ch = 0; ch < ch_list.size(); ch++)
		{
			//cout << "\t" << "ch_id = " << ch_list[ch] << endl;
			//cout << "\t" << "ch = " << ch_list[ch] << endl;
			//cout << "\t \t" << "ymin = " << event->ymin[ch] << endl;

			hist_ymin_v[ch]->Fill(event->ymin[ch]);
			hist_ymax_v[ch]->Fill(event->ymax[ch]);
			hist_baseline_mean_v[ch]->Fill(event->baseline_mean[ch]);
			hist_baseline_sigma_v[ch]->Fill(event->baseline_sigma[ch]);
			hist_n_peaks_v[ch]->Fill(event->peaks[ch]->peak_time.size());
			
			for (unsigned int peak_id = 0; peak_id < event->peaks[ch]->peak_time.size(); peak_id++)
			{
			//	peak_time_v[ch].push_back( event->peaks[ch]->peak_time[peak_id] );
			//	peak_amp_v[ch].push_back( event->peaks[ch]->peak_amp[peak_id] );

				//cuts
				bool is_noise = (event->peaks[ch]->avr_peak_time[peak_id] > 31000 && event->peaks[ch]->avr_peak_time[peak_id] < 34000);
				bool is_in_34_100_us = event->peaks[ch]->peak_time[peak_id] > 34000 && event->peaks[ch]->peak_time[peak_id] < 100000;
				bool is_in_70_100_us = event->peaks[ch]->peak_time[peak_id] > 70000 && event->peaks[ch]->peak_time[peak_id] < 100000;
				bool is_in_110_155_us = event->peaks[ch]->peak_time[peak_id] > 110000 && event->peaks[ch]->peak_time[peak_id] < 155000;
				if (!is_noise && is_in_34_100_us)
				{
					hist_peak_time_v[ch]->Fill(event->peaks[ch]->peak_time[peak_id]);
					hist_peak_amp_v[ch]->Fill(event->peaks[ch]->peak_amp[peak_id]);
					hist_peak_area_v[ch]->Fill(event->peaks[ch]->peak_area[peak_id]);
				}
				
			}
			
			//gr_time_spectrum_v[ch] = new TGraph();
			
			//test
			//vector< pair<int, int> > pair_vec = peaks[ch]->peak_start_stop_poits;
			//vector<double> local_baseline = peaks[ch]->local_baseline_v;
			//vector<double> avr_peak_time = peaks[ch]->avr_peak_time;
			//vector<double> peak_time = peaks[ch]->peak_time;
			//vector<double> peak_area = peaks[ch]->peak_area;
			////cout peak characterictics
			//cout << "\t " << "N_peaks = " << pair_vec.size() << endl;
			////cout << "\t " << "N_peaks = " << pair_vec.size() << endl;
			//for (int j = 0; j < pair_vec.size(); j++)
			//{
			//	cout << "\t \t" << "peak number = " << j << endl;
			//	cout << "\t \t" << "local_baseline = " << local_baseline[j] << endl;
			//	cout << "\t \t" << "avr_peak_time = " << avr_peak_time[j] << endl;
			//	cout << "\t \t" << "peak_time = " << peak_time[j] << endl;
			//	cout << "\t \t" << "peak_area = " << peak_area[j] << endl;
			//	cout << endl;
			//}


			//if (ch_list[ch] == 1)//chose channel "1"
			//{
			//	//hist_ymin->Fill(event->ymin[ch]);
			//	//hist_ymax->Fill(event->ymax[ch]);
			//	//hist_baseline_mean->Fill(event->baseline[ch]);

			//	////cout << "ev = " << ev << "; event->baseline[" << ch << "] = " << event->baseline[ch] << endl;
			//}

			//
			//if (ev == 0 && peaks.size() > 0)
			//{

			//}
			
		}
		

		event->Clear();
	}

	string draw_var = "ymin ymax baseline_mean baseline_sigma peak_time peak_amp peak_area n_peaks";
	//string draw_var = "peak_time";

	//draw
	if (draw_var.find("ymin") != std::string::npos)
	{
		TCanvas *c1 = new TCanvas("c1", "ymin");
		c1->Divide(2, 2, 0.01, 0.01);
		c1->cd(1);
		hist_ymin_v[0]->Draw();
		c1->cd(2);
		hist_ymin_v[1]->Draw();
		c1->cd(3);
		hist_ymin_v[2]->Draw();
		c1->cd(4);
		hist_ymin_v[3]->Draw();
	}

	if (draw_var.find("ymax") != std::string::npos)
	{
		TCanvas *c2 = new TCanvas("c2", "ymax");
		c2->Divide(2, 2, 0.01, 0.01);
		c2->cd(1);
		hist_ymax_v[0]->Draw();
		c2->cd(2);
		hist_ymax_v[1]->Draw();
		c2->cd(3);
		hist_ymax_v[2]->Draw();
		c2->cd(4);
		hist_ymax_v[3]->Draw();
	}

	if (draw_var.find("baseline_mean") != std::string::npos)
	{
		TCanvas *c3 = new TCanvas("c3", "baseline_mean");
		c3->Divide(2, 2, 0.01, 0.01);
		c3->cd(1);
		hist_baseline_mean_v[0]->Draw();
		c3->cd(2);
		hist_baseline_mean_v[1]->Draw();
		c3->cd(3);
		hist_baseline_mean_v[2]->Draw();
		c3->cd(4);
		hist_baseline_mean_v[3]->Draw();
	}

	if (draw_var.find("baseline_sigma") != std::string::npos)
	{ 
		TCanvas *c4 = new TCanvas("c4", "baseline_sigma");
		c4->Divide(2, 2, 0.01, 0.01);
		c4->cd(1);
		hist_baseline_sigma_v[0]->Draw();
		c4->cd(2);
		hist_baseline_sigma_v[1]->Draw();
		c4->cd(3);
		hist_baseline_sigma_v[2]->Draw();
		c4->cd(4);
		hist_baseline_sigma_v[3]->Draw();
	}

	if (draw_var.find("peak_time") != std::string::npos)
	{
		TCanvas *c5 = new TCanvas("c5", "peak_time");
		c5->Divide(2, 2, 0.01, 0.01);
		c5->cd(1);
		hist_peak_time_v[0]->Draw();
		c5->cd(2);
		hist_peak_time_v[1]->Draw();
		c5->cd(3);
		hist_peak_time_v[2]->Draw();
		c5->cd(4);
		hist_peak_time_v[3]->Draw();
	}

	if (draw_var.find("peak_amp") != std::string::npos)
	{
		TCanvas *c6 = new TCanvas("c6", "peak_amp");
		c6->Divide(2, 2, 0.01, 0.01);
		c6->cd(1);
		hist_peak_amp_v[0]->Draw();
		c6->cd(2);
		hist_peak_amp_v[1]->Draw();
		c6->cd(3);
		hist_peak_amp_v[2]->Draw();
		c6->cd(4);
		hist_peak_amp_v[3]->Draw();
	}

	if (draw_var.find("peak_area") != std::string::npos)
	{
		TCanvas *c7 = new TCanvas("c7", "peak_area");
		c7->Divide(2, 2, 0.01, 0.01);
		c7->cd(1);
		hist_peak_area_v[0]->Draw();
		c7->cd(2);
		hist_peak_area_v[1]->Draw();
		c7->cd(3);
		hist_peak_area_v[2]->Draw();
		c7->cd(4);
		hist_peak_area_v[3]->Draw();
	}

	if (draw_var.find("n_peaks") != std::string::npos)
	{
		TCanvas *c8 = new TCanvas("c8", "n_peaks");
		c8->Divide(2, 2, 0.01, 0.01);
		c8->cd(1);
		hist_n_peaks_v[0]->Draw();
		c8->cd(2);
		hist_n_peaks_v[1]->Draw();
		c8->cd(3);
		hist_n_peaks_v[2]->Draw();
		c8->cd(4);
		hist_n_peaks_v[3]->Draw();
	}

	cout << endl;
	cout << "all is ok" << endl;
	theApp.Run();
	system("pause");	
	return 0;
}