//c++
#include <iostream>
#include <sstream>
#include <vector>
#include <iomanip>
#include <fstream>
#include <omp.h> 

//root cern
#include "TApplication.h"
#include "TROOT.h"
#include "TStopwatch.h"
#include "TFile.h"
#include "TTree.h"
#include "TThread.h"

//my
#include "ReadData_CAEN.h"
#include "RunDescription.h"
#include "EventMainCh.h"
#include "Peaks.h"
#include "Calc.h"
#include "PeakFinder.h"
//#include "TreeRaw.h"
//#include "CalcData.h"
//#include "TreeInfo.h"
//#include "TreeRaw.h"

using namespace std;


int main(int argc, char **argv)
{
	cout << "argc = " << argc << endl;
	for (int i = 0; i < argc; i++)
	{
		cout << "argv[" << i << "] = " << argv[i] << endl;
	}

	//string file_name_raw;
	string file_name_raw = "empty";
	if (argc > 1)
	{
		file_name_raw = argv[1];
	}
	else
	{
		file_name_raw = "E:\\190521\\190521_caen_raw\\f1_mod\\000000__000099.dat";
	}
	cout << endl;

	//string file_name_raw("D:\\Data_work\\Reco_test\\171123_caen_raw\\x_ray_20kV_PMT550_0dB_coll_2mm\\run_323__ch_0.dat");

	//some code for proper root cern operation
	TApplication theApp("theApp", &argc, argv);//let's add some magic! https://root.cern.ch/phpBB3/viewtopic.php?f=3&t=22972
	gROOT->SetBatch(kTRUE);

	//Read DAQ_info.txt
	unsigned int ns_per_point = 4/*16*/;
	unsigned int points_per_event_per_ch = 40000/*9999*/;
	unsigned int N_events_per_file = 100;
	vector<unsigned short> ch_list = {1, 2, 3, 4};
	vector<bool> is_positive_polarity_type_list = { true, true, true, true };

	//Read one .dat file
	//ReadData_CAEN rdt(PathInfo, ch_list, str_comm);
	ReadData_CAEN rdt(file_name_raw, N_events_per_file, ch_list.size(), points_per_event_per_ch);

	ostringstream file_for_tree_name;
	file_for_tree_name << "E:\\190521\\190521_caen_trees\\file_1" << ".root";
	TFile file_for_tree(file_for_tree_name.str().c_str(), "RECREATE");

	TTree tree_main("TreeMain", "TreeMain");

	EventMainCh *event = new EventMainCh();

	tree_main.Branch("EventMainCh", &event, 16000, 0);

	//analyze and fill tree
	for (int ev = 0; ev < /*N_events_per_file*/2; ev++)
	{
		if (ev % 10 == 0)
			cout << "ev = " << ev << endl;
		
		event->event_number = ev;
		
		for (int ch = 0; ch < ch_list.size(); ch++)
		{
			Calc calc(rdt.GetDataDouble()[ev][ch], ns_per_point, is_positive_polarity_type_list[ch]);			
			event->ymin.push_back( calc.GetYmin() );
			event->ymax.push_back( calc.GetYmax() );

			calc.CalcBaselineMeanSigma(0, 30000);
			event->baseline.push_back(calc.GetBaselineMean());
			
			calc.SubtractBaseline();
			vector<double> yv_filtered(points_per_event_per_ch); 
			yv_filtered = calc.GetFilteredWaveformGolay(/*21*/ 21, 0);

			PeakFinder peak_finder(yv_filtered, ns_per_point);
			peak_finder.FindPeaksByAmp(80/*mV*/);
			
			Peaks *peaks_obj = new Peaks();
			peaks_obj->peak_start_stop_poits = peak_finder.GetPeakPositions();
			peaks_obj->avr_peak_time = peak_finder.GetAvrPeakTime();
			peaks_obj->peak_time = peak_finder.GetPeakTime();
			peaks_obj->peak_amp = peak_finder.GetPeakAmp();
			peaks_obj->peak_area = peak_finder.GetPeakArea();
			peaks_obj->local_baseline_v = peak_finder.GetLocalBaselineV();

			event->peaks.push_back(peaks_obj);

		}

		tree_main.Fill();
		event->Clear();

	}

	cout << endl << "Write tree:" << file_for_tree_name.str().c_str() << endl;
	file_for_tree.Write();

	//EventMainCh *event_main_ch = new EventMainCh();

	cout << endl;
	cout << "all is ok" << endl;
	system("pause");
	return 0;
}