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
#include "TProfile.h"

//my
#include "EventMainCh.h"
#include "ReadInfo.h"
#include "ReadDaqInfo.h"
#include "ChMapping.h"
#include "Path.h"
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
	//gSystem->Load("libCore");
	//gSystem->Load("libCint");
	
	TApplication theApp("theApp", &argc, argv);//let's add some magic! https://root.cern.ch/phpBB3/viewtopic.php?f=3&t=22972

	//TH1F* hist_test = new TH1F("hist_test", "hist_test", 1330, 0, 160E3);

	string draw_var;
	string what_to_plot;
	what_to_plot = "all";
	//what_to_plot = "main";

	bool is_calib = false;
	double SiPM_axis_area_factor_for_calib = 1;
	if (is_calib)
	{
		SiPM_axis_area_factor_for_calib = 1 /*0.3*/;
	}

	
	if (what_to_plot == "all")
	{
		draw_var += "ymin ymax baseline_mean baseline_sigma";		
		draw_var += "peak_amp peak_area n_peaks";		
		

		draw_var += "n_peaks_map_bkg";
		draw_var += "n_peaks_map_S1";
		draw_var += "n_peaks_map_S2";

		draw_var += "peak_area_ev";

		draw_var += "peak_area_ev_vs_evnum_allSiPMs_allPMTs";
		draw_var += "peak_amp_vs_peak_area_hist2";
	}
	
	draw_var += "peak_time";
	draw_var += "peak_length";
	draw_var += "peak_amp_vs_peak_area";	
	
	draw_var += "peak_time_all_SiPMs_PMTs_slow_PMTs_fast";

	///draw_var += "peak_area_ev_vs_evnum";
	
	
	


	
	draw_var += "peak_area_bkg";
	draw_var += "peak_area_S1";
	draw_var += "peak_area_S2";

	//draw_var += "n_peaks_bkg";
	//draw_var += "n_peaks_S1";
	//draw_var += "n_peaks_S2";

	draw_var += "PE_SiPM_PMTs_slow_S2";	

	draw_var += "peaks_area_map_bkg";
	draw_var += "peaks_area_map_S1";
	draw_var += "peaks_area_map_S2";

	//draw_var += "indiv_peak_area_map";


	//string draw_var = "n_peaks_map"; //view good_SiPMs
	//string draw_var = "peak_time peak_amp peak_area n_peaks n_peaks_ch1_ch2";
	//string draw_var = "peak_amp";
	
	//vector<int> ch_list_to_view = { 4, 9, 32, 38};
	//vector<int> ch_list_to_view = { 32, 33, 34, 35};
	//vector<int> ch_list_to_view = { 1, 2, 3, 4 };
	
	//vector<int> ch_list_to_view = {32, 34, 38, 39};
	
	//vector<int> ch_list_to_view = {0, 9, 11, 12};
	//vector<int> ch_list_to_view = {1, 2, 3, 4};
	//vector<int> ch_list_to_view = {5, 6, 7, 8};

	vector<int> ch_list_to_view = { 32, 33, 34, 35 };
	//vector<int> ch_list_to_view = {36, 37, 38, 39};
	//vector<int> ch_list_to_view = { 40, 41, 42, 48 };
	//vector<int> ch_list_to_view = { 49, 50, 51, 52 };
	//vector<int> ch_list_to_view = { 53, 54, 55, 56 };
	//vector<int> ch_list_to_view = { 56, 57, 58, 59 };
	//vector<int> ch_list_to_view = { 38, 43, 44, 59 };
	vector<int> ch_list_good_SiPMs = { 
		32, 33, 34, 35,
		36, 37, 38, 39,
		40, 41, 42, 48,
		49, 50, 51, 52,
		53, 54, 55, 56,
		57, 58, 59 };

	vector<int> ch_list_good_SiPMs_xy_order = {
		32,33,48,49,34,
		35,50,51,36,37,
		52,53,38,39,54,
		55,40,41,56,57,
		42,58,59};
	
	vector< pair<double, double> > xybins_good_SiPMs = {
		{ -2, 2 }, { -1, 2 }, { 0, 2 }, { 1, 2 }, { 2, 2 },
		{ -2, 1 }, { -1, 1 }, { 0, 1 }, { 1, 1 }, { 2, 1 },
		{ -2, 0 }, { -1, 0 }, { 0, 0 }, { 1, 0 }, { 2, 0 },
		{ -2, -1 }, { -1, -1 }, { 0, -1 }, { 1, -1 }, { 2, -1 },
		{ -2, -2 }, { 0, -2 }, { 1, -2 } };

	//vector< pair<double, double> > xy_good_SiPMs = { 
	//	{ -20, 20 }, { -10, 20 }, { 20, 20 }, { -20, 10},
	//	{10,10}, {20,10}, {0,0}, {10,0},
	//	{-10,-10}, {0,-10}, {-20,-20}, {0,20},
	//	{10, 20}, {-10, 10}, { 0, 10}, {-20,0},
	//	{-10,0}, {20,0}, {-20,-10}, {10,-10},
	//	{20,-10}, {0,-20}, {10,-20}
	//};
	//vector<int> ch_list_good_SiPMs = { 1, 2, 3, 4 };



	

	int hist_baseline_sigma_xmax = 50;
	


	gSystem->Load("libTree");// (to fix: no dictionary for class ttree is available) https://root.cern.ch/root/roottalk/roottalk04/1580.html

	//in
	Path path;


	ReadDAQInfo rd_daq_inf(path.GetFileNameDAQInfo());
	rd_daq_inf.Read();
	ReadInfo rd_inf(path.GetFileNameInfo());
	rd_inf.Read();
	vector<int> ch_list = rd_inf.GetChList(); //all ch
	double ns_per_point = rd_daq_inf.GetNsPerPoint();
	ChMapping ch_map(ch_list, ch_list_to_view);
	vector<int> ch_index_for_view_list = ch_map.GetChIndexList();

	ChMapping ch_map_goodSiPMs(ch_list, ch_list_good_SiPMs_xy_order);
	vector<int> ch_index_for_good_SiPMs_xy_order = ch_map_goodSiPMs.GetChIndexList();

	string file_name_tree = path.GetFirstPartOfPath() + "_caen_trees\\" + path.GetSubFolderName() + "_info_v1.root";
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
	string part_of_path2 = path.GetFirstPartOfPath() + "_caen_trees\\" + path.GetSubFolderName();

	string f_out_allSiPM_time_spectrum_name = part_of_path2 + "_allSiPM_time_specrtrum.txt";
	ofstream f_out_allSiPM_time_spectrum(f_out_allSiPM_time_spectrum_name);
	
	string f_out_allPMTs_slow_time_spectrum_name = part_of_path2 + "_allPMTs_slow_time_specrtrum.txt";
	ofstream f_out_allPMTs_slow_time_spectrum(f_out_allPMTs_slow_time_spectrum_name);

	string f_out_allPMTs_fast_time_spectrum_name = part_of_path2 + "_allPMTs_fast_time_specrtrum.txt";
	ofstream f_out_allPMTs_fast_time_spectrum(f_out_allPMTs_fast_time_spectrum_name);

	string f_out_3PMTs_slow_time_spectrum_name = part_of_path2 + "_3PMTs_slow_time_spectrum.txt";
	ofstream f_out_3PMTs_slow_time_spectrum(f_out_3PMTs_slow_time_spectrum_name);

	string f_out_1PMT_slow_time_spectrum_name = part_of_path2 + "_1PMT_slow_time_spectrum.txt";
	ofstream f_out_1PMT_slow_time_spectrum(f_out_1PMT_slow_time_spectrum_name);

	
	ofstream f_out_1PMT_slow_area_ev_spectrum(part_of_path2 + "_1PMT_slow_area_ev_spectrum.txt");
	ofstream f_out_2PMT_slow_area_ev_spectrum(part_of_path2 + "_2PMT_slow_area_ev_spectrum.txt");
	ofstream f_out_3PMT_slow_area_ev_spectrum(part_of_path2 + "_3PMT_slow_area_ev_spectrum.txt");
	ofstream f_out_4PMT_slow_area_ev_spectrum(part_of_path2 + "_4PMT_slow_area_ev_spectrum.txt");
	ofstream f_out_PE_S1_S2(part_of_path2 + "_PE_S1_S2.txt");
	ofstream f_out_S2_SiPM_PE_map(part_of_path2 + "_S2_SiPM_PE_map.txt");

	TTree *tree = (TTree*)f->Get("TreeMain");

	EventMainCh *event = new EventMainCh(/*ch_list.size()*/);
	TBranch *branch = tree->GetBranch("EventMainCh");
	branch->SetAddress(&event);
	//tree->SetBranchAddress("EventMainCh", &event);


	vector<double> ch_calib(ch_list.size(), 1);

	//my old calib
	//ch_calib[1] = 7700;
	//ch_calib[2] = 19370;
	//ch_calib[3] = 7050;
	//ch_calib[4] = 17220;

	//calib from Egor
	//ch_calib[1] = 6584;
	//ch_calib[2] = 12777;
	//ch_calib[3] = 6327;
	//ch_calib[4] = 14086;

	//my calib from 191107. 850V? 12db?
	ch_calib[1] = 7262;
	ch_calib[2] = 15390;
	ch_calib[3] = 7111;
	ch_calib[4] = 16160;

	//my calib using 200116 f10 800V 0db
	/*ch_calib[1] = 25860;
	ch_calib[2] = 46820;
	ch_calib[3] = 25790;
	ch_calib[4] = 40010;*/

	////my calib using 200116 f10 800V 0db and adapt for 800V 12db
	//ch_calib[1] = 25860 * 0.25;
	//ch_calib[2] = 46820 * 0.25;
	//ch_calib[3] = 25790 * 0.25;
	//ch_calib[4] = 40010 * 0.25;


	//for (int i = 11; i <= 33; i++)
	//{
	//	ch_calib[i] = 1400;//estimated SPE for SiPMs 46V
	//}
	//my calib from 191107 46V
	//ch_calib[11] = 1774; //ch32 (same as 210401_f1)
	//ch_calib[12] = 1449; //ch33 (same as 210401_f1)
	//ch_calib[13] = 2000; //ch34 (same as 210401_f1)
	//ch_calib[14] = 1987; //ch35 (same as 210401_f1)
	//ch_calib[15] = 2158; //ch36 (same as 210401_f1)
	//ch_calib[16] = 1656; //ch37 (same as 210401_f1)
	//ch_calib[17] = 2337; //ch38 (same as 210401_f1)
	//ch_calib[18] = 1980; //ch39 (same as 210401_f1)
	//ch_calib[19] = 2052; //ch40 (same as 210401_f1)
	//ch_calib[20] = 2006; //ch41 (same as 210401_f1)
	//ch_calib[21] = 1985; //ch42 (same as 210401_f1)
	//ch_calib[22] = 1942; //ch48 (same as 210401_f1)
	//ch_calib[23] = 1403; //ch49 (same as 210401_f1)
	//ch_calib[24] = 1435; //ch50 (same as 210401_f1)
	//ch_calib[25] = 1780; //ch51 (same as 210401_f1)
	//ch_calib[26] = 2149; //ch52 (same as 210401_f1)
	//ch_calib[27] = 2407; //ch53 (same as 210401_f1)	
	//ch_calib[28] = 1750; //ch54 (same as 210401_f1)
	//ch_calib[29] = 1886; //ch55 (same as 210401_f1)
	//ch_calib[30] = 2008; //ch56 (same as 210401_f1)
	//ch_calib[31] = 2077; //ch57 (same as 210401_f1)
	//ch_calib[32] = 1994; //ch58 (same as 210401_f1)
	//ch_calib[33] = 2057; //ch59 (same as 210401_f1)

	//my calib from 180830_f1 49V
	ch_calib[2] = 6894; //ch32 
	ch_calib[3] = 4758; //ch33 
	ch_calib[4] = 8680; //ch34 
	ch_calib[5] = 8483; //ch35 
	ch_calib[6] = 8270; //ch36 
	ch_calib[7] = 5682; //ch37 
	ch_calib[8] = 10130; //ch38 
	ch_calib[9] = 8255; //ch39 
	ch_calib[10] = 10270; //ch40 
	ch_calib[11] = 9185; //ch41 
	ch_calib[12] = 10170; //ch42
	ch_calib[13] = 7755; //ch48 
	ch_calib[14] = 4465; //ch49 
	ch_calib[15] = 4622; //ch50 
	ch_calib[16] = 6617; //ch51 
	ch_calib[17] = 7781; //ch52 
	ch_calib[18] = 9828; //ch53 
	ch_calib[19] = 6727; //ch54 
	ch_calib[20] = 7755; //ch55 
	ch_calib[21] = 8697; //ch56 
	ch_calib[22] = 9734; //ch57 
	ch_calib[23] = 8549; //ch58 
	ch_calib[24] = 9630; //ch59

	double Bkg_start = 0;
	double Bkg_stop = /*15000*/ 20000;
	double Bkg_dt = Bkg_stop - Bkg_start;
	
	double S1_start = /*16000*/ 32000 /*4000*/;
	double S1_stop = /*22000*/ 38000 /*8000*/;
	double S1_dt = S1_stop - S1_start;

	double S2_start = 72000/*27000*/;
	double S2_stop =  87000 /*39000*/;
	double S2_dt = S2_stop - S2_start;	

	//useed for alpha-soure only
	double S2_test_pretrigger_start = 0;
	double S2_test_pretrigger_stop = /*70000*/23000;

	double S2_min_SiPM = 3;
	double S2_max_SiPM = 12;

	double S2_test_pretrigger_SiPM_max = 3;

	double S2_trigger_start = 79000/*31000*/;//us
	double S2_trigger_stop = 83000/*35000*/;//us

	//define hists (joined ch)
	TH1F* hist_peak_time_all_SiPMs = new TH1F("hist_peak_time_all_SiPMs", "hist_peak_time_all_SiPMs", 1330, 0, 160E3);
	TH1F* hist_peak_time_all_PMTs_slow = new TH1F("hist_peak_time_all_PMTs_slow", "hist_peak_time_all_PMTs_slow", 1330, 0, 160E3);
	TH1F* hist_peak_time_all_PMTs_fast = new TH1F("hist_peak_time_all_PMTs_fast", "hist_peak_time_all_PMTs_fast", 1330, 0, 160E3);
	TH1F* hist_peak_time_3PMTs_slow = new TH1F("hist_peak_time_3PMTs_slow", "hist_peak_time_3PMTs_slow", 160, 0, 160E3);
	TH1F* hist_peak_time_1PMT_slow = new TH1F("hist_peak_time_1PMT_slow", "hist_peak_time_1PMT_slow", 160, 0, 160E3);
	TH1F* hist_peak_time_PMTtrigg_slow = new TH1F("hist_peak_time_PMTtrigg_slow", "hist_peak_time_PMTtrigg_slow", 1330, 0, 160E3);
	TH2F* hist_peak_area_ev_vs_evnum_all_SiPMs = new TH2F("hist_peak_area_ev_vs_evnum_all_SiPMs",
		"hist_peak_area_ev_vs_evnum_all_SiPMs", 110, 0, tree->GetEntries(),
		200, 0, 600);
	
	int x_max_hist_PE = 180;
	TH1F* hist_PE_allPMT_S2 = new TH1F("hist_PE_allPMT_S2", "hist_PE_allPMT_S2", x_max_hist_PE*2, 0, x_max_hist_PE);
	TH1F* hist_PE_allSiPMs_S2 = new TH1F("hist_PE_allSiPMs_S2", "hist_PE_allSiPMs_S2", x_max_hist_PE * 2, 0, x_max_hist_PE);
	TH1F* hist_PE_allSiPMs_S2_areabased = new TH1F("hist_PE_allSiPMs_S2_areabased", "hist_PE_allSiPMs_S2_areabased", x_max_hist_PE * 2, 0, x_max_hist_PE);
	TH1F* hist_PE_allSiPMs_S2_test_pretrigger_areabased = new TH1F("hist_PE_allSiPMs_S2_test_pretrigger_areabased", "hist_PE_allSiPMs_S2_test_pretrigger_areabased", x_max_hist_PE * 2, 0, x_max_hist_PE);


	vector<double> vec_peak_time_good_SiPMs;
	//TH2F *hist2_peak_time_SiPM_good = new TH2F("hist2_peak_time_SiPM_good", "hist2_peak_time_SiPM_good", /*4000*/4000, 0, 160E3, 1000, 0, 50000);
	TH2F* h2_n_peaks_S1 = new TH2F("h2_n_peaks_S1", "h2_n_peaks_S1", 5, -23, 23, 5, -23, 23);
	TH2F* h2_n_peaks_S2 = new TH2F("h2_n_peaks_S2", "h2_n_peaks_S2", 5, -23, 23, 5, -23, 23);
	TH2F* h2_n_peaks_bkg= new TH2F("h2_n_peaks_bkg", "h2_n_peaks_bkg", 5, -23, 23, 5, -23, 23);

	TH2F* h2_peaks_area_S1 = new TH2F("h2_peaks_area_S1", "h2_peaks_area_S1", 5, -23, 23, 5, -23, 23);
	TH2F* h2_peaks_area_S2 = new TH2F("h2_peaks_area_S2", "h2_peaks_area_S2", 5, -23, 23, 5, -23, 23);
	TH2F* h2_peaks_area_bkg = new TH2F("h2_peaks_area_bkg", "h2_peaks_area_bkg", 5, -23, 23, 5, -23, 23);

	//define hists (individual ch)
	vector<TH1F*> hist_ymin_v(ch_list.size(), NULL);
	vector<TH1F*> hist_ymax_v(ch_list.size(), NULL);
	vector<TH1F*> hist_baseline_mean_v(ch_list.size(), NULL);
	vector<TH1F*> hist_baseline_sigma_v(ch_list.size(), NULL);
	vector<TH1F*> hist_n_peaks_v(ch_list.size(), NULL);
	vector<TH1F*> hist_peak_time_v(ch_list.size(), NULL);
	vector<TH1F*> hist_peak_amp_v(ch_list.size(), NULL);
	vector<TH1F*> hist_peak_area_v(ch_list.size(), NULL);
	vector<TH1F*> hist_peak_length_v(ch_list.size(), NULL);
	vector<TH1F*> hist_peak_area_ev_v(ch_list.size(), NULL);

	vector<TH1F*> hist_peak_area_bkg_v(ch_list.size(), NULL);
	vector<TH1F*> hist_peak_area_S1_v(ch_list.size(), NULL);
	vector<TH1F*> hist_peak_area_S2_v(ch_list.size(), NULL);

	vector<TH1F*> hist_n_peaks_bkg_v(ch_list.size(), NULL);
	vector<TH1F*> hist_n_peaks_S1_v(ch_list.size(), NULL);
	vector<TH1F*> hist_n_peaks_S2_v(ch_list.size(), NULL);

	vector<TH2F*> hist_peak_area_ev_vs_evnum_v(ch_list.size(), NULL);
	vector<TProfile*> prof_hist_peak_area_ev_vs_evnum_v(ch_list.size(), NULL);
	vector<TH2F*> hist_peak_amp_peak_area_v(ch_list.size(), NULL);



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
		ostringstream hist_peak_length_name;
		ostringstream hist_peak_area_ev_name;
		ostringstream hist_peak_area_ev_vs_evnum_name;
		ostringstream hist_peak_amp_peak_area_name;

		ostringstream hist_peak_area_bkg_name;
		ostringstream hist_peak_area_S1_name;
		ostringstream hist_peak_area_S2_name;

		ostringstream hist_n_peaks_bkg_name;
		ostringstream hist_n_peaks_S1_name;
		ostringstream hist_n_peaks_S2_name;
		//ostringstream gr_time_spectrum_name;

		hist_ymin_name << "hist_ymin_ch_" << ch_list[ch] << " (" << rd_inf.GetChNameList()[ch] << ")";
		hist_ymax_name << "hist_ymax_ch_" << ch_list[ch] << " (" << rd_inf.GetChNameList()[ch] << ")";
		hist_baseline_mean_name << "hist_baseline_mean_ch_" << ch_list[ch] << " (" << rd_inf.GetChNameList()[ch] << ")";
		hist_baseline_sigma_name << "hist_baseline_sigma_ch_" << ch_list[ch] << " (" << rd_inf.GetChNameList()[ch] << ")";
		hist_n_peaks_name << "hist_n_peaks_ch_" << ch_list[ch] << " (" << rd_inf.GetChNameList()[ch] << ")";
		hist_peak_time_name << "hist_peak_time_ch_" << ch_list[ch] << " (" << rd_inf.GetChNameList()[ch] << ")";
		hist_peak_amp_name << "hist_peak_amp_ch_" << ch_list[ch] << " (" << rd_inf.GetChNameList()[ch] << ")";
		hist_peak_area_name << "hist_peak_area_ch_" << ch_list[ch] << " (" << rd_inf.GetChNameList()[ch] << ")";
		hist_peak_length_name << "hist_peak_length_ch_" << ch_list[ch] << " (" << rd_inf.GetChNameList()[ch] << ")";
		hist_peak_area_ev_name << "hist_peak_area_ev_ch_" << ch_list[ch] << " (" << rd_inf.GetChNameList()[ch] << ")";
		hist_peak_area_ev_vs_evnum_name << "hist_peak_area_ev_vs_evnum_ch_" << ch_list[ch] << " (" << rd_inf.GetChNameList()[ch] << ")";
		hist_peak_amp_peak_area_name << "hist_peak_amp_peak_area_ch_" << ch_list[ch] << " (" << rd_inf.GetChNameList()[ch] << ")";
		
		hist_peak_area_bkg_name << "hist_peak_area_bkg_ch" << ch_list[ch] << " (" << rd_inf.GetChNameList()[ch] << ")";
		hist_peak_area_S1_name << "hist_peak_area_S1_ch" << ch_list[ch] << " (" << rd_inf.GetChNameList()[ch] << ")";
		hist_peak_area_S2_name << "hist_peak_area_S2_ch" << ch_list[ch] << " (" << rd_inf.GetChNameList()[ch] << ")";

		hist_n_peaks_bkg_name << "hist_n_peaks_bkg_ch" << ch_list[ch] << " (" << rd_inf.GetChNameList()[ch] << ")";
		hist_n_peaks_S1_name << "hist_n_peaks_S1_ch" << ch_list[ch] << " (" << rd_inf.GetChNameList()[ch] << ")";
		hist_n_peaks_S2_name << "hist_n_peaks_S2_ch" << ch_list[ch] << " (" << rd_inf.GetChNameList()[ch] << ")";
		//gr_time_spectrum_name << "gr_time_spectrum_ch_" << ch_list[ch];

		hist_ymin_v[ch] = new TH1F(hist_ymin_name.str().c_str(), hist_ymin_name.str().c_str(), 1000, -1100, 1100);
		hist_ymax_v[ch] = new TH1F(hist_ymax_name.str().c_str(), hist_ymax_name.str().c_str(), 1000, -1100, 1100);
		hist_baseline_mean_v[ch] = new TH1F(hist_baseline_mean_name.str().c_str(), hist_baseline_mean_name.str().c_str(), 1000, -1100, 1100);
		hist_baseline_sigma_v[ch] = new TH1F(hist_baseline_sigma_name.str().c_str(), hist_baseline_sigma_name.str().c_str(), 1000, 0, hist_baseline_sigma_xmax);
		hist_n_peaks_v[ch] = new TH1F(hist_n_peaks_name.str().c_str(), hist_n_peaks_name.str().c_str(), rd_inf.GetHistNpeaksNbinsList()[ch], rd_inf.GetHistNpeaksXminList()[ch], rd_inf.GetHistNpeaksXmaxList()[ch]);
		hist_peak_time_v[ch] = new TH1F(hist_peak_time_name.str().c_str(), hist_peak_time_name.str().c_str(), rd_inf.GetHistPeakTimeNbinsList()[ch], rd_inf.GetHistPeakTimeXminList()[ch], rd_inf.GetHistPeakTimeXmaxList()[ch]);
		hist_peak_amp_v[ch] = new TH1F(hist_peak_amp_name.str().c_str(), hist_peak_amp_name.str().c_str(), rd_inf.GetHistPeakAmpNbinsList()[ch], rd_inf.GetHistPeakAmpXminList()[ch], rd_inf.GetHistPeakAmpXmaxList()[ch]);
		hist_peak_area_v[ch] = new TH1F(hist_peak_area_name.str().c_str(), hist_peak_area_name.str().c_str(), rd_inf.GetHistPeakAreaNbinsList()[ch], rd_inf.GetHistPeakAreaXminList()[ch], rd_inf.GetHistPeakAreaXmaxList()[ch]*SiPM_axis_area_factor_for_calib);
		hist_peak_amp_peak_area_v[ch] = new TH2F(hist_peak_amp_peak_area_name.str().c_str(), hist_peak_amp_peak_area_name.str().c_str(), 
			rd_inf.GetHistPeakAmpNbinsList()[ch], rd_inf.GetHistPeakAmpXminList()[ch], rd_inf.GetHistPeakAmpXmaxList()[ch],
			rd_inf.GetHistPeakAreaNbinsList()[ch], rd_inf.GetHistPeakAreaXminList()[ch], rd_inf.GetHistPeakAreaXmaxList()[ch]);



		if (ch != 9)
			hist_peak_length_v[ch] = new TH1F(hist_peak_length_name.str().c_str(), hist_peak_length_name.str().c_str(), 300, 0, 2000);
		else
			hist_peak_length_v[ch] = new TH1F(hist_peak_length_name.str().c_str(), hist_peak_length_name.str().c_str(), 300, 0, 20000);

		hist_peak_area_ev_v[ch] = new TH1F(hist_peak_area_ev_name.str().c_str(), hist_peak_area_ev_name.str().c_str(), rd_inf.GetHistPeakAreaEvNbinsList()[ch], rd_inf.GetHistPeakAreaEvXminList()[ch], rd_inf.GetHistPeakAreaEvXmaxList()[ch]);
		hist_peak_area_ev_vs_evnum_v[ch] = new TH2F(hist_peak_area_ev_vs_evnum_name.str().c_str(), hist_peak_area_ev_vs_evnum_name.str().c_str(),
			110, 0, tree->GetEntries(), 
			rd_inf.GetHistPeakAreaEvNbinsList()[ch], rd_inf.GetHistPeakAreaEvXminList()[ch], rd_inf.GetHistPeakAreaEvXmaxList()[ch]);
		
		hist_peak_area_bkg_v[ch] = new TH1F(hist_peak_area_bkg_name.str().c_str(), hist_peak_area_bkg_name.str().c_str(), rd_inf.GetHistPeakAreaEvNbinsList()[ch], 0, 100);
		hist_peak_area_S1_v[ch] = new TH1F(hist_peak_area_S1_name.str().c_str(), hist_peak_area_S1_name.str().c_str(), rd_inf.GetHistPeakAreaEvNbinsList()[ch], 0, 50);//test
		hist_peak_area_S2_v[ch] = new TH1F(hist_peak_area_S2_name.str().c_str(), hist_peak_area_S2_name.str().c_str(), rd_inf.GetHistPeakAreaEvNbinsList()[ch], 0, 100);

		hist_n_peaks_bkg_v[ch] = new TH1F(hist_n_peaks_bkg_name.str().c_str(), hist_n_peaks_bkg_name.str().c_str(), rd_inf.GetHistPeakAreaEvNbinsList()[ch], 0, 5);
		hist_n_peaks_S1_v[ch] = new TH1F(hist_n_peaks_S1_name.str().c_str(), hist_n_peaks_S1_name.str().c_str(), rd_inf.GetHistPeakAreaEvNbinsList()[ch], 0, 5);
		hist_n_peaks_S2_v[ch] = new TH1F(hist_n_peaks_S2_name.str().c_str(), hist_n_peaks_S2_name.str().c_str(), rd_inf.GetHistPeakAreaEvNbinsList()[ch], 0, 50);

	}

	//define graphs
	//vector<TGraph*> gr_time_spectrum_v(ch_list.size(), NULL);
	//vector< vector<double> > peak_time_v(ch_list.size());
	vector< vector<double> > peak_amp_v(ch_list.size());
	vector< vector<double> > peak_area_v(ch_list.size());
	vector<double> peak_time_sum_SiPM;
	vector<double> peak_time_sum_PMTslow;
	vector<double> peak_time_sum_PMTfast;

	COUT(tree->GetEntries());


	int n_ev_after_cuts = 0;





















	//ev loop_1
	for (unsigned int ev = 0; ev < tree->GetEntries() /*6000*/; ev++)
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

		bool cut_3 = true;
		//for (unsigned int ch = 0; ch < ch_list.size(); ch++)
		//{
		//	cut_3 = ( cut_3 && (event->ymax[ch] < 500) );
		//}

		int ch_top_counter = 10;//index (not name)
		int ch_bot_counter = 11;//index (not name)
		bool is_narrow_top_counter_peak_time = false;
		bool is_narrow_bot_counter_peak_time = false;
		double peak_time_min = 31800;
		double peak_time_max = 32000;
		for (int peak_id = 0; peak_id < event->peaks[ch_top_counter]->peak_time.size(); peak_id++)
		{			
			if (event->peaks[ch_top_counter]->peak_time[peak_id] > peak_time_min &&
				event->peaks[ch_top_counter]->peak_time[peak_id] < peak_time_max)
			{
				is_narrow_top_counter_peak_time = true;
			}
			else
			{				
				is_narrow_top_counter_peak_time = false;
				break;
			}
		}

		
		
		for (int peak_id = 0; peak_id < event->peaks[ch_bot_counter]->peak_time.size(); peak_id++)
		{
			if (event->peaks[ch_bot_counter]->peak_time[peak_id] > peak_time_min &&
				event->peaks[ch_bot_counter]->peak_time[peak_id] < peak_time_max)
			{
				is_narrow_bot_counter_peak_time = true;
			}
			else
			{
				is_narrow_bot_counter_peak_time = false;
				break;
			}
		}

		bool is_narrow_top_counter_peak_amp = false;
		for (int peak_id = 0; peak_id < event->peaks[ch_top_counter]->peak_amp.size(); peak_id++)
		{
			if (event->peaks[ch_top_counter]->peak_amp[peak_id] > 100 &&
				event->peaks[ch_top_counter]->peak_amp[peak_id] < 400)
			{
				is_narrow_top_counter_peak_amp = true;
			}
			else
			{
				is_narrow_top_counter_peak_amp = false;
				break;
			}
		}

		bool is_narrow_bot_counter_peak_amp = false;
		for (int peak_id = 0; peak_id < event->peaks[ch_bot_counter]->peak_amp.size(); peak_id++)
		{
			if (event->peaks[ch_bot_counter]->peak_amp[peak_id] > 15 &&
				event->peaks[ch_bot_counter]->peak_amp[peak_id] < 55)
			{
				is_narrow_bot_counter_peak_amp = true;
			}
			else
			{
				is_narrow_bot_counter_peak_amp = false;
				break;
			}
		}

		bool no_PMT_saturation = (event->ymax[1] < 1000) && (event->ymax[2] < 1000) && (event->ymax[3] < 1000) && (event->ymax[4] < 1000);

		bool is_trigger_in = false;
		int ch_trigger = 9;
		for (int peak_id = 0; peak_id < event->peaks[ch_trigger]->peak_amp.size(); peak_id++)
		{
			if (event->peaks[ch_trigger]->peak_time[peak_id] > /*31000*/ /*95000*/ S2_trigger_start &&
				event->peaks[ch_trigger]->peak_time[peak_id] < /*35000*/ /*100000*/ S2_trigger_stop)
			{
				is_trigger_in = true;
			}
			else
			{
				is_trigger_in = false;
				break;
			}
		}

		
		double PE_allSiPMs_S2_areabased_cut = 0;
		if (no_PMT_saturation  && is_trigger_in)
		{
			for (int ch = 0; ch < ch_list.size(); ch++)
			{
				if (rd_inf.GetChNameList()[ch] == "SiPM")
				{
					for (int peak_id = 0; peak_id < event->peaks[ch]->peak_amp.size(); peak_id++)
					{
						if (event->peaks[ch]->peak_time[peak_id] > /*85000*/ /*25000*/ S2_start &&
							event->peaks[ch]->peak_time[peak_id] < /*110000*/ /*40000*/ S2_stop)
						{
							PE_allSiPMs_S2_areabased_cut += (event->peaks[ch]->peak_area[peak_id] / ch_calib[ch]);
							//cout << event->peaks[ch]->peak_area[peak_id] << endl;
						}
					}
				}
			}
		}
		//bool is_peak_in_allSiPM_spectrum = PE_allSiPMs_S2_areabased_cut > 2 && PE_allSiPMs_S2_areabased_cut < 12;// 18kV 191107
		//bool is_peak_in_allSiPM_spectrum = PE_allSiPMs_S2_areabased_cut > 5 && PE_allSiPMs_S2_areabased_cut < 18;// 20kV 200910
		//bool is_peak_in_allSiPM_spectrum = PE_allSiPMs_S2_areabased_cut > 4.5 && PE_allSiPMs_S2_areabased_cut < 13.5;// 18kV 200910
		//bool is_peak_in_allSiPM_spectrum = PE_allSiPMs_S2_areabased_cut > 5 && PE_allSiPMs_S2_areabased_cut < 10;// 16kV 200910
		//bool is_peak_in_allSiPM_spectrum = PE_allSiPMs_S2_areabased_cut > 4 && PE_allSiPMs_S2_areabased_cut < 8;// 12kV 200910
		bool is_peak_in_allSiPM_spectrum = PE_allSiPMs_S2_areabased_cut > S2_min_SiPM && PE_allSiPMs_S2_areabased_cut < S2_max_SiPM;

		double PE_allSiPMs_S2_test_pretrigger_areabased_cut = 0;
		if (no_PMT_saturation  && is_trigger_in)
		{
			for (int ch = 0; ch < ch_list.size(); ch++)
			{
				if (rd_inf.GetChNameList()[ch] == "SiPM")
				{
					for (int peak_id = 0; peak_id < event->peaks[ch]->peak_amp.size(); peak_id++)
					{
						if (event->peaks[ch]->peak_time[peak_id] > S2_test_pretrigger_start &&
							event->peaks[ch]->peak_time[peak_id] < S2_test_pretrigger_stop)
						{
							PE_allSiPMs_S2_test_pretrigger_areabased_cut += (event->peaks[ch]->peak_area[peak_id] / ch_calib[ch]);
						}
					}
				}
			}
		}
		bool is_peak_in_allSiPM_S2_test_pretrigger_spectrum = PE_allSiPMs_S2_test_pretrigger_areabased_cut < S2_test_pretrigger_SiPM_max;

	
		//main_cuts
		if (/*is_narrow_top_counter_peak_time && is_narrow_bot_counter_peak_time 
			&& is_narrow_top_counter_peak_amp && is_narrow_bot_counter_peak_amp && */ 
			no_PMT_saturation /*&& is_trigger_in*/ /*&& is_peak_in_allSiPM_S2_test_pretrigger_spectrum && is_peak_in_allSiPM_spectrum*/  /*true*/)//cuts
		{
			n_ev_after_cuts++;
			double peak_area_ev_allSiPMs = 0;
			double PE_allPMT_S2 = 0;
			double PE_allSiPMs_S2 = 0;
			double PE_allSiPMs_S2_areabased = 0;
			double PE_allSiPMs_S2_test_pretrigger_areabased = 0;
			
			//ch loop
			for (unsigned int ch = 0; ch < ch_list.size(); ch++)
			{

				hist_ymin_v[ch]->Fill(event->ymin[ch]);
				hist_ymax_v[ch]->Fill(event->ymax[ch]);
				hist_baseline_mean_v[ch]->Fill(event->baseline_mean[ch]);
				hist_baseline_sigma_v[ch]->Fill(event->baseline_sigma[ch]);
				hist_n_peaks_v[ch]->Fill(event->peaks[ch]->peak_time.size());

				double peak_area_ev = 0;

				double peak_area_bkg = 0;
				double peak_area_S1 = 0;
				double peak_area_S2 = 0;
				double peak_area_S2_test_pretrigger = 0;

				double n_peaks_bkg = 0;
				double n_peaks_S1 = 0;
				double n_peaks_S2 = 0;

				double PMT1_area_ev = 0;
				double PMT2_area_ev = 0;
				double PMT3_area_ev = 0;
				double PMT4_area_ev = 0;

				//peak loop
				for (unsigned int peak_id = 0; peak_id < event->peaks[ch]->peak_time.size(); peak_id++)
				{
					//	peak_time_v[ch].push_back( event->peaks[ch]->peak_time[peak_id] );
					//	peak_amp_v[ch].push_back( event->peaks[ch]->peak_amp[peak_id] );

					//cuts
					bool is_in_5_30_us = (event->peaks[ch]->avr_peak_time[peak_id] > 5000 && event->peaks[ch]->avr_peak_time[peak_id] < 30000);
					bool is_in_5_80_us = (event->peaks[ch]->avr_peak_time[peak_id] > 5000 && event->peaks[ch]->avr_peak_time[peak_id] < 80000);
					bool is_noise = (event->peaks[ch]->avr_peak_time[peak_id] > 31000 && event->peaks[ch]->avr_peak_time[peak_id] < 34000);
					bool is_in_15_24_us = event->peaks[ch]->peak_time[peak_id] > 15000 && event->peaks[ch]->peak_time[peak_id] < 24000;
					bool is_in_31_38_us = event->peaks[ch]->peak_time[peak_id] > 31000 && event->peaks[ch]->peak_time[peak_id] < 38000;
					bool is_in_31_40_us = event->peaks[ch]->peak_time[peak_id] > 31000 && event->peaks[ch]->peak_time[peak_id] < 40000;
					bool is_in_34_100_us = event->peaks[ch]->peak_time[peak_id] > 34000 && event->peaks[ch]->peak_time[peak_id] < 100000;
					bool is_in_70_100_us = event->peaks[ch]->peak_time[peak_id] > 70000 && event->peaks[ch]->peak_time[peak_id] < 100000;
					bool is_in_110_155_us = event->peaks[ch]->peak_time[peak_id] > 110000 && event->peaks[ch]->peak_time[peak_id] < 155000;
					bool is_in_60_155_us = event->peaks[ch]->peak_time[peak_id] > 60000 && event->peaks[ch]->peak_time[peak_id] < 155000;
					bool is_in_61_63_us = event->peaks[ch]->peak_time[peak_id] > 61540 && event->peaks[ch]->peak_time[peak_id] < 63220;
					bool is_in_62_64_us = event->peaks[ch]->peak_time[peak_id] > 62980 && event->peaks[ch]->peak_time[peak_id] < 64660;
					bool is_in_42_44us = event->peaks[ch]->peak_time[peak_id] > 42980 && event->peaks[ch]->peak_time[peak_id] < 44660;

					double peak_length_ch = (event->peaks[ch]->peak_start_stop_poits[peak_id].second -
						event->peaks[ch]->peak_start_stop_poits[peak_id].first) * ns_per_point;
					bool is_short_peak_ch = peak_length_ch > 250 && peak_length_ch < 900;

					bool is_amp = event->peaks[ch]->peak_amp[peak_id] > 15;

					if (event->peaks[ch]->peak_time[peak_id] > Bkg_start && event->peaks[ch]->peak_time[peak_id] < Bkg_stop)
					{
						peak_area_bkg += event->peaks[ch]->peak_area[peak_id];
						n_peaks_bkg++;
					}

					
					
					if (event->peaks[ch]->peak_time[peak_id] > S1_start && event->peaks[ch]->peak_time[peak_id] < S1_stop)
					{
						peak_area_S1 += event->peaks[ch]->peak_area[peak_id];
						n_peaks_S1++;
					}

					
					if (event->peaks[ch]->peak_time[peak_id] > S2_start && event->peaks[ch]->peak_time[peak_id] < S2_stop)
					{
						peak_area_S2 += event->peaks[ch]->peak_area[peak_id];
						n_peaks_S2++;
					}

					if (event->peaks[ch]->peak_time[peak_id] > S2_test_pretrigger_start && event->peaks[ch]->peak_time[peak_id] < S2_test_pretrigger_stop)
					{
						peak_area_S2_test_pretrigger += event->peaks[ch]->peak_area[peak_id];
					}

					
					bool peak_cut = true;
					if (is_calib) peak_cut = event->peaks[ch]->peak_time[peak_id] > 0 && event->peaks[ch]->peak_time[peak_id] < 30000;
					//cut_peak usefull for calibrations
					//bool cut_tmp = event->peaks[ch]->peak_time[peak_id] > 0 && event->peaks[ch]->peak_time[peak_id] < 30000;
					if (/*cut_tmp*/ /*is_in_42_44us*/ peak_cut /*is_short_peak_ch*/ /*&& is_amp && is_in_60_155_us*/)
					{
						peak_area_ev += event->peaks[ch]->peak_area[peak_id];
						
						hist_peak_time_v[ch]->Fill(event->peaks[ch]->peak_time[peak_id]);
						hist_peak_amp_v[ch]->Fill(event->peaks[ch]->peak_amp[peak_id]);
						hist_peak_area_v[ch]->Fill(event->peaks[ch]->peak_area[peak_id]);
						double peak_length = (event->peaks[ch]->peak_start_stop_poits[peak_id].second - 
							event->peaks[ch]->peak_start_stop_poits[peak_id].first) * ns_per_point;
						hist_peak_length_v[ch]->Fill(peak_length);
						hist_peak_amp_peak_area_v[ch]->Fill(event->peaks[ch]->peak_amp[peak_id], event->peaks[ch]->peak_area[peak_id]);

						peak_amp_v[ch].push_back(event->peaks[ch]->peak_amp[peak_id]);
						peak_area_v[ch].push_back(event->peaks[ch]->peak_area[peak_id]);

						if (find(ch_list_good_SiPMs.begin(), ch_list_good_SiPMs.end(), ch_list[ch]) != ch_list_good_SiPMs.end())
						{
							hist_peak_time_all_SiPMs->Fill(event->peaks[ch]->peak_time[peak_id]);
							vec_peak_time_good_SiPMs.push_back(event->peaks[ch]->peak_time[peak_id]);
							f_out_allSiPM_time_spectrum << (event->peaks[ch]->peak_time[peak_id]) / 1000.0 << endl;

							peak_area_ev_allSiPMs += event->peaks[ch]->peak_area[peak_id]/ch_calib[ch];	//test						

							//cout << "ch = " << ch_list[ch]  << endl;
						}

						if (rd_inf.GetChNameList()[ch] == "PMT1_slow" || rd_inf.GetChNameList()[ch] == "PMT2_slow" || 
							rd_inf.GetChNameList()[ch] == "PMT3_slow" || rd_inf.GetChNameList()[ch] == "PMT4_slow")
						{
							hist_peak_time_all_PMTs_slow->Fill(event->peaks[ch]->peak_time[peak_id]);
							f_out_allPMTs_slow_time_spectrum << (event->peaks[ch]->peak_time[peak_id]) / 1000.0 << endl;
							
						}

						if (rd_inf.GetChNameList()[ch] == "PMT1_fast" || rd_inf.GetChNameList()[ch] == "PMT2_fast" ||
							rd_inf.GetChNameList()[ch] == "PMT3_fast" || rd_inf.GetChNameList()[ch] == "PMT4_fast")
						{
							hist_peak_time_all_PMTs_fast->Fill(event->peaks[ch]->peak_time[peak_id]);
							f_out_allPMTs_fast_time_spectrum << (event->peaks[ch]->peak_time[peak_id]) / 1000.0 << endl;
						}

						if (rd_inf.GetChNameList()[ch] == "PMT2_slow" || /*rd_inf.GetChNameList()[ch] == "Sum_3PMT_slow" ||*/
							rd_inf.GetChNameList()[ch] == "PMT3_slow" || rd_inf.GetChNameList()[ch] == "PMT4_slow")
						{
							hist_peak_time_3PMTs_slow->Fill(event->peaks[ch]->peak_time[peak_id]);
							f_out_3PMTs_slow_time_spectrum << (event->peaks[ch]->peak_time[peak_id]) / 1000.0 << endl;
						}

						if (rd_inf.GetChNameList()[ch] == "Sum_4PMT_trigg")
						{
							hist_peak_time_PMTtrigg_slow->Fill(event->peaks[ch]->peak_time[peak_id]);
						}

						if (rd_inf.GetChNameList()[ch] == "PMT1_slow")
						{
							hist_peak_time_1PMT_slow->Fill(event->peaks[ch]->peak_time[peak_id]);
							f_out_1PMT_slow_time_spectrum << (event->peaks[ch]->peak_time[peak_id]) / 1000.0 << endl;
							PMT1_area_ev += event->peaks[ch]->peak_area[peak_id];
						}

						if (rd_inf.GetChNameList()[ch] == "PMT2_slow")
						{
							//hist_peak_time_2PMT_slow->Fill(event->peaks[ch]->peak_time[peak_id]);
							//f_out_2PMT_slow_time_spectrum << (event->peaks[ch]->peak_time[peak_id]) / 1000.0 << endl;
							PMT2_area_ev += event->peaks[ch]->peak_area[peak_id];
						}

						if (rd_inf.GetChNameList()[ch] == "PMT3_slow")
						{
							//hist_peak_time_3PMT_slow->Fill(event->peaks[ch]->peak_time[peak_id]);
							//f_out_3PMT_slow_time_spectrum << (event->peaks[ch]->peak_time[peak_id]) / 1000.0 << endl;
							PMT3_area_ev += event->peaks[ch]->peak_area[peak_id];
						}

						if (rd_inf.GetChNameList()[ch] == "PMT4_slow")
						{
							//hist_peak_time_4PMT_slow->Fill(event->peaks[ch]->peak_time[peak_id]);
							//f_out_4PMT_slow_time_spectrum << (event->peaks[ch]->peak_time[peak_id]) / 1000.0 << endl;
							PMT4_area_ev += event->peaks[ch]->peak_area[peak_id];
							//f_out_4PMT_slow_area_ev_spectrum << PMT4_area_ev << endl;
						}


						//cout << "ev = " << ev << "; ch = " << ch_list[ch] << "; peak_id = " << peak_id << "; peak_area = " << event->peaks[ch]->peak_area[peak_id] << endl;
					}
				}//peak loop end

				hist_peak_area_ev_v[ch]->Fill(peak_area_ev);
				hist_peak_area_ev_vs_evnum_v[ch]->Fill(ev, peak_area_ev);

				hist_peak_area_bkg_v[ch]->Fill(peak_area_bkg / ch_calib[ch]);
				hist_peak_area_S1_v[ch]->Fill(peak_area_S1 / ch_calib[ch]);
				hist_peak_area_S2_v[ch]->Fill(peak_area_S2 / ch_calib[ch]);

				hist_n_peaks_bkg_v[ch]->Fill(n_peaks_bkg);
				hist_n_peaks_S1_v[ch]->Fill(n_peaks_S1);
				hist_n_peaks_S2_v[ch]->Fill(n_peaks_S2);

				f_out_1PMT_slow_area_ev_spectrum << PMT1_area_ev << endl;
				f_out_2PMT_slow_area_ev_spectrum << PMT2_area_ev << endl;
				f_out_3PMT_slow_area_ev_spectrum << PMT3_area_ev << endl;
				f_out_4PMT_slow_area_ev_spectrum << PMT4_area_ev << endl;

				if (rd_inf.GetChNameList()[ch] == "SiPM")
				{
					PE_allSiPMs_S2 += n_peaks_S2;
					PE_allSiPMs_S2_areabased += (peak_area_S2 / ch_calib[ch]);
					PE_allSiPMs_S2_test_pretrigger_areabased += (peak_area_S2_test_pretrigger / ch_calib[ch]);
				}				

				if (rd_inf.GetChNameList()[ch] == "PMT1_slow" || rd_inf.GetChNameList()[ch] == "PMT2_slow" ||
					rd_inf.GetChNameList()[ch] == "PMT3_slow" || rd_inf.GetChNameList()[ch] == "PMT4_slow")
				{
					PE_allPMT_S2 += (peak_area_S2 / ch_calib[ch]);
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

			}//ch loop end			
			
			hist_peak_area_ev_vs_evnum_all_SiPMs->Fill(ev, peak_area_ev_allSiPMs);
			hist_PE_allPMT_S2->Fill(PE_allPMT_S2);
			hist_PE_allSiPMs_S2->Fill(PE_allSiPMs_S2);
			hist_PE_allSiPMs_S2_areabased->Fill(PE_allSiPMs_S2_areabased);
			hist_PE_allSiPMs_S2_test_pretrigger_areabased->Fill(PE_allSiPMs_S2_test_pretrigger_areabased);
			
		}
		event->Clear();

	}
	cout << endl << "n_ev_after_cuts = " << n_ev_after_cuts << endl;

	vector<double> peak_area_bkg_avr(ch_list.size());
	vector<double> peak_area_S1_min_bkg_avr(ch_list.size());
	vector<double> peak_area_S2_min_bkg_avr(ch_list.size());

	vector<double> n_peaks_bkg_avr(ch_list.size());
	vector<double> n_peaks_S1_min_bkg_avr(ch_list.size());
	vector<double> n_peaks_S2_min_bkg_avr(ch_list.size());

	//TH1F* hist_peak_PE_all_PMTs_slow_S1 = new TH1F("hist_peak_PE_all_PMTs_slow_S1", "hist_peak_PE_all_PMTs_slow_S1", 160, 0, 30);
	//TH1F* hist_peak_PE_all_PMTs_slow_S2 = new TH1F("hist_peak_PE_all_PMTs_slow_S2", "hist_peak_PE_all_PMTs_slow_S2", 160, 0, 100);
	//TH1F* hist_peak_PE_all_SiPMs_slow_S1 = new TH1F("hist_peak_PE_all_SiPMs_slow_S1", "hist_peak_PE_all_SiPMs_slow_S1", 160, 0, 30);
	//TH1F* hist_peak_PE_all_SiPMs_slow_S2 = new TH1F("hist_peak_PE_all_SiPMs_slow_S2", "hist_peak_PE_all_SiPMs_slow_S2", 160, 0, 30);
	//ch loop
	double PE_all_PMTs_slow_S1 = 0;
	double PE_all_PMTs_slow_S2 = 0;
	double PE_all_SiPMs_S1 = 0;
	double PE_all_SiPMs_S2 = 0;
	//double PE_all_PMTs_slow_bkg = 0;
	double PE_all_PMTs_slow_bkg_S1 = 0;
	double PE_all_PMTs_slow_bkg_S2 = 0;
	//double PE_all_SiPMs_bkg = 0;
	double PE_all_SiPMs_bkg_S1 = 0;
	double PE_all_SiPMs_bkg_S2 = 0;
	
	for (unsigned int ch = 0; ch < ch_list.size(); ch++)
	{
		peak_area_bkg_avr[ch] = hist_peak_area_bkg_v[ch]->GetMean() / Bkg_dt; //Bkg per us
		//f_out_peak_area_bkg_avg << peak_area_bkg_avr[ch] << endl;
		
		peak_area_S1_min_bkg_avr[ch] = hist_peak_area_S1_v[ch]->GetMean() - peak_area_bkg_avr[ch] * S1_dt;
		peak_area_S2_min_bkg_avr[ch] = hist_peak_area_S2_v[ch]->GetMean() - peak_area_bkg_avr[ch] * S2_dt;

		/*cout << ch << " " << rd_inf.GetChNameList()[ch] << "; " << hist_peak_area_S1_v[ch]->GetMean() <<
			"; " << hist_peak_area_S2_v[ch]->GetMean() << "; " << peak_area_bkg_avr[ch] << endl;*/

		n_peaks_bkg_avr[ch] = hist_n_peaks_bkg_v[ch]->GetMean();
		n_peaks_S1_min_bkg_avr[ch] = hist_n_peaks_S1_v[ch]->GetMean() - n_peaks_bkg_avr[ch];
		n_peaks_S2_min_bkg_avr[ch] = hist_n_peaks_S2_v[ch]->GetMean() - n_peaks_bkg_avr[ch];

		/*cout << ch << " " << rd_inf.GetChNameList()[ch] << "; " << hist_n_peaks_S1_v[ch]->GetMean() <<
			"; " << hist_n_peaks_S2_v[ch]->GetMean() << "; " << n_peaks_bkg_avr[ch] << endl;*/


		for (int i = 1; i <= 4; i++)
		{
			ostringstream ch_name;
			ch_name << "PMT" << i << "_slow";
			if (rd_inf.GetChNameList()[ch] == ch_name.str())
			{
				/*cout << ch_name.str() << " S1[PE] = " << peak_area_S1_min_bkg_avr[ch] << endl;
				cout << ch_name.str() << " S2[PE] = " << peak_area_S2_min_bkg_avr[ch] << endl;
				cout << endl;*/

				//cout << ch_name.str() << " S1[PE] = " << peak_area_S1_min_bkg_avr[ch] << "; " << " S2[PE] = " << peak_area_S2_min_bkg_avr[ch] << endl;
				//cout << endl;
				
				//v1
				//f_out_PE_S1_S2 << ch_name.str() << " S1[PE] = " << peak_area_S1_min_bkg_avr[ch] << "; " << " S2[PE] = " << peak_area_S2_min_bkg_avr[ch] << endl;

				//v2
				f_out_PE_S1_S2 << ch_name.str() <<
					" S1[PE] (+ bkg) = " << hist_peak_area_S1_v[ch]->GetMean() << "; " <<
					" S1[PE] (bkg) = " << peak_area_bkg_avr[ch] * S1_dt << "; " <<
					" S1[PE] (- bkg) = " << peak_area_S1_min_bkg_avr[ch] << "; " <<
					" S2[PE] (- bkg) = " << peak_area_S2_min_bkg_avr[ch] << "; " << 
					" S2[PE] (bkg) = " << peak_area_bkg_avr[ch] * S2_dt << "; " << endl;

				PE_all_PMTs_slow_S1 += peak_area_S1_min_bkg_avr[ch];
				PE_all_PMTs_slow_S2 += peak_area_S2_min_bkg_avr[ch];
				PE_all_PMTs_slow_bkg_S1 += peak_area_bkg_avr[ch]*S1_dt;
				PE_all_PMTs_slow_bkg_S2 += peak_area_bkg_avr[ch] * S2_dt;
			}
		}

		if (rd_inf.GetChNameList()[ch] == "SiPM")
		{
			//cout << rd_inf.GetChNameList()[ch] << "_" << rd_inf.GetChList()[ch] << "; S1[PE](n_peaks) = " << n_peaks_S1_min_bkg_avr[ch] << "; S1[PE](area) = " << peak_area_S1_min_bkg_avr[ch] <<
			//	"; S2[PE](n_peaks) = " << n_peaks_S2_min_bkg_avr[ch] << "; S2[PE](peak area) = " << peak_area_S2_min_bkg_avr[ch] << endl;
			
			
			f_out_PE_S1_S2 << rd_inf.GetChNameList()[ch] << "_" << rd_inf.GetChList()[ch] << "; S1[PE](n_peaks) = " << n_peaks_S1_min_bkg_avr[ch] << "; S1[PE](area) = " << peak_area_S1_min_bkg_avr[ch] <<
				"; S2[PE](n_peaks) = " << n_peaks_S2_min_bkg_avr[ch] << "; S2[PE](peak area) = " << peak_area_S2_min_bkg_avr[ch] << endl;

			PE_all_SiPMs_S1 += peak_area_S1_min_bkg_avr[ch];
			PE_all_SiPMs_S2 += peak_area_S2_min_bkg_avr[ch];
			PE_all_SiPMs_bkg_S1 += peak_area_bkg_avr[ch]*S1_dt;
			PE_all_SiPMs_bkg_S2 += peak_area_bkg_avr[ch] * S2_dt;
		}


	}
		

	/*COUT(PE_all_PMTs_slow_S1);
	COUT(PE_all_PMTs_slow_S2);
	COUT(PE_all_SiPMs_S1);
	COUT(PE_all_SiPMs_S2);*/

	f_out_PE_S1_S2 << "PE_all_PMTs_slow_S1 =" << PE_all_PMTs_slow_S1 << endl;
	f_out_PE_S1_S2 << "PE_all_PMTs_slow_S2 =" << PE_all_PMTs_slow_S2 << endl;
	f_out_PE_S1_S2 << "PE_all_SiPMs_S1 =" << PE_all_SiPMs_S1 << endl;
	f_out_PE_S1_S2 << "PE_all_SiPMs_S2 =" << PE_all_SiPMs_S2 << endl; 
	f_out_PE_S1_S2 << "PE_all_PMTs_slow_bkg_S1 = " << PE_all_PMTs_slow_bkg_S1 << endl;
	f_out_PE_S1_S2 << "PE_all_PMTs_slow_bkg_S2 = " << PE_all_PMTs_slow_bkg_S2 << endl;
	f_out_PE_S1_S2 << "PE_all_SiPMs_bkg_S1 = " << PE_all_SiPMs_bkg_S1 << endl;
	f_out_PE_S1_S2 << "PE_all_SiPMs_bkg_S2 = " << PE_all_SiPMs_bkg_S2 << endl;


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

	if (draw_var.find("peak_length") != std::string::npos)
	{
		TCanvas *c13 = new TCanvas("c13", "peak_length");
		c13->Divide(2, 2, 0.01, 0.01);
		c13->cd(1);
		hist_peak_length_v[ch_index_for_view_list[0]]->Draw();
		c13->cd(2);
		hist_peak_length_v[ch_index_for_view_list[1]]->Draw();
		c13->cd(3);
		hist_peak_length_v[ch_index_for_view_list[2]]->Draw();
		c13->cd(4);
		hist_peak_length_v[ch_index_for_view_list[3]]->Draw();
	}

	if (draw_var.find("peak_area_ev") != std::string::npos)
	{
		TCanvas *c12 = new TCanvas("c12", "peak_area_ev");
		c12->Divide(2, 2, 0.01, 0.01);
		for (int i = 0; i < 4; i++)
		{
			c12->cd(i + 1);
			gPad->SetLogy();
			int ch = ch_index_for_view_list[i];
			hist_peak_area_ev_v[ch]->Draw();
			hist_peak_area_ev_v[ch]->GetXaxis()->SetTitle("Total area in event [ns*mV]");
			hist_peak_area_ev_v[ch]->GetYaxis()->SetTitle("Counts");
		}
	}

	if (draw_var.find("peak_area_bkg") != std::string::npos)
	{
		TCanvas *c17 = new TCanvas("c17", "peak_area_bkg");
		c17->Divide(2, 2, 0.01, 0.01);
		for (int i = 0; i < 4; i++)
		{
			c17->cd(i + 1);
			gPad->SetLogy();
			int ch = ch_index_for_view_list[i];
			hist_peak_area_bkg_v[ch]->Draw();
			hist_peak_area_bkg_v[ch]->GetXaxis()->SetTitle("Bkg area [ns*mV]");
			hist_peak_area_bkg_v[ch]->GetYaxis()->SetTitle("Counts");
		}
	}

	if (draw_var.find("peak_area_S1") != std::string::npos)
	{
		TCanvas *c18 = new TCanvas("c18", "peak_area_S1");
		c18->Divide(2, 2, 0.01, 0.01);
		for (int i = 0; i < 4; i++)
		{
			c18->cd(i + 1);
			gPad->SetLogy();
			int ch = ch_index_for_view_list[i];
			hist_peak_area_S1_v[ch]->Draw();
			hist_peak_area_S1_v[ch]->GetXaxis()->SetTitle("S1 area [PE]");
			hist_peak_area_S1_v[ch]->GetYaxis()->SetTitle("Counts");
		}
	}

	if (draw_var.find("peak_area_S2") != std::string::npos)
	{
		TCanvas *c19 = new TCanvas("c19", "peak_area_S2");
		c19->Divide(2, 2, 0.01, 0.01);
		for (int i = 0; i < 4; i++)
		{
			c19->cd(i + 1);
			gPad->SetLogy();
			int ch = ch_index_for_view_list[i];
			hist_peak_area_S2_v[ch]->Draw();
			hist_peak_area_S2_v[ch]->GetXaxis()->SetTitle("S2 area [ns*mV]");
			hist_peak_area_S2_v[ch]->GetYaxis()->SetTitle("Counts");
		}
	}

	if (draw_var.find("n_peaks_bkg") != std::string::npos)
	{
		TCanvas *c20 = new TCanvas("c20", "n_peaks_bkg");
		c20->Divide(2, 2, 0.01, 0.01);
		for (int i = 0; i < 4; i++)
		{
			c20->cd(i + 1);
			gPad->SetLogy();
			int ch = ch_index_for_view_list[i];
			hist_n_peaks_bkg_v[ch]->Draw();
			hist_n_peaks_bkg_v[ch]->GetXaxis()->SetTitle("Bkg n_peaks");
			hist_n_peaks_bkg_v[ch]->GetYaxis()->SetTitle("Counts");
		}
	}

	if (draw_var.find("n_peaks_S1") != std::string::npos)
	{
		TCanvas *c21 = new TCanvas("c21", "n_peaks_S1");
		c21->Divide(2, 2, 0.01, 0.01);
		for (int i = 0; i < 4; i++)
		{
			c21->cd(i + 1);
			gPad->SetLogy();
			int ch = ch_index_for_view_list[i];
			hist_n_peaks_S1_v[ch]->Draw();
			hist_n_peaks_S1_v[ch]->GetXaxis()->SetTitle("S1 n_peaks");
			hist_n_peaks_S1_v[ch]->GetYaxis()->SetTitle("Counts");
		}
	}

	if (draw_var.find("n_peaks_S2") != std::string::npos)
	{
		TCanvas *c22 = new TCanvas("c22", "n_peaks_S2");
		c22->Divide(2, 2, 0.01, 0.01);
		for (int i = 0; i < 4; i++)
		{
			c22->cd(i + 1);
			gPad->SetLogy();
			int ch = ch_index_for_view_list[i];
			hist_n_peaks_S2_v[ch]->Draw();
			hist_n_peaks_S2_v[ch]->GetXaxis()->SetTitle("S2 n_peaks");
			hist_n_peaks_S2_v[ch]->GetYaxis()->SetTitle("Counts");
		}
	}

	//if (draw_var.find("PE_SiPM_PMTs_slow_h1") != std::string::npos)
	//{
	//	

	//	
	//	vector<TH1F*> hist_vector{ hist_peak_PE_all_PMTs_slow_S1, hist_peak_PE_all_PMTs_slow_S2, hist_peak_PE_all_SiPMs_slow_S1, hist_peak_PE_all_SiPMs_slow_S2};
	//	TCanvas *c20 = new TCanvas("c20", "PE_SiPM_PMTs_slow_h1");
	//	c20->Divide(2, 2, 0.01, 0.01);
	//	for (int i = 0; i < 4; i++)
	//	{
	//		c20->cd(i + 1);
	//		gPad->SetLogy();
	//		int ch = ch_index_for_view_list[i];
	//		hist_vector[ch]->Draw();
	//		hist_vector[ch]->GetXaxis()->SetTitle("Area [PE]");
	//		hist_vector[ch]->GetYaxis()->SetTitle("Counts");
	//	}
	//}

	if (draw_var.find("PE_SiPM_PMTs_slow_S2") != std::string::npos)
	{
		TCanvas *c25 = new TCanvas("c25", "PE_SiPM_PMTs_slow_S2");
		c25->Divide(2, 2, 0.01, 0.01);
		vector<TH1F*> h_vec = { hist_PE_allSiPMs_S2, hist_PE_allPMT_S2, hist_PE_allSiPMs_S2_areabased, hist_PE_allSiPMs_S2_test_pretrigger_areabased };
		vector<string> name = { "23SiPM [PE] (sig + bkg), peak based", "4PMT [PE] (sig + bkg), area based", "23SiPM [PE] (sig + bkg), area based", "23SiPM [PE] (sig + bkg) S2_test_pretrigger, area based" };

		for (int i = 0; i < 4; i++)
		{
			c25->cd(i + 1);
			//gPad->SetLogy();
			gStyle->SetOptFit(0111);
			h_vec[i]->SetTitle(name[i].c_str());
			h_vec[i]->GetXaxis()->SetTitle("Amplitude [PE]");
			h_vec[i]->GetYaxis()->SetTitle("Counts");
			//hist_peak_area_ev_vs_evnum_v[ch]->GetYaxis()->SetRangeUser(0, 1E6);
			h_vec[i]->Draw();
		}
	}
	

	if (draw_var.find("peak_area_ev_vs_evnum") != std::string::npos)
	{
		TCanvas *c14 = new TCanvas("c14", "peak_area_ev_vs_evnum");
		c14->Divide(2, 2, 0.01, 0.01);
		
		for (int i = 0; i < 4; i++)
		{
			c14->cd(i + 1);
			gPad->SetLogy();
			int ch = ch_index_for_view_list[i];
			hist_peak_area_ev_vs_evnum_v[ch]->GetXaxis()->SetTitle("event");
			hist_peak_area_ev_vs_evnum_v[ch]->GetYaxis()->SetTitle("peak_area_ev [mV*ns]");
			//hist_peak_area_ev_vs_evnum_v[ch]->GetYaxis()->SetRangeUser(0, 1E6);
			hist_peak_area_ev_vs_evnum_v[ch]->Draw("colz");
			prof_hist_peak_area_ev_vs_evnum_v[ch] = hist_peak_area_ev_vs_evnum_v[ch]->ProfileX();
			prof_hist_peak_area_ev_vs_evnum_v[ch]->Draw("same");
			prof_hist_peak_area_ev_vs_evnum_v[ch]->SetMarkerStyle(20);
			prof_hist_peak_area_ev_vs_evnum_v[ch]->SetMarkerColor(kRed);
		}
	}

	if (draw_var.find("peak_area_ev_vs_evnum_allSiPMs_allPMTs") != std::string::npos)
	{
		TCanvas *c15 = new TCanvas("c15", "peak_area_ev_vs_evnum_allSiPMs_allPMTs");
		c15->Divide(2, 2, 0.01, 0.01);

		vector<TH2F*> hist_vector{ hist_peak_area_ev_vs_evnum_all_SiPMs };
		vector<TProfile*> profile_vector(hist_vector.size());
		for (int i = 0; i < hist_vector.size(); i++)
		{
			c15->cd(i + 1);
			//gPad->SetLogy();
			hist_vector[i]->GetXaxis()->SetTitle("event");
			hist_vector[i]->GetYaxis()->SetTitle("peak_area_ev [mV*ns]");
			hist_vector[i]->Draw("colz");
			hist_vector[i]->GetYaxis()->SetRangeUser(0, 1E5);
			profile_vector[i] = hist_vector[i]->ProfileX();
			profile_vector[i]->Draw("same");
			profile_vector[i]->SetMarkerStyle(20);
			profile_vector[i]->SetMarkerColor(kRed);
		}
	}

	if (draw_var.find("n_peaks") != std::string::npos)
	{
		TCanvas *c8 = new TCanvas("c8", "n_peaks");
		c8->Divide(2, 2, 0.01, 0.01);
		for (int i = 0; i < 4; i++)
		{
			c8->cd(i+1);
			int ch_index = ch_index_for_view_list[i];
			hist_n_peaks_v[ch_index]->Draw();
			gPad->SetLogy();
			hist_n_peaks_v[ch_index]->SetFillColor(kGreen);
		}
	}

	if (draw_var.find("peak_amp_vs_peak_area") != std::string::npos)
	{
		TCanvas *c9 = new TCanvas("c9", "peak_amp_vs_peak_area");
		vector<TGraph*> gr_peak_amp_peak_area(ch_list.size());
		c9->Divide(2, 2, 0.01, 0.01);
		for (int i = 0; i < 4; i++)
		{
			c9->cd(i+1);
			int ch = ch_index_for_view_list[i];
			ostringstream gr_peak_amp_peak_area_name;
			gr_peak_amp_peak_area_name << "graph_peak_amp_peak_area_ch_" << ch_list[ch] << " (" << rd_inf.GetChNameList()[ch] << ")";
			gr_peak_amp_peak_area[ch] = new TGraph(peak_amp_v[ch].size(), &peak_amp_v[ch][0], &peak_area_v[ch][0]);
			gr_peak_amp_peak_area[ch]->SetTitle(gr_peak_amp_peak_area_name.str().c_str());
			gr_peak_amp_peak_area[ch]->Draw("AP");
			//gr_peak_amp_peak_area[ch]->Draw("colz");
			gr_peak_amp_peak_area[ch]->GetXaxis()->SetTitle("Peak Amp");
			gr_peak_amp_peak_area[ch]->GetYaxis()->SetTitle("Peak Area");
			gr_peak_amp_peak_area[ch]->GetXaxis()->SetRangeUser(rd_inf.GetHistPeakAmpXminList()[ch], rd_inf.GetHistPeakAmpXmaxList()[ch]);
			gr_peak_amp_peak_area[ch]->GetYaxis()->SetRangeUser(rd_inf.GetHistPeakAreaXminList()[ch], rd_inf.GetHistPeakAreaXmaxList()[ch]);
		}		
	}

	if (draw_var.find("peak_amp_vs_peak_area_hist2") != std::string::npos)
	{
		TCanvas *c16 = new TCanvas("c16", "peak_amp_vs_peak_area_hist2");
		
		c16->Divide(2, 2, 0.01, 0.01);
		for (int i = 0; i < 4; i++)
		{
			c16->cd(i + 1);
			int ch = ch_index_for_view_list[i];
			hist_peak_amp_peak_area_v[ch]->Draw("colz");
			hist_peak_amp_peak_area_v[ch]->GetXaxis()->SetTitle("Peak Amp");
			hist_peak_amp_peak_area_v[ch]->GetYaxis()->SetTitle("Peak Area");
			hist_peak_amp_peak_area_v[ch]->GetXaxis()->SetRangeUser(rd_inf.GetHistPeakAmpXminList()[ch], rd_inf.GetHistPeakAmpXmaxList()[ch]);
			hist_peak_amp_peak_area_v[ch]->GetYaxis()->SetRangeUser(rd_inf.GetHistPeakAreaXminList()[ch], rd_inf.GetHistPeakAreaXmaxList()[ch]);
		}
	}

	if (draw_var.find("peak_time_all_SiPMs_PMTs_slow_PMTs_fast") != std::string::npos)
	{
		TCanvas *c10 = new TCanvas("c10", "peak_time_spectra");
		vector<TH1F*> hist_vector{ hist_peak_time_all_SiPMs, hist_peak_time_all_PMTs_slow, hist_peak_time_all_PMTs_fast, hist_peak_time_PMTtrigg_slow};
		//vector<TH1F*> hist_vector{ hist_peak_time_all_SiPMs, hist_peak_time_3PMTs_slow, hist_peak_time_1PMT_slow };
		c10->Divide(2, 2, 0.01, 0.01);
		for (int i = 0; i < 4; i++)
		{
			c10->cd(i + 1);
			gPad->SetLogy();
			hist_vector[i]->Draw();
			hist_vector[i]->GetXaxis()->SetTitle("Time [us]");
			hist_vector[i]->GetYaxis()->SetTitle("Counts");
			hist_vector[i]->GetXaxis()->SetRangeUser(0, 160E3);
		}
	}

	if (draw_var.find("n_peaks_chi_vs_chj") != std::string::npos)
	{

	}

	const Int_t NRGBs = 6;
	const Int_t NCont = 999;

	Double_t stops[NRGBs] = { 0.00, 0.2, 0.4, 0.6, 0.8, 1.00 };
	Double_t red[NRGBs] = { 0.99, 0.44, 0.00, 0.87, 1.00, 0.51 };
	Double_t green[NRGBs] = { 0.00, 0.66, 0.81, 1.00, 0.20, 0.00 };
	Double_t blue[NRGBs] = { 0.99, 0.72, 1.00, 0.12, 0.00, 0.00 };

	TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
	gStyle->SetNumberContours(NCont);
	//gStyle->SetOptStat(0);

	if (draw_var.find("n_peaks_map_S1") != std::string::npos)
	{
		TCanvas *c11 = new TCanvas("c11", "n_peaks_map_S1");
		const Double_t min = 0;
		const Double_t max = *max_element(n_peaks_S1_min_bkg_avr.begin()+11, n_peaks_S1_min_bkg_avr.end());
		const Int_t nLevels = 999;
		Double_t levels[nLevels];

		for (int i = 1; i < nLevels; i++)
		{
			levels[i] = min + (max - min) / (nLevels - 1) * (i);
		}
		levels[0] = 0.01;

		h2_n_peaks_S1->SetContour((sizeof(levels) / sizeof(Double_t)), levels);

		for (int i = 0; i < 23; i++)
		{
			int ch_index = ch_index_for_good_SiPMs_xy_order[i];			
			int xi = xybins_good_SiPMs[i].first + 3;
			int yi = xybins_good_SiPMs[i].second + 3;
			
			double z = n_peaks_S1_min_bkg_avr[ch_index];
			h2_n_peaks_S1->SetBinContent(xi, yi, z);
		}

		ostringstream h2_n_peaks_S1_name;
		h2_n_peaks_S1_name << "S1 uniformity (n_peaks based)";
		h2_n_peaks_S1->SetTitle(h2_n_peaks_S1_name.str().c_str());

		h2_n_peaks_S1->GetXaxis()->SetTitle("x [cm]");
		h2_n_peaks_S1->GetYaxis()->SetTitle("y [cm]");
		gStyle->SetPaintTextFormat("2.5f");
		h2_n_peaks_S1->SetMarkerSize(2);

		h2_n_peaks_S1->Draw("col text");
		gPad->Update();
		h2_n_peaks_S1->SetStats(0);

		h2_n_peaks_S1->GetZaxis()->SetRangeUser(min, max); // ... set the range ...

		h2_n_peaks_S1->Draw("z same"); // draw the "color palette"
		gPad->Update();
	}

	if (draw_var.find("n_peaks_map_S2") != std::string::npos)
	{
		TCanvas *c23 = new TCanvas("c23", "n_peaks_map_S2");
		const Double_t min = 0;
		const Double_t max = *max_element(n_peaks_S2_min_bkg_avr.begin() + 11, n_peaks_S2_min_bkg_avr.end());
		const Int_t nLevels = 999;
		Double_t levels[nLevels];

		for (int i = 1; i < nLevels; i++)
		{
			levels[i] = min + (max - min) / (nLevels - 1) * (i);
		}
		levels[0] = 0.01;

		h2_n_peaks_S2->SetContour((sizeof(levels) / sizeof(Double_t)), levels);

		for (int i = 0; i < 23; i++)
		{
			int ch_index = ch_index_for_good_SiPMs_xy_order[i];
			int xi = xybins_good_SiPMs[i].first + 3;
			int yi = xybins_good_SiPMs[i].second + 3;

			double z = n_peaks_S2_min_bkg_avr[ch_index];
			h2_n_peaks_S2->SetBinContent(xi, yi, z);

		}

		
		

		ostringstream h2_n_peaks_S2_name;
		h2_n_peaks_S2_name << "S2 uniformity (n_peaks based)";
		h2_n_peaks_S2->SetTitle(h2_n_peaks_S2_name.str().c_str());

		h2_n_peaks_S2->GetXaxis()->SetTitle("x [cm]");
		h2_n_peaks_S2->GetYaxis()->SetTitle("y [cm]");
		gStyle->SetPaintTextFormat("2.5f");
		h2_n_peaks_S2->SetMarkerSize(2);

		h2_n_peaks_S2->Draw("col text");
		gPad->Update();
		h2_n_peaks_S2->SetStats(0);

		h2_n_peaks_S2->GetZaxis()->SetRangeUser(min, max); // ... set the range ...

		h2_n_peaks_S2->Draw("z same"); // draw the "color palette"
		gPad->Update();

		
	}

	if (draw_var.find("n_peaks_map_bkg") != std::string::npos)
	{
		TCanvas *c24 = new TCanvas("c24", "n_peaks_map_bkg");
		const Double_t min = 0;
		const Double_t max = *max_element(n_peaks_bkg_avr.begin() + 11, n_peaks_bkg_avr.end());
		const Int_t nLevels = 999;
		Double_t levels[nLevels];

		for (int i = 1; i < nLevels; i++)
		{
			levels[i] = min + (max - min) / (nLevels - 1) * (i);
		}
		levels[0] = 0.01;

		h2_n_peaks_bkg->SetContour((sizeof(levels) / sizeof(Double_t)), levels);

		for (int i = 0; i < 23; i++)
		{
			int ch_index = ch_index_for_good_SiPMs_xy_order[i];
			int xi = xybins_good_SiPMs[i].first + 3;
			int yi = xybins_good_SiPMs[i].second + 3;

			double z = n_peaks_bkg_avr[ch_index];
			h2_n_peaks_bkg->SetBinContent(xi, yi, z);
		}

		ostringstream h2_n_peaks_bkg_name;
		h2_n_peaks_bkg_name << "Bkg uniformity (n_peaks based)";
		h2_n_peaks_bkg->SetTitle(h2_n_peaks_bkg_name.str().c_str());

		h2_n_peaks_bkg->GetXaxis()->SetTitle("x [cm]");
		h2_n_peaks_bkg->GetYaxis()->SetTitle("y [cm]");
		gStyle->SetPaintTextFormat("2.5f");
		h2_n_peaks_bkg->SetMarkerSize(2);

		h2_n_peaks_bkg->Draw("col text");
		gPad->Update();
		h2_n_peaks_bkg->SetStats(0);

		h2_n_peaks_bkg->GetZaxis()->SetRangeUser(min, max); // ... set the range ...

		h2_n_peaks_bkg->Draw("z same"); // draw the "color palette"
		gPad->Update();
	}


	if (draw_var.find("peaks_area_map_bkg") != std::string::npos)
	{
		TCanvas *c26 = new TCanvas("c26", "peaks_area_map_bkg");
		const Double_t min = 0;
		const Double_t max = *max_element(peak_area_bkg_avr.begin() + 11, peak_area_bkg_avr.end()) * S1_dt;
		const Int_t nLevels = 999;
		Double_t levels[nLevels];

		for (int i = 1; i < nLevels; i++)
		{
			levels[i] = min + (max - min) / (nLevels - 1) * (i);
		}
		levels[0] = 0.01;

		h2_peaks_area_bkg->SetContour((sizeof(levels) / sizeof(Double_t)), levels);

		for (int i = 0; i < 23; i++)
		{
			int ch_index = ch_index_for_good_SiPMs_xy_order[i];
			int xi = xybins_good_SiPMs[i].first + 3;
			int yi = xybins_good_SiPMs[i].second + 3;

			double z = peak_area_bkg_avr[ch_index] * S1_dt;
			h2_peaks_area_bkg->SetBinContent(xi, yi, z);
		}

		ostringstream h2_n_peaks_bkg_name;
		h2_n_peaks_bkg_name << "Bkg uniformity (peak area based)";
		h2_peaks_area_bkg->SetTitle(h2_n_peaks_bkg_name.str().c_str());

		h2_peaks_area_bkg->GetXaxis()->SetTitle("x [cm]");
		h2_peaks_area_bkg->GetYaxis()->SetTitle("y [cm]");
		gStyle->SetPaintTextFormat("2.5f");
		h2_peaks_area_bkg->SetMarkerSize(2);

		h2_peaks_area_bkg->Draw("col text");
		gPad->Update();
		h2_peaks_area_bkg->SetStats(0);

		h2_peaks_area_bkg->GetZaxis()->SetRangeUser(min, max); // ... set the range ...

		h2_peaks_area_bkg->Draw("z same"); // draw the "color palette"
		gPad->Update();
	}

	if (draw_var.find("peaks_area_map_S1") != std::string::npos)
	{
		TCanvas *c28 = new TCanvas("c28", "peaks_area_map_S1");
		const Double_t min = 0;
		const Double_t max = *max_element(peak_area_S1_min_bkg_avr.begin() + 11, peak_area_S1_min_bkg_avr.end());
		const Int_t nLevels = 999;
		Double_t levels[nLevels];

		for (int i = 1; i < nLevels; i++)
		{
			levels[i] = min + (max - min) / (nLevels - 1) * (i);
		}
		levels[0] = 0.01;

		h2_peaks_area_S1->SetContour((sizeof(levels) / sizeof(Double_t)), levels);

		for (int i = 0; i < 23; i++)
		{
			int ch_index = ch_index_for_good_SiPMs_xy_order[i];
			int xi = xybins_good_SiPMs[i].first + 3;
			int yi = xybins_good_SiPMs[i].second + 3;

			double z = peak_area_S1_min_bkg_avr[ch_index];
			h2_peaks_area_S1->SetBinContent(xi, yi, z);
		}

		ostringstream h2_n_peaks_S1_name;
		h2_n_peaks_S1_name << "S1 uniformity (peak area based)";
		h2_peaks_area_S1->SetTitle(h2_n_peaks_S1_name.str().c_str());

		h2_peaks_area_S1->GetXaxis()->SetTitle("x [cm]");
		h2_peaks_area_S1->GetYaxis()->SetTitle("y [cm]");
		gStyle->SetPaintTextFormat("2.5f");
		h2_peaks_area_S1->SetMarkerSize(2);

		h2_peaks_area_S1->Draw("col text");
		gPad->Update();
		h2_peaks_area_S1->SetStats(0);

		h2_peaks_area_S1->GetZaxis()->SetRangeUser(min, max); // ... set the range ...

		h2_peaks_area_S1->Draw("z same"); // draw the "color palette"
		gPad->Update();
	}

	if (draw_var.find("peaks_area_map_S2") != std::string::npos)
	{
		TCanvas *c29 = new TCanvas("c29", "peaks_area_map_S2");
		const Double_t min = 0;
		const Double_t max = *max_element(peak_area_S2_min_bkg_avr.begin() + 11, peak_area_S2_min_bkg_avr.end());
		const Int_t nLevels = 999;
		Double_t levels[nLevels];

		for (int i = 1; i < nLevels; i++)
		{
			levels[i] = min + (max - min) / (nLevels - 1) * (i);
		}
		levels[0] = 0.01;

		h2_peaks_area_S2->SetContour((sizeof(levels) / sizeof(Double_t)), levels);

		for (int i = 0; i < 23; i++)
		{
			int ch_index = ch_index_for_good_SiPMs_xy_order[i];
			int xi = xybins_good_SiPMs[i].first + 3;
			int yi = xybins_good_SiPMs[i].second + 3;

			double z = peak_area_S2_min_bkg_avr[ch_index];
			h2_peaks_area_S2->SetBinContent(xi, yi, z);
		}

		for (int i = 0; i < 20; i++)
		{			
			//cout << "i = " << i << endl;
			
			if (i == 5 || i == 10 || i == 15 || i == 20) f_out_S2_SiPM_PE_map << endl;						
			f_out_S2_SiPM_PE_map << peak_area_S2_min_bkg_avr[ch_index_for_good_SiPMs_xy_order[i]] << "\t" ;	
		}
		f_out_S2_SiPM_PE_map << endl;
		f_out_S2_SiPM_PE_map << peak_area_S2_min_bkg_avr[ch_index_for_good_SiPMs_xy_order[20]] << "\t";
		f_out_S2_SiPM_PE_map << peak_area_S2_min_bkg_avr[ch_index_for_good_SiPMs_xy_order[1]] << "\t";
		f_out_S2_SiPM_PE_map << peak_area_S2_min_bkg_avr[ch_index_for_good_SiPMs_xy_order[21]] << "\t";
		f_out_S2_SiPM_PE_map << peak_area_S2_min_bkg_avr[ch_index_for_good_SiPMs_xy_order[22]] << "\t";
		f_out_S2_SiPM_PE_map << peak_area_S2_min_bkg_avr[ch_index_for_good_SiPMs_xy_order[4]] << "\t";
		f_out_S2_SiPM_PE_map << endl;

		ostringstream h2_n_peaks_S1_name;
		h2_n_peaks_S1_name << "S2 uniformity (peak area based)";
		h2_peaks_area_S2->SetTitle(h2_n_peaks_S1_name.str().c_str());

		h2_peaks_area_S2->GetXaxis()->SetTitle("x [cm]");
		h2_peaks_area_S2->GetYaxis()->SetTitle("y [cm]");
		gStyle->SetPaintTextFormat("2.5f");
		h2_peaks_area_S2->SetMarkerSize(2);

		h2_peaks_area_S2->Draw("col text");
		gPad->Update();
		h2_peaks_area_S2->SetStats(0);

		h2_peaks_area_S2->GetZaxis()->SetRangeUser(min, max); // ... set the range ...

		h2_peaks_area_S2->Draw("z same"); // draw the "color palette"
		gPad->Update();
	}



	cout << endl;
	cout << "all is ok" << endl;
	theApp.Run();
	system("pause");
	return 0;
}