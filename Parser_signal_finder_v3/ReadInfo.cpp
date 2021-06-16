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
		system("pause");
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
			string ch_name;
			int ch;
			bool is_positive_polarity_type;
			bool is_local_baseline;
			double th_raw;
			double th_filtered;
			unsigned int peak_finder_version;
			double window;
			double local_baseline_window;
			double local_baseline_window_shift;
			double check_overlapping_window;
			double shrinking_of_left_tail;
			double shrinking_of_right_tail;
			double filtering_window;
			
			int hist_peak_amp_nbins;
			double hist_peak_amp_xmin;
			double hist_peak_amp_xmax;

			int hist_peak_area_nbins;
			double hist_peak_area_xmin;
			double hist_peak_area_xmax;

			int hist_n_peaks_nbins;
			double hist_n_peaks_xmin;
			double hist_n_peaks_xmax;

			int hist_peak_time_nbins;
			double hist_peak_time_xmin;
			double hist_peak_time_xmax;

			int hist_peak_area_ev_nbins;
			double hist_peak_area_ev_xmin;
			double hist_peak_area_ev_xmax;

			iss >> ch_name >> ch >> is_positive_polarity_type >> th_raw >> th_filtered >> peak_finder_version >>
				is_local_baseline >> window >> local_baseline_window >>
				local_baseline_window_shift >> check_overlapping_window >>
				shrinking_of_left_tail >> shrinking_of_right_tail >> filtering_window >>
				hist_peak_amp_nbins >> hist_peak_amp_xmin >> hist_peak_amp_xmax >>
				hist_peak_area_nbins >> hist_peak_area_xmin >> hist_peak_area_xmax >>
				hist_n_peaks_nbins >> hist_n_peaks_xmin >> hist_n_peaks_xmax >> 
				hist_peak_time_nbins >> hist_peak_time_xmin >> hist_peak_time_xmax >>
				hist_peak_area_ev_nbins >> hist_peak_area_ev_xmin >> hist_peak_area_ev_xmax;
			
			ch_name_list.push_back(ch_name);
			ch_list.push_back(ch);
			is_positive_polarity_type_list.push_back(is_positive_polarity_type);
			th_raw_list.push_back(th_raw);
			th_filtered_list.push_back(th_filtered);
			peak_finder_version_list.push_back(peak_finder_version);
			is_local_baseline_list.push_back(is_local_baseline);
			window_list.push_back(window);
			local_baseline_window_list.push_back(local_baseline_window);
			local_baseline_window_shift_list.push_back(local_baseline_window_shift);
			check_overlapping_window_list.push_back(check_overlapping_window);
			shrinking_of_left_tail_list.push_back(shrinking_of_left_tail);
			shrinking_of_right_tail_list.push_back(shrinking_of_right_tail);
			filtering_window_list.push_back(filtering_window);

			hist_peak_amp_nbins_list.push_back(hist_peak_amp_nbins);
			hist_peak_amp_xmin_list.push_back(hist_peak_amp_xmin);
			hist_peak_amp_xmax_list.push_back(hist_peak_amp_xmax);

			hist_peak_area_nbins_list.push_back(hist_peak_area_nbins);
			hist_peak_area_xmin_list.push_back(hist_peak_area_xmin);
			hist_peak_area_xmax_list.push_back(hist_peak_area_xmax);

			hist_n_peaks_nbins_list.push_back(hist_n_peaks_nbins);
			hist_n_peaks_xmin_list.push_back(hist_n_peaks_xmin);
			hist_n_peaks_xmax_list.push_back(hist_n_peaks_xmax);

			hist_peak_time_nbins_list.push_back(hist_peak_time_nbins);
			hist_peak_time_xmin_list.push_back(hist_peak_time_xmin);
			hist_peak_time_xmax_list.push_back(hist_peak_time_xmax);

			hist_peak_area_ev_nbins_list.push_back(hist_peak_area_ev_nbins);
			hist_peak_area_ev_xmin_list.push_back(hist_peak_area_ev_xmin);
			hist_peak_area_ev_xmax_list.push_back(hist_peak_area_ev_xmax);
		}		
	}
}

void ReadInfo::Show()
{
	cout << "ch_list.size() = " << ch_list.size() << endl;
	cout << "is_positive_polarity_type_list.size() = " << is_positive_polarity_type_list.size() << endl;
	cout << "th_raw_list.size() = " << th_raw_list.size() << endl;
	
	cout << "i \t ch \t pol. \t th_raw \t th_fil \t ver." << endl;
	for (int i = 0; i < ch_list.size(); i++)
	{
		cout << i << "\t" << ch_list[i] << "\t" << is_positive_polarity_type_list[i] << "\t" << th_raw_list[i] << "\t" << th_filtered_list[i] <<
			"\t" << peak_finder_version_list[i] << endl;
	}

	system("pause");
}

std::vector<std::string> &ReadInfo::GetChNameList()
{
	return ch_name_list;
}

std::vector<int> &ReadInfo::GetChList()
{
	return ch_list;
}

std::vector<bool> &ReadInfo::GetIsPositivePolarityTypeList()
{
	return is_positive_polarity_type_list;
}

std::vector<double> &ReadInfo::GetThRawList()
{
	return th_raw_list;
}

std::vector<double> &ReadInfo::GetThFilteredList()
{
	return th_filtered_list;
}

std::vector<unsigned int> &ReadInfo::GetPeakFinderVersionList()
{
	return peak_finder_version_list;
}

std::vector<bool> &ReadInfo::GetIsLocalBaselineList()
{
	return is_local_baseline_list;
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

std::vector<int> &ReadInfo::GetHistPeakAmpNbinsList()
{
	return hist_peak_amp_nbins_list;
}

std::vector<double> &ReadInfo::GetHistPeakAmpXminList()
{
	return hist_peak_amp_xmin_list;
}

std::vector<double> &ReadInfo::GetHistPeakAmpXmaxList()
{
	return hist_peak_amp_xmax_list;
}

std::vector<int> &ReadInfo::GetHistPeakAreaNbinsList()
{
	return hist_peak_area_nbins_list;
}

std::vector<double> &ReadInfo::GetHistPeakAreaXminList()
{
	return hist_peak_area_xmin_list;
}

std::vector<double> &ReadInfo::GetHistPeakAreaXmaxList()
{
	return hist_peak_area_xmax_list;
}

std::vector<int> &ReadInfo::GetHistNpeaksNbinsList()
{
	return hist_n_peaks_nbins_list;
}
std::vector<double> &ReadInfo::GetHistNpeaksXminList()
{
	return hist_n_peaks_xmin_list;
}
std::vector<double> &ReadInfo::GetHistNpeaksXmaxList()
{
	return hist_n_peaks_xmax_list;
}

std::vector<int> &ReadInfo::GetHistPeakTimeNbinsList()
{
	return hist_peak_time_nbins_list;
}

std::vector<double> &ReadInfo::GetHistPeakTimeXminList()
{
	return hist_peak_time_xmin_list;
}
std::vector<double> &ReadInfo::GetHistPeakTimeXmaxList()
{
	return hist_peak_time_xmax_list;
}

std::vector<int> &ReadInfo::GetHistPeakAreaEvNbinsList()
{
	return hist_peak_area_ev_nbins_list;
}

std::vector<double> &ReadInfo::GetHistPeakAreaEvXminList()
{
	return hist_peak_area_ev_xmin_list;
}

std::vector<double> &ReadInfo::GetHistPeakAreaEvXmaxList()
{
	return hist_peak_area_ev_xmax_list;
}

ReadInfo::~ReadInfo()
{
	file_input.close();
}
