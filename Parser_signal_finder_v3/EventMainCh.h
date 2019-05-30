#pragma once

//c++
#include <vector>
#include <utility>      // std::pair, std::make_pair

//root cern
#include "TObject.h"

//my
#include "Peaks.h"


class EventMainCh : public TObject
{
public:
	EventMainCh(/*unsigned int number_of_channels*/);
	~EventMainCh();

	void Clear();

	unsigned int event_number;
	std::vector<double> ymin;
	std::vector<double> ymax;
	std::vector<double> baseline_mean;
	std::vector<double> baseline_sigma;
	std::vector<Peaks*> peaks;

	//std::vector< std::vector< std::pair<double, double> > > peak_position_amp;
	//std::vector< std::vector< std::pair<double, double> > > peak_position_amp;


	ClassDef(EventMainCh, 9);
};

