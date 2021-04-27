#include "DefineHists.h"


DefineHists::DefineHists(int NEntries, std::vector<int> ch_list, ReadInfo &rd_inf, bool is_calib)
{
	const int hist_baseline_sigma_xmax = 50;
	double SiPM_axis_area_factor_for_calib = 1;
	if (is_calib)
	{
		SiPM_axis_area_factor_for_calib = 1 /*0.3*/;
	}
	
	
	hist_peak_time_all_SiPMs = new TH1F("hist_peak_time_all_SiPMs", "hist_peak_time_all_SiPMs", 1330, 0, 160E3);
	hist_peak_time_all_PMTs_slow = new TH1F("hist_peak_time_all_PMTs_slow", "hist_peak_time_all_PMTs_slow", 1330, 0, 160E3);
	hist_peak_time_all_PMTs_fast = new TH1F("hist_peak_time_all_PMTs_fast", "hist_peak_time_all_PMTs_fast", 1330, 0, 160E3);
	hist_peak_time_3PMTs_slow = new TH1F("hist_peak_time_3PMTs_slow", "hist_peak_time_3PMTs_slow", 160, 0, 160E3);
	hist_peak_time_1PMT_slow = new TH1F("hist_peak_time_1PMT_slow", "hist_peak_time_1PMT_slow", 160, 0, 160E3);
	hist_peak_time_PMTtrigg_slow = new TH1F("hist_peak_time_PMTtrigg_slow", "hist_peak_time_PMTtrigg_slow", 1330, 0, 160E3);
	hist_peak_area_ev_vs_evnum_all_SiPMs = new TH2F("hist_peak_area_ev_vs_evnum_all_SiPMs", "hist_peak_area_ev_vs_evnum_all_SiPMs", 110, 0, NEntries/*tree->GetEntries()*/,
		200, 0, 600);

	x_max_hist_PE = 180;
	hist_PE_allPMT_S2 = new TH1F("hist_PE_allPMT_S2", "hist_PE_allPMT_S2", x_max_hist_PE * 2, 0, x_max_hist_PE);
	hist_PE_allSiPMs_S2 = new TH1F("hist_PE_allSiPMs_S2", "hist_PE_allSiPMs_S2", x_max_hist_PE * 2, 0, x_max_hist_PE);
	hist_PE_allSiPMs_S2_areabased = new TH1F("hist_PE_allSiPMs_S2_areabased", "hist_PE_allSiPMs_S2_areabased", x_max_hist_PE * 2, 0, x_max_hist_PE);
	hist_PE_allSiPMs_S2_test_pretrigger_areabased = new TH1F("hist_PE_allSiPMs_S2_test_pretrigger_areabased", "hist_PE_allSiPMs_S2_test_pretrigger_areabased", x_max_hist_PE * 2, 0, x_max_hist_PE);

	TH2F* h2_n_peaks_S1 = new TH2F("h2_n_peaks_S1", "h2_n_peaks_S1", 5, -23, 23, 5, -23, 23);
	TH2F* h2_n_peaks_S2 = new TH2F("h2_n_peaks_S2", "h2_n_peaks_S2", 5, -23, 23, 5, -23, 23);
	TH2F* h2_n_peaks_bkg = new TH2F("h2_n_peaks_bkg", "h2_n_peaks_bkg", 5, -23, 23, 5, -23, 23);

	TH2F* h2_peaks_area_S1 = new TH2F("h2_peaks_area_S1", "h2_peaks_area_S1", 5, -23, 23, 5, -23, 23);
	TH2F* h2_peaks_area_S2 = new TH2F("h2_peaks_area_S2", "h2_peaks_area_S2", 5, -23, 23, 5, -23, 23);
	TH2F* h2_peaks_area_bkg = new TH2F("h2_peaks_area_bkg", "h2_peaks_area_bkg", 5, -23, 23, 5, -23, 23);

	hist_ymin_v.resize(ch_list.size(), NULL);
	hist_ymax_v.resize(ch_list.size(), NULL);
	hist_baseline_mean_v.resize(ch_list.size(), NULL);
	hist_baseline_sigma_v.resize(ch_list.size(), NULL);
	hist_n_peaks_v.resize(ch_list.size(), NULL);
	hist_peak_time_v.resize(ch_list.size(), NULL);
	hist_peak_amp_v.resize(ch_list.size(), NULL);
	hist_peak_area_v.resize(ch_list.size(), NULL);
	hist_peak_length_v.resize(ch_list.size(), NULL);
	hist_peak_area_ev_v.resize(ch_list.size(), NULL);
	hist_peak_area_bkg_v.resize(ch_list.size(), NULL);
	hist_peak_area_S1_v.resize(ch_list.size(), NULL);
	hist_peak_area_S2_v.resize(ch_list.size(), NULL);
	hist_n_peaks_bkg_v.resize(ch_list.size(), NULL);
	hist_n_peaks_S1_v.resize(ch_list.size(), NULL);
	hist_n_peaks_S2_v.resize(ch_list.size(), NULL);

	hist_peak_amp_peak_area_v.resize(ch_list.size(), NULL);
	hist_peak_area_ev_vs_evnum_v.resize(ch_list.size(), NULL);

	for (int ch = 0; ch < ch_list.size(); ch++)
	{
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

		

		hist_ymin_v[ch] = new TH1F(hist_ymin_name.str().c_str(), hist_ymin_name.str().c_str(), 1000, -1100, 1100);
		hist_ymax_v[ch] = new TH1F(hist_ymax_name.str().c_str(), hist_ymax_name.str().c_str(), 1000, -1100, 1100);
		hist_baseline_mean_v[ch] = new TH1F(hist_baseline_mean_name.str().c_str(), hist_baseline_mean_name.str().c_str(), 1000, -1100, 1100);
		hist_baseline_sigma_v[ch] = new TH1F(hist_baseline_sigma_name.str().c_str(), hist_baseline_sigma_name.str().c_str(), 1000, 0, hist_baseline_sigma_xmax);
		hist_n_peaks_v[ch] = new TH1F(hist_n_peaks_name.str().c_str(), hist_n_peaks_name.str().c_str(), rd_inf.GetHistNpeaksNbinsList()[ch], rd_inf.GetHistNpeaksXminList()[ch], rd_inf.GetHistNpeaksXmaxList()[ch]);
		hist_peak_time_v[ch] = new TH1F(hist_peak_time_name.str().c_str(), hist_peak_time_name.str().c_str(), rd_inf.GetHistPeakTimeNbinsList()[ch], rd_inf.GetHistPeakTimeXminList()[ch], rd_inf.GetHistPeakTimeXmaxList()[ch]);
		hist_peak_amp_v[ch] = new TH1F(hist_peak_amp_name.str().c_str(), hist_peak_amp_name.str().c_str(), rd_inf.GetHistPeakAmpNbinsList()[ch], rd_inf.GetHistPeakAmpXminList()[ch], rd_inf.GetHistPeakAmpXmaxList()[ch]);
		hist_peak_area_v[ch] = new TH1F(hist_peak_area_name.str().c_str(), hist_peak_area_name.str().c_str(), rd_inf.GetHistPeakAreaNbinsList()[ch], rd_inf.GetHistPeakAreaXminList()[ch], rd_inf.GetHistPeakAreaXmaxList()[ch] * SiPM_axis_area_factor_for_calib);
		hist_peak_amp_peak_area_v[ch] = new TH2F(hist_peak_amp_peak_area_name.str().c_str(), hist_peak_amp_peak_area_name.str().c_str(),
			rd_inf.GetHistPeakAmpNbinsList()[ch], rd_inf.GetHistPeakAmpXminList()[ch], rd_inf.GetHistPeakAmpXmaxList()[ch],
			rd_inf.GetHistPeakAreaNbinsList()[ch], rd_inf.GetHistPeakAreaXminList()[ch], rd_inf.GetHistPeakAreaXmaxList()[ch]);



		if (ch != 9)
			hist_peak_length_v[ch] = new TH1F(hist_peak_length_name.str().c_str(), hist_peak_length_name.str().c_str(), 300, 0, 2000);
		else
			hist_peak_length_v[ch] = new TH1F(hist_peak_length_name.str().c_str(), hist_peak_length_name.str().c_str(), 300, 0, 20000);

		hist_peak_area_ev_v[ch] = new TH1F(hist_peak_area_ev_name.str().c_str(), hist_peak_area_ev_name.str().c_str(),
			rd_inf.GetHistPeakAreaEvNbinsList()[ch], rd_inf.GetHistPeakAreaEvXminList()[ch], rd_inf.GetHistPeakAreaEvXmaxList()[ch]);
		hist_peak_area_ev_vs_evnum_v[ch] = new TH2F(hist_peak_area_ev_vs_evnum_name.str().c_str(), hist_peak_area_ev_vs_evnum_name.str().c_str(),
			110, 0, NEntries, rd_inf.GetHistPeakAreaEvNbinsList()[ch], rd_inf.GetHistPeakAreaEvXminList()[ch], rd_inf.GetHistPeakAreaEvXmaxList()[ch]);

		hist_peak_area_bkg_v[ch] = new TH1F(hist_peak_area_bkg_name.str().c_str(), hist_peak_area_bkg_name.str().c_str(), rd_inf.GetHistPeakAreaEvNbinsList()[ch], 0, 100);
		hist_peak_area_S1_v[ch] = new TH1F(hist_peak_area_S1_name.str().c_str(), hist_peak_area_S1_name.str().c_str(), rd_inf.GetHistPeakAreaEvNbinsList()[ch], 0, 50);//test
		hist_peak_area_S2_v[ch] = new TH1F(hist_peak_area_S2_name.str().c_str(), hist_peak_area_S2_name.str().c_str(), rd_inf.GetHistPeakAreaEvNbinsList()[ch], 0, 100);

		hist_n_peaks_bkg_v[ch] = new TH1F(hist_n_peaks_bkg_name.str().c_str(), hist_n_peaks_bkg_name.str().c_str(), rd_inf.GetHistPeakAreaEvNbinsList()[ch], 0, 5);
		hist_n_peaks_S1_v[ch] = new TH1F(hist_n_peaks_S1_name.str().c_str(), hist_n_peaks_S1_name.str().c_str(), rd_inf.GetHistPeakAreaEvNbinsList()[ch], 0, 5);
		hist_n_peaks_S2_v[ch] = new TH1F(hist_n_peaks_S2_name.str().c_str(), hist_n_peaks_S2_name.str().c_str(), rd_inf.GetHistPeakAreaEvNbinsList()[ch], 0, 50);

	}

}

TH1F* &DefineHists::Get_hist_peak_time_all_SiPMs(){	return hist_peak_time_all_SiPMs;}
TH1F* &DefineHists::Get_hist_peak_time_all_PMTs_slow() { return hist_peak_time_all_PMTs_slow; }
TH1F* &DefineHists::Get_hist_peak_time_all_PMTs_fast() { return hist_peak_time_all_PMTs_fast; }
TH1F* &DefineHists::Get_hist_peak_time_3PMTs_slow(){ return hist_peak_time_3PMTs_slow; }
TH1F* &DefineHists::Get_hist_peak_time_1PMT_slow(){ return hist_peak_time_1PMT_slow; }
TH1F* &DefineHists::Get_hist_peak_time_PMTtrigg_slow(){ return hist_peak_time_PMTtrigg_slow; }
TH2F* &DefineHists::Get_hist_peak_area_ev_vs_evnum_all_SiPMs(){ return hist_peak_area_ev_vs_evnum_all_SiPMs; }

TH1F* &DefineHists::Get_hist_PE_allPMT_S2(){ return hist_PE_allPMT_S2; }
TH1F* &DefineHists::Get_hist_PE_allSiPMs_S2(){ return hist_PE_allSiPMs_S2; }
TH1F* &DefineHists::Get_hist_PE_allSiPMs_S2_areabased(){ return hist_PE_allSiPMs_S2_areabased; }
TH1F* &DefineHists::Get_hist_PE_allSiPMs_S2_test_pretrigger_areabased(){ return hist_PE_allSiPMs_S2_test_pretrigger_areabased; }

//TH2F* &DefineHists::Get_h2_n_peaks_S1(){ return h2_n_peaks_S1; }
//TH2F* &DefineHists::Get_h2_n_peaks_S2(){ return h2_n_peaks_S2; }
//TH2F* &DefineHists::Get_h2_n_peaks_bkg(){ return h2_n_peaks_bkg; }
//TH2F* &DefineHists::Get_h2_peaks_area_S1(){ return h2_peaks_area_S1; }
//TH2F* &DefineHists::Get_h2_peaks_area_S2(){ return h2_peaks_area_S2; }
//TH2F* &DefineHists::Get_h2_peaks_area_bkg(){ return h2_peaks_area_bkg; }

std::vector<TH1F*> &DefineHists::Get_hist_ymin_v(){ return hist_ymin_v; }
std::vector<TH1F*> &DefineHists::Get_hist_ymax_v(){ return hist_ymax_v; }
std::vector<TH1F*> &DefineHists::Get_hist_baseline_mean_v(){ return hist_baseline_mean_v; }
std::vector<TH1F*> &DefineHists::Get_hist_baseline_sigma_v(){ return hist_baseline_sigma_v; }
std::vector<TH1F*> &DefineHists::Get_hist_n_peaks_v(){ return hist_n_peaks_v; }
std::vector<TH1F*> &DefineHists::Get_hist_peak_time_v(){ return hist_peak_time_v; }
std::vector<TH1F*> &DefineHists::Get_hist_peak_amp_v(){ return hist_peak_amp_v; }
std::vector<TH1F*> &DefineHists::Get_hist_peak_area_v(){ return hist_peak_area_v; }
std::vector<TH1F*> &DefineHists::Get_hist_peak_length_v(){ return hist_peak_length_v; }
std::vector<TH1F*> &DefineHists::Get_hist_peak_area_ev_v(){ return hist_peak_area_ev_v; }

std::vector<TH1F*> &DefineHists::Get_hist_peak_area_bkg_v(){ return hist_peak_area_bkg_v; }
std::vector<TH1F*> &DefineHists::Get_hist_peak_area_S1_v(){ return hist_peak_area_S1_v; }
std::vector<TH1F*> &DefineHists::Get_hist_peak_area_S2_v(){ return hist_peak_area_S2_v; }
std::vector<TH1F*> &DefineHists::Get_hist_n_peaks_bkg_v(){ return hist_n_peaks_bkg_v; }
std::vector<TH1F*> &DefineHists::Get_hist_n_peaks_S1_v(){ return hist_n_peaks_S1_v; }
std::vector<TH1F*> &DefineHists::Get_hist_n_peaks_S2_v(){ return hist_n_peaks_S2_v; }

std::vector<TH2F*> &DefineHists::Get_hist_peak_amp_peak_area_v(){ return hist_peak_amp_peak_area_v; }
std::vector<TH2F*> &DefineHists::Get_hist_peak_area_ev_vs_evnum_v(){ return hist_peak_area_ev_vs_evnum_v; }

DefineHists::~DefineHists()
{
}
