#pragma once

#include "TObject.h"

#include <vector>
#include <utility>      // std::pair, std::make_pair

class EventMainCh : public TObject
{
public:
	EventMainCh(/*unsigned int number_of_channels*/);
	~EventMainCh();

	void Clear();

	unsigned int event_number;
	std::vector<double> ymin;
	std::vector<double> ymax;
	std::vector<double> baseline;
	std::vector<int> peaks_number;

	//std::vector< std::vector< std::pair<double, double> > > peak_position_amp;


	ClassDef(EventMainCh, 6);
};

