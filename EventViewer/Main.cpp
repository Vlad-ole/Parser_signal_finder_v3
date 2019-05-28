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
#include "TGraph.h"
#include "TCanvas.h"

//my
#include "ReadData_CAEN.h"
#include "Calc.h"


using namespace std;

#define COUT(x) cout << #x " = " << x << endl;

int main(int argc, char *argv[])
{
	TApplication theApp("theApp", &argc, argv);//let's add some magic! https://root.cern.ch/phpBB3/viewtopic.php?f=3&t=22972
	
	bool is_batch_mode = true;
	if (is_batch_mode)
		gROOT->SetBatch(kTRUE);

	

	unsigned int ns_per_point = 4/*16*/;
	unsigned int points_per_event_per_ch = 40000/*9999*/;
	unsigned int N_events_per_file = 100;
	vector<unsigned short> ch_list = { 1, 2, 3, 4 };
	vector<bool> is_positive_polarity_type_list = { true, true, true, true };

	string file_name_raw = "E:\\190521\\190521_caen_raw\\f1_mod\\000000__000099.dat";
	ReadData_CAEN rdt(file_name_raw, N_events_per_file, ch_list.size(), points_per_event_per_ch);

	vector<double> yv_filtered(points_per_event_per_ch);
	vector<double> xv(points_per_event_per_ch);
	for (int i = 0; i < points_per_event_per_ch; i++)
		xv[i] = i * ns_per_point;

	//v1
	//unsigned int ev = 0;
	//unsigned int ch = 0;
	//TGraph *gr_waveform = new TGraph(points_per_event_per_ch, &xv[0], &rdt.GetDataDouble()[ev][ch][0]);	
	//gr_waveform->Draw("APL");

	//v2	
	TObjArray TObjArraylist(0);      // create an array of TGraph
	//TGraph* gr;                 // create a pointer to a TGraph
	TCanvas* canv;
	//const unsigned int n_events = N_events_per_file;
	const unsigned int n_events = 1;
	cout << endl;
	for (int ev = 0; ev < n_events; ev++)
	{
		if (ev % 10 == 0)
			cout << "ev = " << ev << endl;

		for (int ch = 0; ch < ch_list.size(); ch++)
		{
			Calc calc(rdt.GetDataDouble()[ev][ch], ns_per_point, is_positive_polarity_type_list[ch]);
			yv_filtered = calc.GetFilteredWaveformGolay(/*21*/ 151, 0);
			
			ostringstream canv_name;
			canv_name << "event = " << ev << ", channel = " << ch;
			canv = new TCanvas( canv_name.str().c_str(), canv_name.str().c_str() );
			
			TGraph* gr = new TGraph(points_per_event_per_ch, &xv[0], &rdt.GetDataDouble()[ev][ch][0]);
			ostringstream gr_name;
			gr_name << "event = " << ev << ", channel = " << ch;
			gr->SetTitle( gr_name.str().c_str() );
			gr->Draw();

			TGraph* gr_filtered = new TGraph(points_per_event_per_ch, &xv[0], &yv_filtered[0]);
			gr_filtered->Draw("same");
			gr_filtered->SetLineColor(kBlue);

			TObjArraylist.Add(canv);
		}

	}

	if (is_batch_mode)
	{ 
		string file_name_output = "E:\\190521\\190521_caen_raw\\f1_analysis\\analysis.root";		
		cout << endl << "Save plots in " << file_name_output.c_str() << endl;
		TFile f_root_out(file_name_output.c_str(), "recreate");
		TObjArraylist.Write();
		f_root_out.Close();
	}

	cout << endl;
	cout << "all is ok" << endl;
	if (!is_batch_mode)
		theApp.Run();
	system("pause");
	return 0;
}