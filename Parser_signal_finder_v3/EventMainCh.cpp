#include "EventMainCh.h"

using namespace std;

ClassImp(EventMainCh)

EventMainCh::EventMainCh(/*unsigned int number_of_channels*/) /*: 
ymin(number_of_channels, 0.), ymax(number_of_channels, 0.), 
peaks_number(number_of_channels, 0)*//*, peak_position_amp(number_of_channels)*/
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
	baseline.clear();
	peaks_number.clear();
	

	//for (int i = 0; i < peak_position_amp.size(); i++)
	//{
	//	peak_position_amp[i].clear();
	//}

}

EventMainCh::~EventMainCh()
{
	;
}


