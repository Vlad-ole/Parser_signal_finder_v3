#pragma once

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
#include "ReadInfo.h"

class DefineHists
{
public:
	DefineHists(int NEntries, std::vector<int> ch_list, ReadInfo &rd_inf, bool is_calib);
	~DefineHists();

	TH1F* &Get_hist_peak_time_all_SiPMs();
	TH1F* &Get_hist_peak_time_all_PMTs_slow();
	TH1F* &Get_hist_peak_time_all_PMTs_fast();
	TH1F* &Get_hist_peak_time_3PMTs_slow();
	TH1F* &Get_hist_peak_time_1PMT_slow();
	TH1F* &Get_hist_peak_time_PMTtrigg_slow();
	TH2F* &Get_hist_peak_area_ev_vs_evnum_all_SiPMs();
	TH1F* &Get_hist_allSiPM_time_spectrum_PE_corrected();

	TH1F* &Get_hist_PE_allPMT_S2();
	TH1F* &Get_hist_PE_allSiPMs_S2();
	TH1F* &Get_hist_PE_allSiPMs_S2_areabased();
	TH1F* &Get_hist_PE_allSiPMs_S2_test_pretrigger_areabased();

	/*TH2F* &Get_h2_n_peaks_S1();
	TH2F* &Get_h2_n_peaks_S2();
	TH2F* &Get_h2_n_peaks_bkg();
	TH2F* &Get_h2_peaks_area_S1();
	TH2F* &Get_h2_peaks_area_S2();*/
	/*TH2F* &Get_h2_peaks_area_bkg();*/

	std::vector<TH1F*> &Get_hist_ymin_v();
	std::vector<TH1F*> &Get_hist_ymax_v();
	std::vector<TH1F*> &Get_hist_baseline_mean_v();
	std::vector<TH1F*> &Get_hist_baseline_sigma_v();
	std::vector<TH1F*> &Get_hist_n_peaks_v();
	std::vector<TH1F*> &Get_hist_peak_time_v();
	std::vector<TH1F*> &Get_hist_peak_amp_v();
	std::vector<TH1F*> &Get_hist_peak_area_v();
	std::vector<TH1F*> &Get_hist_peak_length_v();
	std::vector<TH1F*> &Get_hist_peak_area_ev_v();

	std::vector<TH1F*> &Get_hist_peak_area_bkg_v();
	std::vector<TH1F*> &Get_hist_peak_area_S1_v();
	std::vector<TH1F*> &Get_hist_peak_area_S2_v();
	std::vector<TH1F*> &Get_hist_n_peaks_bkg_v();
	std::vector<TH1F*> &Get_hist_n_peaks_S1_v();
	std::vector<TH1F*> &Get_hist_n_peaks_S2_v();

	std::vector<TH2F*> &Get_hist_peak_amp_peak_area_v();
	std::vector<TH2F*> &Get_hist_peak_area_ev_vs_evnum_v();
	

private:
	TH1F* hist_peak_time_all_SiPMs;
	TH1F* hist_peak_time_all_PMTs_slow;
	TH1F* hist_peak_time_all_PMTs_fast;
	TH1F* hist_peak_time_3PMTs_slow; 
	TH1F* hist_peak_time_1PMT_slow;
	TH1F* hist_peak_time_PMTtrigg_slow; 
	TH2F* hist_peak_area_ev_vs_evnum_all_SiPMs;
	TH1F* hist_allSiPM_time_spectrum_PE_corrected;

	int x_max_hist_PE;
	TH1F* hist_PE_allPMT_S2; 
	TH1F* hist_PE_allSiPMs_S2;
	TH1F* hist_PE_allSiPMs_S2_areabased;
	TH1F* hist_PE_allSiPMs_S2_test_pretrigger_areabased;	

	/*TH2F* h2_n_peaks_S1;
	TH2F* h2_n_peaks_S2;
	TH2F* h2_n_peaks_bkg;
	TH2F* h2_peaks_area_S1;
	TH2F* h2_peaks_area_S2;
	TH2F* h2_peaks_area_bkg;*/

	std::vector<TH1F*> hist_ymin_v;
	std::vector<TH1F*> hist_ymax_v;
	std::vector<TH1F*> hist_baseline_mean_v;
	std::vector<TH1F*> hist_baseline_sigma_v;
	std::vector<TH1F*> hist_n_peaks_v;
	std::vector<TH1F*> hist_peak_time_v;
	std::vector<TH1F*> hist_peak_amp_v;
	std::vector<TH1F*> hist_peak_area_v;
	std::vector<TH1F*> hist_peak_length_v;
	std::vector<TH1F*> hist_peak_area_ev_v;

	std::vector<TH1F*> hist_peak_area_bkg_v;
	std::vector<TH1F*> hist_peak_area_S1_v; 
	std::vector<TH1F*> hist_peak_area_S2_v;
	std::vector<TH1F*> hist_n_peaks_bkg_v;
	std::vector<TH1F*> hist_n_peaks_S1_v;
	std::vector<TH1F*> hist_n_peaks_S2_v;

	std::vector<TH2F*> hist_peak_amp_peak_area_v;
	std::vector<TH2F*> hist_peak_area_ev_vs_evnum_v;

	

	
};

