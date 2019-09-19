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
#include "TGraphErrors.h"
#include "TMarker.h"

//my
#include "ReadData_CAEN.h"
#include "Calc.h"
#include "ReadInfo.h"
#include "ReadDaqInfo.h"


using namespace std;

#define COUT(x) cout << #x " = " << x << endl;

int main(int argc, char *argv[])
{
	TApplication theApp("theApp", &argc, argv);//let's add some magic! https://root.cern.ch/phpBB3/viewtopic.php?f=3&t=22972
	
	bool is_batch_mode = true;
	if (is_batch_mode)
		gROOT->SetBatch(kTRUE);

	string date = "190718";
	string subfolder_name = "f2";
	string output_folder = "E:\\" + date + "\\" + date + "_caen_raw\\analysis\\";
	ofstream file_detailed_info_output(output_folder + subfolder_name + "_detailed_info.txt");
	string file_name_output = output_folder + subfolder_name + ".root";
	string file_name_info = "E:\\" + date + "\\" + date + "_caen_raw\\info\\" + subfolder_name + "_info.txt";
	string file_name_daq_info = "E:\\" + date + "\\" + date + "_caen_raw\\info\\daq_info.txt";
	
	ReadDAQInfo rd_daq_inf(file_name_daq_info);
	rd_daq_inf.Read();
	//rd_daq_inf.Show();
	//system("pause");
	//exit(1);
	unsigned int ns_per_point = /*4*//*16*/rd_daq_inf.GetNsPerPoint();
	unsigned int points_per_event_per_ch = /*40000*//*9999*/rd_daq_inf.GetPointsPerEventPerCh();
	unsigned int N_events_per_file = /*50*/rd_daq_inf.GetNEventsPerFileOutput();
	ReadInfo rd_inf(file_name_info);
	rd_inf.Read();
	//rd_inf.Show();

	string file_name_raw = "E:\\" + date + "\\" + date + "_caen_raw\\" + subfolder_name + "_mod" + "\\000000__000009.dat";
	ReadData_CAEN rdt(file_name_raw, N_events_per_file, rd_inf.GetChList().size(), points_per_event_per_ch);

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
	const unsigned int n_events = 7;
	cout << endl;
	for (int ev = 0; ev < n_events; ev++)
	{
		//if (ev % 10 == 0)
			cout << "ev = " << ev << endl;

		file_detailed_info_output << "ev = " << ev << endl;



		for (int ch = 0; ch < rd_inf.GetChList().size(); ch++)
		{
			//test
			/*ev = 1;
			ch = 1;*/
			
			file_detailed_info_output << "\t" << "ch = " << rd_inf.GetChList()[ch] << endl;
			cout << "\t" << "ch = " << rd_inf.GetChList()[ch] << endl;
			Calc calc(rdt.GetDataDouble()[ev][ch], ns_per_point, rd_inf.GetIsPositivePolarityTypeList()[ch]);
			calc.CalcBaselineMeanSigma(0, 30000);
			calc.SubtractBaseline();
			yv_filtered = calc.GetFilteredWaveformGolay(/*21*/ rd_inf.GetFilteringWindowList()[ch], 0);

			PeakFinder peak_finder(rdt.GetDataDouble()[ev][ch], yv_filtered, ns_per_point, 
				rd_inf.GetWindowList()[ch], rd_inf.GetLocalBaselineWindowList()[ch], rd_inf.GetLocalBaselineWindowShiftList()[ch],
				rd_inf.GetCheckOverlappingWindowList()[ch], rd_inf.GetShrinkingOfLeftTailList()[ch], rd_inf.GetShrinkingOfRightTailList()[ch]);
			peak_finder.FindPeaksByAmp(/*30*//*mV*/ rd_inf.GetThList()[ch]);
			vector< pair<int, int> > pair_vec = peak_finder.GetPeakPositions();
			vector<double> local_baseline = peak_finder.GetLocalBaselineV();
			vector<double> avr_peak_time = peak_finder.GetAvrPeakTime();
			vector<double> peak_time = peak_finder.GetPeakTime();
			vector<double> peak_area = peak_finder.GetPeakArea();
			
			//cout peak characterictics
			file_detailed_info_output << "\t " << "N_peaks = " << pair_vec.size() << endl;
			//cout << "\t " << "N_peaks = " << pair_vec.size() << endl;
			for (int j = 0; j < pair_vec.size(); j++)
			{				
				file_detailed_info_output << "\t \t" << "peak number = " << j << endl;
				file_detailed_info_output << "\t \t" << "local_baseline = " << local_baseline[j] << endl;
				file_detailed_info_output << "\t \t" << "avr_peak_time = " << avr_peak_time[j] << endl;
				file_detailed_info_output << "\t \t" << "peak_time = " << peak_time[j] << endl;
				file_detailed_info_output << "\t \t" << "peak_area = " << peak_area[j] << endl;
				file_detailed_info_output << endl;
			}
			
			//cout << "\t" << "ch = " << ch << endl;
			//cout << "\t \t sigma = " << calc.GetBaselineSigma() << endl;

			ostringstream canv_name;
			canv_name << "event = " << ev << ", channel = " << rd_inf.GetChList()[ch];
			canv = new TCanvas( canv_name.str().c_str(), canv_name.str().c_str() );
			
			//draw raw values
			TGraph* gr = new TGraph(points_per_event_per_ch, &xv[0], &rdt.GetDataDouble()[ev][ch][0]);
			ostringstream gr_name;
			gr_name << "event = " << ev << ", channel = " << rd_inf.GetChList()[ch];
			gr->SetTitle( gr_name.str().c_str() );
			gr->Draw();

			//draw filtered values
			TGraph* gr_filtered = new TGraph(points_per_event_per_ch, &xv[0], &yv_filtered[0]);
			gr_filtered->Draw("same");
			gr_filtered->SetLineColor(kBlue);

			//draw peaks
			vector<double> signals_x_values;
			vector<double> signals_y_values;
			vector<double> local_baseline_y_values;
			for (int j = 0; j < pair_vec.size(); j++)
			{
				for (int k = pair_vec[j].first; k < pair_vec[j].second; k++)
				{
					signals_x_values.push_back(k * ns_per_point);
					//signals_y_values.push_back(yv_filtered[k]);
					signals_y_values.push_back(rdt.GetDataDouble()[ev][ch][k]);
					local_baseline_y_values.push_back(local_baseline[j]);
				}

				TMarker *mrk = new TMarker(peak_time[j], rdt.GetDataDouble()[ev][ch][peak_time[j]/ns_per_point], 20);
				mrk->Draw();
				mrk->SetMarkerColor(kMagenta+2);
				mrk->SetMarkerSize(2);
			}				
			if (signals_x_values.size() > 0)
			{
				TGraph *gr_peaks = new TGraph(signals_x_values.size(), &signals_x_values[0], &signals_y_values[0]);
				gr_peaks->SetMarkerSize(1);
				gr_peaks->SetMarkerStyle(29);
				gr_peaks->SetMarkerColor(kRed);
				gr_peaks->GetXaxis()->SetLimits(0, 160E3);
				gr_peaks->Draw("same P");

				TGraph *gr_local_baseline = new TGraph(signals_x_values.size(), &signals_x_values[0], &local_baseline_y_values[0]);
				gr_local_baseline->SetMarkerSize(1);
				gr_local_baseline->SetMarkerStyle(20);
				gr_local_baseline->SetMarkerColor(kGreen);
				gr_local_baseline->Draw("same P");
			}

			//draw peak time


			
			//draw baseline_sigma
			//const int gr_err_npoints = 100;
			//vector<double> gr_err_xv(gr_err_npoints);
			//for (int i = 0; i < gr_err_npoints; i++) gr_err_xv[i] = i * ns_per_point * points_per_event_per_ch / gr_err_npoints;
			//vector<double> gr_err_yv(gr_err_npoints, 0);
			//vector<double> gr_err_ex(gr_err_npoints, 0);
			//vector<double> gr_err_ey(gr_err_npoints, calc.GetBaselineSigma() * 1.5 );
			//TGraphErrors* ge = new TGraphErrors(gr_err_npoints, &gr_err_xv[0], &gr_err_yv[0], &gr_err_ex[0], &gr_err_ey[0]);
			////ge->SetFillColor(/*4*/kMagenta + 1);
			//ge->SetFillColorAlpha(kMagenta + 1, 0.35);
			//ge->SetFillStyle(/*3010*//*3005*//*3365*/3244);
			//ge->Draw("same 3");

			if (is_batch_mode)
				TObjArraylist.Add(canv);
		}

	}

	if (is_batch_mode)
	{ 			
		cout << endl << "Save plots in " << file_name_output.c_str() << endl;
		TFile f_root_out(file_name_output.c_str(), "recreate");
		TObjArraylist.Write();
		f_root_out.Close();
	}

	file_detailed_info_output.close();

	cout << endl;
	cout << "all is ok" << endl;
	if (!is_batch_mode)
		theApp.Run();
	system("pause");
	return 0;
}