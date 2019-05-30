#pragma once

//c++
#include <vector>
#include <utility>      // std::pair, std::make_pair

//root cern
#include "TObject.h"


class Peaks : public TObject
{

public:
	Peaks();
	~Peaks();

	std::vector<std::pair<int, int>> peak_start_stop_poits;
	std::vector<double> avr_peak_time;
	std::vector<double> peak_time;
	std::vector<double> peak_amp;
	std::vector<double> peak_area;
	std::vector<double> local_baseline_v;

	ClassDef(Peaks, 8);
	
};

