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
#include "TSystem.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TColor.h"
#include "TStyle.h"

//my
#include "ReadData_CAEN.h"
#include "Calc.h"
#include "ReadInfo.h"
#include "ReadDaqInfo.h"
#include "Path.h"

using namespace std;

int main(int argc, char *argv[])
{
	TApplication theApp("theApp", &argc, argv);//let's add some magic! https://root.cern.ch/phpBB3/viewtopic.php?f=3&t=22972
	gSystem->Load("libTree");// (to fix: no dictionary for class ttree is available) https://root.cern.ch/root/roottalk/roottalk04/1580.html
	
	Path path;
	//string date = "190704";
	string subfolder_name = path.GetSubFolderName();
	string output_folder = path.GetFirstPartOfPath() + "_caen_raw\\analysis\\";
	string file_name_output = output_folder + subfolder_name + "_avr_wf.txt";
	//string file_name_info = "E:\\" + date + "\\" + date + "_caen_raw\\info\\" + subfolder_name + "_info.txt";
	//string file_name_daq_info = "E:\\" + date + "\\" + date + "_caen_raw\\info\\daq_info.txt";

	
	ofstream file_output(file_name_output);

	//ReadDAQInfo rd_daq_inf(file_name_daq_info);
	ReadDAQInfo rd_daq_inf(path.GetFileNameDAQInfo());
	rd_daq_inf.Read();	


	unsigned int ns_per_point = /*4*//*16*/rd_daq_inf.GetNsPerPoint();
	unsigned int points_per_event_per_ch = /*40000*//*9999*/rd_daq_inf.GetPointsPerEventPerCh();
	unsigned int N_events_per_file = /*50*/rd_daq_inf.GetNEventsPerFileOutput();
	//ReadInfo rd_inf(file_name_info);
	ReadInfo rd_inf(path.GetFileNameInfo());
	rd_inf.Read();

	vector<double> yv_avr(points_per_event_per_ch);
	vector<double> xv(points_per_event_per_ch);
	for (int i = 0; i < points_per_event_per_ch; i++)
		xv[i] = i * ns_per_point / 1000.0;

	//vector<int> ch_to_avr_list = {0};
	vector<int> ch_to_avr_list = { 1,2,3,4 };
	//vector<int> ch_to_avr_list = { 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59 };

	if (rd_inf.GetChList().size() != 34)
	{
		cout << "rd_inf.GetChList().size() = " << rd_inf.GetChList().size() << " ;err" << endl;
		system("pause");
		exit(1);
	}

	const unsigned int number_of_input_files = path.GetNumberOfInputFiles();
	//string path_to_folder = "E:\\" + date + "\\" + date + "_caen_raw\\" + subfolder_name + "_mod\\";
	string path_to_folder = path.GetFirstPartOfPath() + "_caen_raw\\" + subfolder_name + "_mod\\";
	

	int absolute_event_counter = 0;
	for (int file_index = 0; file_index < number_of_input_files; file_index++)
	{
		cout << endl;
		int event_from = file_index * N_events_per_file;
		int event_to = (file_index + 1) * N_events_per_file - 1;
		ostringstream datafile_name;
		datafile_name << path_to_folder << std::setfill('0') << std::setw(6) << event_from << "__" << std::setfill('0') << std::setw(6) << event_to << ".dat";
		
		ReadData_CAEN rdt(datafile_name.str().c_str(), N_events_per_file, rd_inf.GetChList().size(), points_per_event_per_ch);

		//analyze and fill tree
		for (int ev = 0; ev < N_events_per_file; ev++)
		{
			//if (absolute_event_counter % 100 == 0)
				//cout << "abs_ev = " << absolute_event_counter << "; rel_ev = " << ev << endl;
			

			for (int ch = 0; ch < rd_inf.GetChList().size(); ch++)
			{
				Calc calc(rdt.GetDataDouble()[ev][ch], ns_per_point, rd_inf.GetIsPositivePolarityTypeList()[ch]);
				calc.CalcBaselineMeanSigma(0, 15000);
				//calc.CalcBaselineMeanSigma(0, 60000);
				//calc.CalcBaselineMeanSigma(0, 30000);
				//calc.CalcBaselineMeanSigma(120000, 140000);
				
				
				// (rd_inf.GetChList()[ch] == 0)
				// (find(ch_to_avr_list.begin(), ch_to_avr_list.end(), rd_inf.GetChList()[ch]) != ch_to_avr_list.end())
				if (find(ch_to_avr_list.begin(), ch_to_avr_list.end(), rd_inf.GetChList()[ch]) != ch_to_avr_list.end())
				{
					//cout << "ch = " << rd_inf.GetChList()[ch] << endl;
					//cout << calc.GetBaselineMean() << endl;

					for (int p = 0; p < points_per_event_per_ch; p++)
					{
						yv_avr[p] += (rdt.GetDataDouble()[ev][ch][p] - calc.GetBaselineMean());
						//yv_avr[p] += (rdt.GetDataDouble()[ev][ch][p]);
					}
				}
			}
			absolute_event_counter++;

			
			//if (ev == 50) goto label;
		}	
	}

	//label:

	for (int p = 0; p < points_per_event_per_ch; p++)
	{
		yv_avr[p] = yv_avr[p] / absolute_event_counter;
		file_output << xv[p] << "\t" << yv_avr[p] << endl;
	}

	TGraph *gr = new TGraph(xv.size(), &xv[0], &yv_avr[0]);
	gr->Draw("APL");

	cout << endl;
	cout << "all is ok" << endl;
	theApp.Run();
	system("pause");
}