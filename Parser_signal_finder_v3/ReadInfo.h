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

	std::vector<int> &GetChList();
	std::vector<bool> &GetIsPositivePolarityTypeList();
	std::vector<double> &GetThList();

	std::vector<double> &GetWindowList();
	std::vector<double> &GetLocalBaselineWindowList();
	std::vector<double> &GetLocalBaselineWindowShiftList();
	std::vector<double> &GetCheckOverlappingWindowList();
	std::vector<double> &GetShrinkingOfLeftTailList();
	std::vector<double> &GetFilteringWindowList();

private:
	std::ifstream file_input;
	std::string file_name_info;
	std::vector<int> ch_list;
	std::vector<bool> is_positive_polarity_type_list;
	std::vector<double> th_list;

	std::vector<double> window_list;
	std::vector<double> local_baseline_window_list;
	std::vector<double> local_baseline_window_shift_list;
	std::vector<double> check_overlapping_window_list;
	std::vector<double> shrinking_of_left_tail_list;
	std::vector<double> filtering_window_list;
};

