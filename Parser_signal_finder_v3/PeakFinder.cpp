//c++
#include <algorithm>
#include <iterator>
#include <iostream>

//my
#include "PeakFinder.h"

using namespace std;

PeakFinder::PeakFinder(std::vector<double>& yv_data, unsigned int ns_per_point):
yv_data(yv_data), ns_per_point(ns_per_point)
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


void PeakFinder::FindPeaksByAmp(const double th)
{
	bool is_search = true;
	int position_tmp;
	double local_baseline;
	pair<int, int> pair_var;
	
	const double window = 200; //ns
	const double local_baseline_window = 350; //ns
	const double local_baseline_window_shift = 50; //ns
	const double check_overlapping_window = /*500*/100; //ns

	const int half_window_p = window / ns_per_point;
	const int local_baseline_window_p = local_baseline_window / ns_per_point;
	const int local_baseline_window_shift_p = local_baseline_window_shift / ns_per_point;
	const int check_overlapping_window_p = check_overlapping_window / ns_per_point;

	//th by amp
	for (int i = 0; i < yv_data.size(); i++)
	{
		//cout << yv_der[i] << endl;

		/*if (i > 8260)
		{
			std::cout << i << "\t" << yv_data[i] << endl;
		}*/


		if ((yv_data[i] > th) && is_search)
		{
			is_search = false;
			position_tmp = i;

			const double j_from = (i - local_baseline_window_shift_p - local_baseline_window_p) > 0 ? i - local_baseline_window_shift_p - local_baseline_window_p : 0;

			//v1 trivial local_baseline
			local_baseline = 0;
			
			//v2 calculation of local_baseline
			/*local_baseline = 0;
			for (int j = j_from; j < i - local_baseline_window_shift_p; j++)
			{
				local_baseline += yv_data[j];
			}
			local_baseline /= (i - local_baseline_window_shift_p - j_from);*/

			local_baseline_v.push_back(local_baseline);
			//cout << "local_baseline = " << local_baseline << endl;

			//find the start of the signal
			//v1 as minimun on the interval
			//pair_var.first = *min_element(yv_data.begin(), yv_data.begin());
			
			//v2 (as minimun on the interval) or (as intersection with baseline)
			pair_var.first = distance(yv_data.begin(), min_element(yv_data.begin() + j_from, yv_data.begin() + i));
			for (int j = i; j > j_from; j--)
			{
				if (yv_data[j] < local_baseline)
				{
					pair_var.first = j;
					break;
				}
			}

		}

		if (!is_search && (i > position_tmp) && (yv_data[i] < th))
		{
			if (yv_data[i] < local_baseline)
			{
				bool is_overlapped = false;
				if ((i + check_overlapping_window_p) < yv_data.size())//can be error
				{
					for (int j = i; j < i + check_overlapping_window_p; j++)
					{
						if (yv_data[j] > th)//error?
						{
							is_overlapped = true;
							break;
						}
					}
				}

				if (!is_overlapped)
				{
					is_search = true;
					pair_var.second = i;
					pair_v.push_back(pair_var);
					avr_peak_time.push_back((pair_var.first + pair_var.second) / 2.0 * ns_per_point);

					double peak_area_tmp = 0;					
					for (int j = pair_var.first; j < pair_var.second; j++)
					{
						peak_area_tmp += yv_data[j] - local_baseline;
					}
					peak_area.push_back(peak_area_tmp * ns_per_point);

					vector<double>::iterator it_max = max_element(yv_data.begin() + pair_var.first, yv_data.begin() + pair_var.second);
					peak_amp.push_back(*it_max);
					peak_time.push_back( distance(yv_data.begin(), it_max) * ns_per_point );

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


