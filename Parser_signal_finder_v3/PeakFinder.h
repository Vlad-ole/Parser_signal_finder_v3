#pragma once

#include <vector>

class PeakFinder
{
public:
	PeakFinder(std::vector<double>& yv_raw, std::vector<double>& yv_filtered, const unsigned int ns_per_point);
	~PeakFinder();

	std::vector<double> GetAvrPeakTime();
	std::vector<double> GetPeakTime();
	std::vector<double> GetPeakAmp();
	std::vector<std::pair<int, int>> GetPeakPositions();
	std::vector<double> GetLocalBaselineV();
	std::vector<double> GetPeakArea();
	

	void FindPeaksByAmp(const double th);

private:
	std::vector<double>& yv_raw;
	std::vector<double>& yv_filtered;

	const unsigned int ns_per_point;
	std::vector<std::pair<int, int>> pair_v;
	std::vector<double> avr_peak_time;
	std::vector<double> peak_time;
	std::vector<double> peak_amp;
	std::vector<double> local_baseline_v;
	std::vector<double> peak_area;

	const double window; //ns
	const double local_baseline_window; //ns
	const double local_baseline_window_shift; //ns
	const double check_overlapping_window; /*500*//*100 used for 46V and folder2 190704*/ /*optimum for SiPM = 250 ns*/; //ns
	//shrinking of left tail for raw signal relative to filtered signal
	const double shrinking_of_left_tail;  //ns
};

