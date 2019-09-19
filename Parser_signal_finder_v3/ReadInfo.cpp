#include "ReadInfo.h"

using namespace std;

ReadInfo::ReadInfo(std::string file_name_info) : file_name_info(file_name_info)
{
	file_input.open(file_name_info, std::ifstream::in);
	if (file_input.is_open())
	{
		cout << "File has been opened correctly: " << file_name_info.c_str() << endl;
	}
	else
	{
		cout << "Can't open this file: " << file_name_info.c_str() << endl;
		exit(1);
	}
}

void ReadInfo::Read()
{
	std::string line;

	while (std::getline(file_input, line))
	{
		if (line[0] != '#')
		{
			std::istringstream iss(line);
			int ch;
			bool is_positive_polarity_type;
			double th;
			double window;
			double local_baseline_window;
			double local_baseline_window_shift;
			double check_overlapping_window;
			double shrinking_of_left_tail;
			double shrinking_of_right_tail;
			double filtering_window;

			iss >> ch >> is_positive_polarity_type >> th >> window >> local_baseline_window >>
				local_baseline_window_shift >> check_overlapping_window >>
				shrinking_of_left_tail >> shrinking_of_right_tail >> filtering_window;
			
			ch_list.push_back(ch);
			is_positive_polarity_type_list.push_back(is_positive_polarity_type);
			th_list.push_back(th);
			window_list.push_back(window);
			local_baseline_window_list.push_back(local_baseline_window);
			local_baseline_window_shift_list.push_back(local_baseline_window_shift);
			check_overlapping_window_list.push_back(check_overlapping_window);
			shrinking_of_left_tail_list.push_back(shrinking_of_left_tail);
			shrinking_of_right_tail_list.push_back(shrinking_of_right_tail);
			filtering_window_list.push_back(filtering_window);
		}		
	}
}

void ReadInfo::Show()
{
	cout << "ch_list.size() = " << ch_list.size() << endl;
	cout << "is_positive_polarity_type_list.size() = " << is_positive_polarity_type_list.size() << endl;
	cout << "th_list.size() = " << th_list.size() << endl;
	
	cout << "i \t ch \t is_positive_polarity_type \t th" << endl;
	for (int i = 0; i < ch_list.size(); i++)
	{
		cout << i << "\t" << ch_list[i] << "\t" << is_positive_polarity_type_list[i] << "\t" << th_list[i] << endl;
	}
}

std::vector<int> &ReadInfo::GetChList()
{
	return ch_list;
}

std::vector<bool> &ReadInfo::GetIsPositivePolarityTypeList()
{
	return is_positive_polarity_type_list;
}

std::vector<double> &ReadInfo::GetThList()
{
	return th_list;
}

std::vector<double> &ReadInfo::GetWindowList()
{
	return window_list;
}

std::vector<double> &ReadInfo::GetLocalBaselineWindowList()
{
	return local_baseline_window_list;
}

std::vector<double> &ReadInfo::GetLocalBaselineWindowShiftList()
{
	return local_baseline_window_shift_list;
}

std::vector<double> &ReadInfo::GetCheckOverlappingWindowList()
{
	return check_overlapping_window_list;
}

std::vector<double> &ReadInfo::GetShrinkingOfLeftTailList()
{
	return shrinking_of_left_tail_list;
}

std::vector<double> &ReadInfo::GetShrinkingOfRightTailList()
{
	return shrinking_of_right_tail_list;
}

std::vector<double> &ReadInfo::GetFilteringWindowList()
{
	return filtering_window_list;
}

ReadInfo::~ReadInfo()
{
	file_input.close();
}
