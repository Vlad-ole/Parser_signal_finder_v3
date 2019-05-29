#pragma once

#include <vector>

class PeakFinder
{
public:
	PeakFinder(std::vector<double>& yv_data, const unsigned int ns_per_point);
	~PeakFinder();

	std::vector<double> GetAvrPeakTime();
	std::vector<double> GetPeakTime();
	std::vector<double> GetPeakAmp();
	std::vector<std::pair<int, int>> GetPeakPositions();
	std::vector<double> GetLocalBaselineV();
	std::vector<double> GetPeakArea();
	

	void FindPeaksByAmp(const double th);

private:
	std::vector<double>& yv_data;
	const unsigned int ns_per_point;
	std::vector<std::pair<int, int>> pair_v;
	std::vector<double> avr_peak_time;
	std::vector<double> peak_time;
	std::vector<double> peak_amp;
	std::vector<double> local_baseline_v;
	std::vector<double> peak_area;
};

