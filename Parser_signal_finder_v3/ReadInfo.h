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

private:
	std::ifstream file_input;
	std::string file_name_info;
	std::vector<int> ch_list;
	std::vector<bool> is_positive_polarity_type_list;
	std::vector<double> th_list;
};

