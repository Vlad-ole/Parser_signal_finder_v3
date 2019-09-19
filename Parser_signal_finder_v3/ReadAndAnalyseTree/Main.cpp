//c++
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <omp.h> 
#include <utility>      // std::pair, std::make_pair
#include <algorithm>

//root cern
#include "TApplication.h"
#include "TROOT.h"
#include "TStopwatch.h"
#include "TFile.h"
#include "TTree.h"
#include "TThread.h"
#include "TSystem.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TColor.h"
#include "TStyle.h"

//my
#include "EventMainCh.h"
#include "ReadInfo.h"
#include "ReadDaqInfo.h"
#include "ChMapping.h"
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

	string draw_var = "ymin ymax baseline_mean baseline_sigma peak_time peak_amp peak_area n_peaks peak_amp_vs_peak_area"; //view 4 ch
	//string draw_var = "n_peaks_map"; //view good_SiPMs
	//string draw_var = "peak_time peak_amp peak_area n_peaks n_peaks_ch1_ch2";
	//vector<int> ch_list_to_view = { 32, 33, 34, 35 };
	//vector<int> ch_list_to_view = {36, 37, 38, 39};
	//vector<int> ch_list_to_view = { 40, 41, 42, 43 };
	//vector<int> ch_list_to_view = { 44, 48, 49, 50 };
	//vector<int> ch_list_to_view = { 51, 52, 53, 54 };
	//vector<int> ch_list_to_view = { 55, 56, 57, 58 };
	//vector<int> ch_list_to_view = { 38, 43, 44, 59 };
	//vector<int> ch_list_good_SiPMs = { 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59 };
	vector<int> ch_list_good_SiPMs = { 1, 2, 3, 4 };
	vector<int> ch_list_to_view = { 1, 2, 3, 4 };

	/*int hist_peak_area_nbins = 1000;
	int hist_peak_area_xmin = -5000;
	int hist_peak_area_xmax = 500000;*/

	int hist_peak_area_nbins = 800;
	int hist_peak_area_xmin = -100;
	int hist_peak_area_xmax = /*35000*/ 2000;

	int hist_baseline_sigma_xmax = 50;

	int hist_peak_amp_nbins = 300;
	int hist_peak_amp_xmin = 0;
	int hist_peak_amp_xmax = /*200*/400;

	int hist_n_peaks_nbins = 40;
	int hist_n_peaks_xmax = 40;

	//vector<unsigned short> ch_list = { 1, 2, 3, 4 };
	//const double ns_per_point = 4;
	//map <unsigned short, unsigned short> ;

	gSystem->Load("libTree");// (to fix: no dictionary for class ttree is available) https://root.cern.ch/root/roottalk/roottalk04/1580.html

	//double peak_finder_th = 80;
	//string date = "190704";
	////string subfolder_name = "f1_th80mV";
	//ostringstream subfolder_name;
	////subfolder_name << "f6_th" << peak_finder_th << "mV";
	//subfolder_name << "f2" ;


	//in
	string date = "190718";
	string subfolder_name = "f2";
	string file_name_info = "E:\\" + date + "\\" + date + "_caen_raw\\info\\" + subfolder_name + "_info.txt";
	string file_name_daq_info = "E:\\" + date + "\\" + date + "_caen_raw\\info\\daq_info.txt";

	ReadDAQInfo rd_daq_inf(file_name_daq_info);
	rd_daq_inf.Read();
	ReadInfo rd_inf(file_name_info);
	rd_inf.Read();
	vector<int> ch_list = rd_inf.GetChList(); //all ch
	double ns_per_point = rd_daq_inf.GetNsPerPoint();
	ChMapping ch_map(ch_list, ch_list_to_view);
	vector<int> ch_index_for_view_list = ch_map.GetChIndexList();

	string file_name_tree = "E:\\" + date + "\\" + date + "_caen_trees\\" + subfolder_name + "_info_v1.root";
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

	//out
	string f_out_name = "E:\\" + date + "\\" + date + "_caen_trees\\" + subfolder_name + "_out.txt";
	ofstream f_out(f_out_name);



	TTree *tree = (TTree*)f->Get("TreeMain");

	EventMainCh *event = new EventMainCh(/*ch_list.size()*/);
	TBranch *branch = tree->GetBranch("EventMainCh");
	branch->SetAddress(&event);
	//tree->SetBranchAddress("EventMainCh", &event);

	//define hists (joined ch)
	TH1F* hist_peak_time_good_SiPMs = new TH1F("peak_time_good_SiPMs", "peak_time_good_SiPMs", 4000, 0, 160E3);
	vector<double> vec_peak_time_good_SiPMs;
	//TH2F *hist2_peak_time_SiPM_good = new TH2F("hist2_peak_time_SiPM_good", "hist2_peak_time_SiPM_good", /*4000*/4000, 0, 160E3, 1000, 0, 50000);
	TH2F* h2_n_peaks = new TH2F("h2_n_peaks", "h2_n_peaks", 5, -23, 23, 5, -23, 23);

	//define hists (individual ch)
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
		hist_baseline_sigma_v[ch] = new TH1F(hist_baseline_sigma_name.str().c_str(), hist_baseline_sigma_name.str().c_str(), 1000, 0, hist_baseline_sigma_xmax);
		hist_n_peaks_v[ch] = new TH1F(hist_n_peaks_name.str().c_str(), hist_n_peaks_name.str().c_str(), hist_n_peaks_nbins, 0, hist_n_peaks_xmax);
		hist_peak_time_v[ch] = new TH1F(hist_peak_time_name.str().c_str(), hist_peak_time_name.str().c_str(), 160, 0, 160000);
		hist_peak_amp_v[ch] = new TH1F(hist_peak_amp_name.str().c_str(), hist_peak_amp_name.str().c_str(), hist_peak_amp_nbins, hist_peak_amp_xmin, hist_peak_amp_xmax);
		hist_peak_area_v[ch] = new TH1F(hist_peak_area_name.str().c_str(), hist_peak_area_name.str().c_str(), hist_peak_area_nbins, hist_peak_area_xmin, hist_peak_area_xmax);

	}

	//define graphs
	//vector<TGraph*> gr_time_spectrum_v(ch_list.size(), NULL);
	//vector< vector<double> > peak_time_v(ch_list.size());
	vector< vector<double> > peak_amp_v(ch_list.size());
	vector< vector<double> > peak_area_v(ch_list.size());
	//vector<double> peak_time_SiPM_good;

	COUT(tree->GetEntries());


	int n_ev_after_cuts = 0;

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

		/*if (ch_list.size() != 35)
		{
		cout << "err" << endl;
		system("pause");
		}*/

		bool cut_1 = event->peaks[1]->peak_time.size() > 0 && event->peaks[2]->peak_time.size() > 0 &&
			event->peaks[3]->peak_time.size() > 0 && event->peaks[4]->peak_time.size() > 0;

		bool cut_2 = event->peaks[1]->peak_time.size() > 0 || event->peaks[2]->peak_time.size() > 0 ||
			event->peaks[3]->peak_time.size() > 0 || event->peaks[4]->peak_time.size() > 0;

		if (cut_2)//cuts
		{
			n_ev_after_cuts++;
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
					bool is_in_60_155_us = event->peaks[ch]->peak_time[peak_id] > 60000 && event->peaks[ch]->peak_time[peak_id] < 155000;
					if (/*!is_noise && is_in_110_155_us*/ /*is_in_60_155_us*/ true)
					{
						hist_peak_time_v[ch]->Fill(event->peaks[ch]->peak_time[peak_id]);
						hist_peak_amp_v[ch]->Fill(event->peaks[ch]->peak_amp[peak_id]);
						hist_peak_area_v[ch]->Fill(event->peaks[ch]->peak_area[peak_id]);

						peak_amp_v[ch].push_back(event->peaks[ch]->peak_amp[peak_id]);
						peak_area_v[ch].push_back(event->peaks[ch]->peak_area[peak_id]);

						if (find(ch_list_good_SiPMs.begin(), ch_list_good_SiPMs.end(), ch_list[ch]) != ch_list_good_SiPMs.end())
						{
							hist_peak_time_good_SiPMs->Fill(event->peaks[ch]->peak_time[peak_id]);
							vec_peak_time_good_SiPMs.push_back(event->peaks[ch]->peak_time[peak_id]);
							f_out << (event->peaks[ch]->peak_time[peak_id]) / 1000.0 << endl;
							//cout << "ch = " << ch_list[ch]  << endl;
						}


						//cout << "ev = " << ev << "; ch = " << ch_list[ch] << "; peak_id = " << peak_id << "; peak_area = " << event->peaks[ch]->peak_area[peak_id] << endl;
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


		}
		event->Clear();

	}

	cout << endl << "n_ev_after_cuts = " << n_ev_after_cuts << endl;

	//draw
	if (draw_var.find("ymin") != std::string::npos)
	{
		TCanvas *c1 = new TCanvas("c1", "ymin");
		c1->Divide(2, 2, 0.01, 0.01);
		c1->cd(1);
		gPad->SetLogy();
		hist_ymin_v[ch_index_for_view_list[0]]->Draw();
		c1->cd(2);
		gPad->SetLogy();
		hist_ymin_v[ch_index_for_view_list[1]]->Draw();
		c1->cd(3);
		gPad->SetLogy();
		hist_ymin_v[ch_index_for_view_list[2]]->Draw();
		c1->cd(4);
		gPad->SetLogy();
		hist_ymin_v[ch_index_for_view_list[3]]->Draw();
	}

	if (draw_var.find("ymax") != std::string::npos)
	{
		TCanvas *c2 = new TCanvas("c2", "ymax");
		c2->Divide(2, 2, 0.01, 0.01);
		c2->cd(1);
		gPad->SetLogy();
		hist_ymax_v[ch_index_for_view_list[0]]->Draw();
		c2->cd(2);
		gPad->SetLogy();
		hist_ymax_v[ch_index_for_view_list[1]]->Draw();
		c2->cd(3);
		gPad->SetLogy();
		hist_ymax_v[ch_index_for_view_list[2]]->Draw();
		c2->cd(4);
		gPad->SetLogy();
		hist_ymax_v[ch_index_for_view_list[3]]->Draw();
	}

	if (draw_var.find("baseline_mean") != std::string::npos)
	{
		TCanvas *c3 = new TCanvas("c3", "baseline_mean");
		c3->Divide(2, 2, 0.01, 0.01);
		c3->cd(1);
		gPad->SetLogy();
		hist_baseline_mean_v[ch_index_for_view_list[0]]->Draw();
		c3->cd(2);
		gPad->SetLogy();
		hist_baseline_mean_v[ch_index_for_view_list[1]]->Draw();
		c3->cd(3);
		gPad->SetLogy();
		hist_baseline_mean_v[ch_index_for_view_list[2]]->Draw();
		c3->cd(4);
		gPad->SetLogy();
		hist_baseline_mean_v[ch_index_for_view_list[3]]->Draw();
	}

	if (draw_var.find("baseline_sigma") != std::string::npos)
	{
		TCanvas *c4 = new TCanvas("c4", "baseline_sigma");
		c4->Divide(2, 2, 0.01, 0.01);
		c4->cd(1);
		gPad->SetLogy();
		hist_baseline_sigma_v[ch_index_for_view_list[0]]->Draw();
		c4->cd(2);
		gPad->SetLogy();
		hist_baseline_sigma_v[ch_index_for_view_list[1]]->Draw();
		c4->cd(3);
		gPad->SetLogy();
		hist_baseline_sigma_v[ch_index_for_view_list[2]]->Draw();
		c4->cd(4);
		gPad->SetLogy();
		hist_baseline_sigma_v[ch_index_for_view_list[3]]->Draw();
	}

	if (draw_var.find("peak_time") != std::string::npos)
	{
		TCanvas *c5 = new TCanvas("c5", "peak_time");
		c5->Divide(2, 2, 0.01, 0.01);
		c5->cd(1);
		gPad->SetLogy();
		hist_peak_time_v[ch_index_for_view_list[0]]->Draw();
		c5->cd(2);
		gPad->SetLogy();
		hist_peak_time_v[ch_index_for_view_list[1]]->Draw();
		c5->cd(3);
		gPad->SetLogy();
		hist_peak_time_v[ch_index_for_view_list[2]]->Draw();
		c5->cd(4);
		gPad->SetLogy();
		hist_peak_time_v[ch_index_for_view_list[3]]->Draw();
	}

	if (draw_var.find("peak_amp") != std::string::npos)
	{
		TCanvas *c6 = new TCanvas("c6", "peak_amp");
		c6->Divide(2, 2, 0.01, 0.01);
		c6->cd(1);
		hist_peak_amp_v[ch_index_for_view_list[0]]->Draw();
		c6->cd(2);
		hist_peak_amp_v[ch_index_for_view_list[1]]->Draw();
		c6->cd(3);
		hist_peak_amp_v[ch_index_for_view_list[2]]->Draw();
		c6->cd(4);
		hist_peak_amp_v[ch_index_for_view_list[3]]->Draw();
	}

	if (draw_var.find("peak_area") != std::string::npos)
	{
		TCanvas *c7 = new TCanvas("c7", "peak_area");
		c7->Divide(2, 2, 0.01, 0.01);
		c7->cd(1);
		hist_peak_area_v[ch_index_for_view_list[0]]->Draw();
		c7->cd(2);
		hist_peak_area_v[ch_index_for_view_list[1]]->Draw();
		c7->cd(3);
		hist_peak_area_v[ch_index_for_view_list[2]]->Draw();
		c7->cd(4);
		hist_peak_area_v[ch_index_for_view_list[3]]->Draw();
	}

	if (draw_var.find("n_peaks") != std::string::npos)
	{
		TCanvas *c8 = new TCanvas("c8", "n_peaks");
		c8->Divide(2, 2, 0.01, 0.01);
		c8->cd(1);
		hist_n_peaks_v[ch_index_for_view_list[0]]->Draw();
		gPad->SetLogy();
		c8->cd(2);
		hist_n_peaks_v[ch_index_for_view_list[1]]->Draw();
		gPad->SetLogy();
		c8->cd(3);
		hist_n_peaks_v[ch_index_for_view_list[2]]->Draw();
		gPad->SetLogy();
		c8->cd(4);
		hist_n_peaks_v[ch_index_for_view_list[3]]->Draw();
		gPad->SetLogy();
	}

	if (draw_var.find("peak_amp_vs_peak_area") != std::string::npos)
	{
		TCanvas *c9 = new TCanvas("c9", "peak_amp_vs_peak_area");
		vector<TGraph*> gr_peak_amp_peak_area(ch_list.size());
		c9->Divide(2, 2, 0.01, 0.01);
		c9->cd(1);
		gr_peak_amp_peak_area[ch_index_for_view_list[0]] = new TGraph(peak_amp_v[ch_index_for_view_list[0]].size(), &peak_amp_v[ch_index_for_view_list[0]][0], &peak_area_v[ch_index_for_view_list[0]][0]);
		gr_peak_amp_peak_area[ch_index_for_view_list[0]]->Draw("AP");
		//gr_peak_amp_peak_area[ch_index_for_view_list[0]]->SetMarkerStyle(20);
		//gr_peak_amp_peak_area[ch_index_for_view_list[0]]->GetXaxis()->SetRangeUser(hist_peak_amp_xmin, hist_peak_amp_xmax);
		//gr_peak_amp_peak_area[ch_index_for_view_list[0]]->GetYaxis()->SetRangeUser(hist_peak_area_xmin, hist_peak_area_xmax);
		c9->cd(2);
		gr_peak_amp_peak_area[ch_index_for_view_list[1]] = new TGraph(peak_amp_v[ch_index_for_view_list[1]].size(), &peak_amp_v[ch_index_for_view_list[1]][0], &peak_area_v[ch_index_for_view_list[1]][0]);
		gr_peak_amp_peak_area[ch_index_for_view_list[1]]->Draw("AP");
		c9->cd(3);
		gr_peak_amp_peak_area[ch_index_for_view_list[2]] = new TGraph(peak_amp_v[ch_index_for_view_list[2]].size(), &peak_amp_v[ch_index_for_view_list[2]][0], &peak_area_v[ch_index_for_view_list[2]][0]);
		gr_peak_amp_peak_area[ch_index_for_view_list[2]]->Draw("AP");
		c9->cd(4);
		gr_peak_amp_peak_area[ch_index_for_view_list[3]] = new TGraph(peak_amp_v[ch_index_for_view_list[3]].size(), &peak_amp_v[ch_index_for_view_list[3]][0], &peak_area_v[ch_index_for_view_list[3]][0]);
		gr_peak_amp_peak_area[ch_index_for_view_list[3]]->Draw("AP");
	}

	if (draw_var.find("peak_time_good_SiPMs") != std::string::npos)
	{
		TCanvas *c10 = new TCanvas("c10", "peak_time_good_SiPMs");
		hist_peak_time_good_SiPMs->Draw();

		for (int i = 0; i < vec_peak_time_good_SiPMs.size(); i++)
		{
			f_out << vec_peak_time_good_SiPMs[i] / 1000.0 << endl; //us
		}
	}

	if (draw_var.find("n_peaks_map") != std::string::npos)
	{
		TCanvas *c11 = new TCanvas("c11", "n_peaks_map");

		const Int_t NRGBs = 6;
		const Int_t NCont = 999;

		Double_t stops[NRGBs] = { 0.00, 0.2, 0.4, 0.6, 0.8, 1.00 };
		Double_t red[NRGBs] = { 0.99, 0.44, 0.00, 0.87, 1.00, 0.51 };
		Double_t green[NRGBs] = { 0.00, 0.66, 0.81, 1.00, 0.20, 0.00 };
		Double_t blue[NRGBs] = { 0.99, 0.72, 1.00, 0.12, 0.00, 0.00 };

		TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
		gStyle->SetNumberContours(NCont);
		gStyle->SetOptStat(0);

		const Double_t min = 0.65;
		const Double_t max = 1.3;

		const Int_t nLevels = 999;
		Double_t levels[nLevels];

		for (int i = 1; i < nLevels; i++)
		{
			levels[i] = min + (max - min) / (nLevels - 1) * (i);
		}
		levels[0] = 0.01;

		h2_n_peaks->SetContour((sizeof(levels) / sizeof(Double_t)), levels);

		int n_events = 0;
		//for (int i = 0; i < 25; i++)
		//{
		//	int xi = i % 4 + 1;
		//	int yi = i / 4 + 1;
		//	h2_n_peaks->SetBinContent(xi, yi, yv[i]);
		//	cout << i << "\t" << xi << "\t" << yi << "\t" << h2_S2_total_rel->GetBinContent(xi, yi) << endl;
		//	n_events++;
		//}
	}

	if (draw_var.find("n_peaks_chi_vs_chj") != std::string::npos)
	{
	
	}


	cout << endl;
	cout << "all is ok" << endl;
	theApp.Run();
	system("pause");
	return 0;
}