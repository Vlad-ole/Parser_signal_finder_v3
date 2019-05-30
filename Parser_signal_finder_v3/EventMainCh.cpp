#include "EventMainCh.h"

using namespace std;

ClassImp(EventMainCh)

EventMainCh::EventMainCh() : peaks(NULL)
{
	event_number = 0.;
}

void EventMainCh::Clear()
{
	event_number = 0.;
	/*std::fill(ymin.begin(), ymin.end(), 0);
	std::fill(ymax.begin(), ymax.end(), 0);
	std::fill(peaks_number.begin(), peaks_number.end(), 0);*/

	ymin.clear();
	ymax.clear();
	baseline_mean.clear();
	baseline_sigma.clear();

	for (size_t i = 0; i<peaks.size(); i++)
		delete peaks.at(i);
	peaks.clear();


	//for (int i = 0; i < peak_position_amp.size(); i++)
	//{
	//	peak_position_amp[i].clear();
	//}

}

EventMainCh::~EventMainCh()
{
	;
}


