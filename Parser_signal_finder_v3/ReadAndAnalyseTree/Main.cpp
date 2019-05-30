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
	//map <unsigned short, unsigned short> ;

	gSystem->Load("libTree");// (to fix: no dictionary for class ttree is available) https://root.cern.ch/root/roottalk/roottalk04/1580.html
	
	string file_name_tree = "E:\\190521\\190521_caen_trees\\file_1.root";
	TFile *f = new TFile(file_name_tree.c_str());
	if (!(f->IsOpen()))
	{
		cout << "could not open file: " << file_name_tree << endl;
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
	for (int ch = 0; ch < ch_list.size(); ch++)
	{
		ostringstream hist_ymin_name;
		ostringstream hist_ymax_name;
		
		hist_ymin_name << "hist_ymin_ch_" << ch_list[ch];
		hist_ymax_name << "hist_ymax_ch_" << ch_list[ch];

		hist_ymin_v[ch] = new TH1F(hist_ymin_name.str().c_str(), hist_ymin_name.str().c_str(), 1000, -1100, 1100);
		hist_ymax_v[ch] = new TH1F(hist_ymax_name.str().c_str(), hist_ymax_name.str().c_str(), 1000, -1100, 1100);
	}

	TH1F *hist_ymin = new TH1F("hist_ymin", "hist_ymin", 1000, -1000, 1000);
	TH1F *hist_ymax = new TH1F("hist_ymax", "hist_ymax", 1000, -1000, 1000);
	TH1F *hist_baseline_mean = new TH1F("hist_baseline_mean", "hist_baseline_mean", 1000, -1000, 1000);

	COUT( tree->GetEntries() )
	//ev loop
	for (unsigned int ev = 0; ev < tree->GetEntries(); ev++)
	{
		//read branch "EventMainCh"only
		if (ev % 1000 == 0)
			cout << "ev = " << ev << endl;
		
		branch->GetEntry(ev);

		vector<Peaks*> peaks = event->peaks;
		//cout << "\t" << "peaks.size() = " << peaks.size() << endl;

		//ch loop
		for (unsigned int ch = 0; ch < ch_list.size(); ch++)
		{
			//cout << "\t" << "ch_id = " << ch_list[ch] << endl;

			hist_ymin_v[ch]->Fill(event->ymin[ch]);
			hist_ymax_v[ch]->Fill(event->ymax[ch]);

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

	//draw

	//y_min
	/*TCanvas *c1 = new TCanvas("c1", "ymin");
	c1->Divide(2, 2, 0.01, 0.01);
	c1->cd(1);
	hist_ymin_v[0]->Draw();
	c1->cd(2);
	hist_ymin_v[1]->Draw();
	c1->cd(3);
	hist_ymin_v[2]->Draw();
	c1->cd(4);
	hist_ymin_v[3]->Draw();*/

	//y_max
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

	cout << endl;
	cout << "all is ok" << endl;
	theApp.Run();
	system("pause");	
	return 0;
}