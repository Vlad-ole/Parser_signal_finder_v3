//c++
#include <algorithm>
#include <iterator>
#include <iostream>

//my
#include "PeakFinder.h"

using namespace std;

PeakFinder::PeakFinder(std::vector<double>& yv_raw, std::vector<double>& yv_filtered, const unsigned int ns_per_point, const unsigned int peak_finder_version,
	const double window, const double local_baseline_window, const double local_baseline_window_shift, 
	const double check_overlapping_window, const double shrinking_of_left_tail, const double shrinking_of_right_tail, const bool is_local_baseline) :
yv_raw(yv_raw), yv_filtered(yv_filtered), ns_per_point(ns_per_point), 
window(window), local_baseline_window(local_baseline_window), peak_finder_version(peak_finder_version),
local_baseline_window_shift(local_baseline_window_shift), check_overlapping_window(check_overlapping_window),
shrinking_of_left_tail(shrinking_of_left_tail), shrinking_of_right_tail(shrinking_of_right_tail), is_local_baseline(is_local_baseline)
{
}


PeakFinder::~PeakFinder()
{
}

std::vector<std::pair<int, int>> PeakFinder::GetPeakPositions()
{
	return pair_v;
}

std::vector<double> PeakFinder::GetLocalBaselineV()
{
	return local_baseline_v;
}

std::vector<double> PeakFinder::GetAvrPeakTime()
{
	return avr_peak_time;
}

std::vector<double> PeakFinder::GetPeakTime()
{
	return peak_time;
}

std::vector<double> PeakFinder::GetPeakArea()
{
	return peak_area;
}

std::vector<double> PeakFinder::GetPeakAmp()
{
	return peak_amp;
}


void PeakFinder::FindPeaks(const double th_raw, const double th_filtered)
{
	bool is_search = true;
	int position_tmp;
	double local_baseline;
	pair<int, int> pair_var;
	
	//const double window = 200; //ns
	//const double local_baseline_window = 350; //ns
	//const double local_baseline_window_shift = 50; //ns
	//const double check_overlapping_window = /*500*//*100 used for 46V and folder2 190704*/250; //ns
	////shrinking of left tail for raw signal relative to filtered signal
	//const double shrinking_of_left_tail = 64;  //ns

	const int half_window_p = window / ns_per_point;
	const int local_baseline_window_p = local_baseline_window / ns_per_point;
	const int local_baseline_window_shift_p = local_baseline_window_shift / ns_per_point;
	const int check_overlapping_window_p = check_overlapping_window / ns_per_point;
	const int shrinking_of_left_tail_p = shrinking_of_left_tail / ns_per_point;
	const int shrinking_of_right_tail_p = shrinking_of_right_tail / ns_per_point;

	//v1, th by yv_filtered
	if (peak_finder_version == 1)
	{
		for (int i = 0; i < yv_filtered.size(); i++)
		{
			//cout << yv_der[i] << endl;

			/*if (i > 8260)
			{
			std::cout << i << "\t" << yv_data[i] << endl;
			}*/


			if ((yv_filtered[i] > th_filtered) && is_search)
			{
				is_search = false;
				position_tmp = i;

				const int j_from = (i - local_baseline_window_shift_p - local_baseline_window_p) > 0 ? i - local_baseline_window_shift_p - local_baseline_window_p : 0;
				const int j_from_raw = j_from + shrinking_of_left_tail_p;

				//v1 trivial local_baseline
				//local_baseline = 0;

				//v2 calculation of local_baseline
				/*local_baseline = 0;
				for (int j = j_from; j < i - local_baseline_window_shift_p; j++)
				{
				local_baseline += yv_filtered[j];
				}
				local_baseline /= (i - local_baseline_window_shift_p - j_from);*/

				//v3 calculation of local_baseline
				local_baseline = 0;

				if (is_local_baseline)
				{
					for (int j = j_from_raw; j < (i - local_baseline_window_shift_p + shrinking_of_left_tail_p); j++)
					{
						local_baseline += yv_raw[j];
					}
					local_baseline /= (i - local_baseline_window_shift_p - j_from);
				}

				local_baseline_v.push_back(local_baseline);
				//cout << "local_baseline = " << local_baseline << endl;

				//find the start of the signal
				//v1 as minimun on the interval
				//pair_var.first = *min_element(yv_data.begin(), yv_data.begin());

				//v2 (as minimun on the interval) or (as intersection with baseline)
				pair_var.first = distance(yv_filtered.begin(), min_element(yv_filtered.begin() + j_from, yv_filtered.begin() + i));
				for (int j = i; j > j_from; j--)
				{
					if (yv_filtered[j] < local_baseline)
					{
						pair_var.first = j;
						break;
					}
				}
				pair_var.first += shrinking_of_left_tail_p;

			}

			if (!is_search && (i > (position_tmp + shrinking_of_left_tail_p)) && (yv_filtered[i] < th_filtered))
			{
				if (yv_filtered[i] < local_baseline)
				{
					bool is_overlapped = false;
					if ((i + check_overlapping_window_p) < yv_filtered.size())//can be error
					{
						for (int j = i; j < i + check_overlapping_window_p; j++)
						{
							if (yv_filtered[j] > th_filtered)//error?
							{
								is_overlapped = true;
								break;
							}
						}
					}

					if (!is_overlapped)
					{
						is_search = true;
						pair_var.second = i - shrinking_of_right_tail_p;//check this
						//to avoid pair_var.first >= pair_var.second due to fasle indentification because of noise 
						if ((pair_var.second - pair_var.first) <= 3)
							pair_var.second += (4 - (pair_var.second - pair_var.first));


						pair_v.push_back(pair_var);
						avr_peak_time.push_back((pair_var.first + pair_var.second) / 2.0 * ns_per_point);

						double peak_area_tmp = 0;
						for (int j = pair_var.first; j < pair_var.second; j++)
						{
							peak_area_tmp += yv_raw[j] - local_baseline;
						}
						peak_area.push_back(peak_area_tmp * ns_per_point);

						if (pair_var.first >= pair_var.second)
						{
							cout << "err: pair_var.first >= pair_var.second" << endl;
							system("pause");
							exit(1);
						}
						vector<double>::iterator it_max = max_element(yv_raw.begin() + pair_var.first, yv_raw.begin() + pair_var.second);
						peak_amp.push_back(*it_max);
						peak_time.push_back(distance(yv_raw.begin(), it_max) * ns_per_point);

						//cout << pair_var.first << "\t" << pair_var.second << endl;
					}
				}
			}

			//if (!is_search)
			//{
			//	if (yv[i] > pair_value.second)
			//	{
			//		pair_value.first = i;
			//		pair_value.second = yv[i];
			//	}
			//}

			//if ((yv[i] < th) && !is_search)
			//{
			//	is_search = true;
			//	peak_position.push_back(pair_value.first);
			//}
		}
	}

	// v2, th by yv_raw, using information about yv_filtered 
	if (peak_finder_version == 2)
	{
		for (int i = 0; i < yv_raw.size(); i++)
		{

			if ((yv_raw[i] > th_raw) && (yv_filtered[i] > th_filtered) && is_search)
			{
				is_search = false;
				position_tmp = i;

				const int j_from = (i - local_baseline_window_shift_p - local_baseline_window_p) > 0 ? i - local_baseline_window_shift_p - local_baseline_window_p : 0;
				const int j_from_raw = j_from + shrinking_of_left_tail_p;

				//v1 trivial local_baseline
				//local_baseline = 0;

				//v2 calculation of local_baseline
				/*local_baseline = 0;
				for (int j = j_from; j < i - local_baseline_window_shift_p; j++)
				{
				local_baseline += yv_filtered[j];
				}
				local_baseline /= (i - local_baseline_window_shift_p - j_from);*/

				//v3 calculation of local_baseline
				local_baseline = 0;

				if (is_local_baseline)
				{
					for (int j = j_from_raw; j < (i - local_baseline_window_shift_p + shrinking_of_left_tail_p); j++)
					{
						local_baseline += yv_raw[j];
					}
					local_baseline /= (i - local_baseline_window_shift_p - j_from);
				}

				local_baseline_v.push_back(local_baseline);
				//cout << "local_baseline = " << local_baseline << endl;

				//find the start of the signal
				//v1 as minimun on the interval
				//pair_var.first = *min_element(yv_data.begin(), yv_data.begin());

				//v2 (as minimun on the interval) or (as intersection with baseline)
				pair_var.first = distance(yv_raw.begin(), min_element(yv_raw.begin() + j_from, yv_raw.begin() + i));
				for (int j = i; j > j_from; j--)
				{
					if (yv_raw[j] < local_baseline)
					{
						pair_var.first = j;
						break;
					}
				}
				pair_var.first += shrinking_of_left_tail_p;

			}

			if (!is_search && (i > (position_tmp)) && (yv_raw[i] < th_raw))
			{
				if (yv_raw[i] < local_baseline)
				{
					bool is_overlapped = false;
					if ((i + check_overlapping_window_p) < yv_raw.size())//can be error
					{
						for (int j = i; j < i + check_overlapping_window_p; j++)
						{
							if (yv_raw[j] > th_raw)//error?
							{
								is_overlapped = true;
								break;
							}
						}
					}

					if (!is_overlapped)
					{
						is_search = true;
						pair_var.second = i - shrinking_of_right_tail_p;//check this
						//to avoid pair_var.first >= pair_var.second due to fasle indentification because of noise 
						if ((pair_var.second - pair_var.first) <= 3)
							pair_var.second += (4 - (pair_var.second - pair_var.first));


						pair_v.push_back(pair_var);
						avr_peak_time.push_back((pair_var.first + pair_var.second) / 2.0 * ns_per_point);

						double peak_area_tmp = 0;
						for (int j = pair_var.first; j < pair_var.second; j++)
						{
							peak_area_tmp += yv_raw[j] - local_baseline;
						}
						peak_area.push_back(peak_area_tmp * ns_per_point);

						if (pair_var.first >= pair_var.second)
						{
							cout << "err: pair_var.first >= pair_var.second" << endl;
							system("pause");
							exit(1);
						}
						vector<double>::iterator it_max = max_element(yv_raw.begin() + pair_var.first, yv_raw.begin() + pair_var.second);
						peak_amp.push_back(*it_max);
						peak_time.push_back(distance(yv_raw.begin(), it_max) * ns_per_point);

					}
				}
			}


		}
	}
	
}


