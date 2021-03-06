#pragma once

//c++
#include <iostream>
#include <sstream>
#include <vector>
#include <iomanip>
#include <fstream>
#include <string>

class ReadInfo
{
public:
	ReadInfo(std::string file_name_info);
	~ReadInfo();

	void Read();
	void Show();

	std::vector<std::string> &GetChNameList();
	std::vector<int> &GetChList();
	std::vector<bool> &GetIsPositivePolarityTypeList();
	std::vector<double> &GetThRawList();
	std::vector<double> &GetThFilteredList();
	std::vector<unsigned int> &GetPeakFinderVersionList();
	std::vector<bool> &GetIsLocalBaselineList();

	std::vector<double> &GetWindowList();
	std::vector<double> &GetLocalBaselineWindowList();
	std::vector<double> &GetLocalBaselineWindowShiftList();
	std::vector<double> &GetCheckOverlappingWindowList();
	std::vector<double> &GetShrinkingOfLeftTailList();
	std::vector<double> &GetShrinkingOfRightTailList();
	std::vector<double> &GetFilteringWindowList();

	std::vector<int> &GetHistPeakAmpNbinsList();
	std::vector<double> &GetHistPeakAmpXminList();
	std::vector<double> &GetHistPeakAmpXmaxList();

	std::vector<int> &GetHistPeakAreaNbinsList();
	std::vector<double> &GetHistPeakAreaXminList();
	std::vector<double> &GetHistPeakAreaXmaxList();

	std::vector<int> &GetHistNpeaksNbinsList();
	std::vector<double> &GetHistNpeaksXminList();
	std::vector<double> &GetHistNpeaksXmaxList();

	std::vector<int> &GetHistPeakTimeNbinsList();
	std::vector<double> &GetHistPeakTimeXminList();
	std::vector<double> &GetHistPeakTimeXmaxList();

	std::vector<int> &GetHistPeakAreaEvNbinsList();
	std::vector<double> &GetHistPeakAreaEvXminList();
	std::vector<double> &GetHistPeakAreaEvXmaxList();

private:
	std::ifstream file_input;
	std::string file_name_info;
	std::vector<std::string> ch_name_list;
	std::vector<int> ch_list;
	std::vector<bool> is_positive_polarity_type_list;
	std::vector<bool> is_local_baseline_list;
	std::vector<double> th_raw_list;
	std::vector<double> th_filtered_list;
	std::vector<unsigned int> peak_finder_version_list;

	std::vector<double> window_list;
	std::vector<double> local_baseline_window_list;
	std::vector<double> local_baseline_window_shift_list;
	std::vector<double> check_overlapping_window_list;
	std::vector<double> shrinking_of_left_tail_list;
	std::vector<double> shrinking_of_right_tail_list;
	std::vector<double> filtering_window_list;

	std::vector<int> hist_peak_amp_nbins_list;
	std::vector<double> hist_peak_amp_xmin_list;
	std::vector<double> hist_peak_amp_xmax_list;

	std::vector<int> hist_peak_area_nbins_list;
	std::vector<double> hist_peak_area_xmin_list;
	std::vector<double> hist_peak_area_xmax_list;

	std::vector<int> hist_n_peaks_nbins_list;
	std::vector<double> hist_n_peaks_xmin_list;
	std::vector<double> hist_n_peaks_xmax_list;

	std::vector<int> hist_peak_time_nbins_list;
	std::vector<double> hist_peak_time_xmin_list;
	std::vector<double> hist_peak_time_xmax_list;

	std::vector<int> hist_peak_area_ev_nbins_list;
	std::vector<double> hist_peak_area_ev_xmin_list;
	std::vector<double> hist_peak_area_ev_xmax_list;
};

