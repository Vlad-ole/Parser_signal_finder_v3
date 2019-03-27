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
//#include "TreeRaw.h"
//#include "CalcData.h"
//#include "TreeInfo.h"
//#include "TreeRaw.h"

using namespace std;


int main(int argc, char **argv)
{
	
	for (int i = 0; i < argc; i++)
	{
		cout << "argv[" << i << "] = " << argv[i] << endl;
	}

	////test
	/////////////////////////////////////
	//double x = 1;

	//for (int i = 0; i < 100000000; i++)
	//{
	//	x = sqrt(x + 1) + 1 + sin(x*x);
	//	if (i % 10000000 == 0)
	//		cout << "i = " << i << endl;
	//}
	//cout << x << endl;
	/////////////////////////////////////


	//string file_name_raw;
	string file_name_raw = "empty";
	if (argc > 1)
	{
		file_name_raw = argv[1];
	}


	//string file_name_raw("D:\\Data_work\\Reco_test\\171123_caen_raw\\x_ray_20kV_PMT550_0dB_coll_2mm\\run_323__ch_0.dat");

	//some code for prorerly root cern operation
	TApplication theApp("theApp", &argc, argv);//let's add some magic! https://root.cern.ch/phpBB3/viewtopic.php?f=3&t=22972
	gROOT->SetBatch(kTRUE);

	//Read DAQ_info.txt
	int ns_per_point = 16;
	int points_per_event = 9999;
	int N_events_per_file = 1000;
	vector<unsigned short> ch_list = { 0, 1, 2 };

	//Read one .dat file


	//ReadData_CAEN rdt(PathInfo, ch_list, str_comm);
	ReadData_CAEN rdt(file_name_raw, N_events_per_file, points_per_event);

	TFile* f_tree = NULL;
	TTree* tree = NULL;

	TTree tree_main("TreeMain", "A Tree with Events");
	EventMainCh *event_main_ch = new EventMainCh();

	cout << endl;
	cout << "all is ok" << endl;
	system("pause");
	return 0;
}