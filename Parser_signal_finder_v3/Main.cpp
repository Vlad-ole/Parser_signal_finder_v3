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
#include "ReadInfo.h"
#include "ReadDaqInfo.h"
//#include "TreeRaw.h"
//#include "CalcData.h"
//#include "TreeInfo.h"
//#include "TreeRaw.h"

using namespace std;


int main(int argc, char **argv)
{	
	
	//timer
	TStopwatch timer_total;
	timer_total.Start();
	double time_read_binary = 0;
	double time_find_peaks = 0;
	double time_filter_waveform = 0;
	
	double peak_finder_th = 80;

	cout << "argc = " << argc << endl;
	for (int i = 0; i < argc; i++)
	{
		cout << "argv[" << i << "] = " << argv[i] << endl;
	}

	//string file_name_raw;
	string file_name_raw = "empty";
	if (argc > 1)
	{
		//file_name_raw = argv[1];
		peak_finder_th = atof(argv[1]);
	}
	else
	{
		//file_name_raw = "E:\\190521\\190521_caen_raw\\f1_mod\\000000__000099.dat";		
	}	
	cout << endl;
	//cout << "peak_finder_th = " << peak_finder_th << endl;

	//cout << "Press any key to confirm the execution." << endl;
	//system("pause");


	//string file_name_raw("D:\\Data_work\\Reco_test\\171123_caen_raw\\x_ray_20kV_PMT550_0dB_coll_2mm\\run_323__ch_0.dat");

	//some code for proper root cern operation
	TApplication theApp("theApp", &argc, argv);//let's add some magic! https://root.cern.ch/phpBB3/viewtopic.php?f=3&t=22972
	gROOT->SetBatch(kTRUE);

	string date = "190919";
	string year = "20" + date.substr(0, 2);
	string first_part_of_path = "E:\\" + year + "\\" + date + "\\" + date;
	string subfolder_name = "f1";
	string file_name_info = first_part_of_path + "_caen_raw\\info\\" + subfolder_name + "_info.txt";
	string file_name_daq_info = first_part_of_path + "_caen_raw\\info\\daq_info.txt";
	
	ReadDAQInfo rd_daq_inf(file_name_daq_info);
	rd_daq_inf.Read();
	//Read DAQ_info.txt
	unsigned int ns_per_point = /*4*//*16*/rd_daq_inf.GetNsPerPoint();
	unsigned int points_per_event_per_ch = /*40000*//*9999*/rd_daq_inf.GetPointsPerEventPerCh();
	unsigned int N_events_per_file = /*100*/rd_daq_inf.GetNEventsPerFileOutput();
	ReadInfo rd_inf(file_name_info);
	rd_inf.Read();
	//if (rd_inf.GetChList().size() != 33)
	//{
	//	cout << "err" << endl;
	//	system("pause");
	//}
	//cout << "peak_finder_th for ch 32 = " << rd_inf.GetThList()[10] << " mV" << endl;
	//vector<unsigned short> ch_list = {1, 2, 3, 4};
	//vector<bool> is_positive_polarity_type_list = { true, true, true, true };

	//const unsigned int n_event_to_process = 2;
	const unsigned int number_of_input_files = 237;
	string path_to_folder = first_part_of_path + "_caen_raw\\" + subfolder_name + "_mod\\";

	//create tree
	//ostringstream file_for_tree_name;
	string file_for_tree_name;
	//file_for_tree_name << "E:\\" << date << "\\" << date << "_caen_trees\\" << subfolder_name << "_th" << rd_inf.GetThList()[10] << "mV.root";
	//file_for_tree_name << "E:\\" << year << "\\" << date << "\\" << date << "_caen_trees\\" << subfolder_name << "_info_v1.root";
	file_for_tree_name = first_part_of_path + "_caen_trees\\" + subfolder_name + "_info_v1.root";
	TFile file_for_tree(file_for_tree_name.c_str(), "RECREATE");
	TTree tree_main("TreeMain", "TreeMain");
	EventMainCh *event = new EventMainCh();
	tree_main.Branch("EventMainCh", &event, 16000, 0);

	unsigned int absolute_event_counter = 0;
	for (int file_index = 0; file_index < number_of_input_files; file_index++)
	{
		cout << endl;
		int event_from = file_index * N_events_per_file;
		int event_to = (file_index + 1) * N_events_per_file - 1;
		ostringstream datafile_name;
		datafile_name << path_to_folder << std::setfill('0') << std::setw(6) << event_from << "__" << std::setfill('0') << std::setw(6) << event_to << ".dat";
		//cout << datafile_name.str() << endl << endl;

		//Read one xxxxxx__xxxxxx.dat file
		TStopwatch timer_read_binary;
		timer_read_binary.Start();		
		ReadData_CAEN rdt(datafile_name.str().c_str(), N_events_per_file, rd_inf.GetChList().size(), points_per_event_per_ch);
		timer_read_binary.Stop();
		time_read_binary += timer_read_binary.RealTime();

		//analyze and fill tree
		for (int ev = 0; ev < N_events_per_file; ev++)
		{			
			if (ev % 10 == 0)
				cout << "abs_ev = " << absolute_event_counter << "; rel_ev = " << ev << endl;

			event->event_number = absolute_event_counter;

			for (int ch = 0; ch < rd_inf.GetChList().size(); ch++)
			{
				Calc calc(rdt.GetDataDouble()[ev][ch], ns_per_point, rd_inf.GetIsPositivePolarityTypeList()[ch]);
				event->ymin.push_back(calc.GetYmin());
				event->ymax.push_back(calc.GetYmax());

				calc.CalcBaselineMeanSigma(0, 30000);
				event->baseline_mean.push_back(calc.GetBaselineMean());
				event->baseline_sigma.push_back(calc.GetBaselineSigma());

				calc.SubtractBaseline();

				TStopwatch timer_filter_waveform;
				timer_filter_waveform.Start();
				vector<double> yv_filtered(points_per_event_per_ch);
				yv_filtered = calc.GetFilteredWaveformGolay(/*21*/ rd_inf.GetFilteringWindowList()[ch], 0);
				timer_filter_waveform.Stop();
				time_filter_waveform += timer_filter_waveform.RealTime();

				TStopwatch timer_find_peaks;
				timer_find_peaks.Start();
				//PeakFinder peak_finder(rdt.GetDataDouble()[ev][ch], yv_filtered, ns_per_point);
				PeakFinder peak_finder(rdt.GetDataDouble()[ev][ch], yv_filtered, ns_per_point,
					rd_inf.GetWindowList()[ch], rd_inf.GetLocalBaselineWindowList()[ch], rd_inf.GetLocalBaselineWindowShiftList()[ch],
					rd_inf.GetCheckOverlappingWindowList()[ch], rd_inf.GetShrinkingOfLeftTailList()[ch],
					rd_inf.GetShrinkingOfRightTailList()[ch], rd_inf.GetIsLocalBaselineList()[ch]);
				peak_finder.FindPeaksByAmp(rd_inf.GetThList()[ch]/*peak_finder_th*//*mV*/);
				timer_find_peaks.Stop();
				time_find_peaks += timer_find_peaks.RealTime();

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
			absolute_event_counter++;
		}

	}

	cout << endl << "Write tree:" << file_for_tree_name.c_str() << endl;
	TStopwatch timer_write_and_close;
	timer_write_and_close.Start();
	file_for_tree.Write();
	timer_write_and_close.Stop();

	//EventMainCh *event_main_ch = new EventMainCh();

	timer_total.Stop();
	cout << endl;
	cout << "-------------------------------" << endl;
	cout << "time to read binary files = " << time_read_binary << " sec (" << time_read_binary*100.0 / timer_total.RealTime() << " %)" << endl;
	cout << "time to filter waveform = " << time_filter_waveform << " sec (" << time_filter_waveform*100.0 / timer_total.RealTime() << " %)" << endl;
	cout << "time to find peaks = " << time_find_peaks << " sec (" << time_find_peaks*100.0 / timer_total.RealTime() << " %)" << endl;
	cout << "time to write and close = " << timer_write_and_close.RealTime() << " sec (" << timer_write_and_close.RealTime()*100.0 / timer_total.RealTime() << " %)" << endl;
	cout << "total time = " << timer_total.RealTime() << " sec" << endl;
	cout << "time per event = " << timer_total.RealTime() / absolute_event_counter << " sec per event" << endl;

	cout << endl;
	cout << "all is ok" << endl;
	system("pause");
	return 0;
}