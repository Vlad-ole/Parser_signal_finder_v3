#include "Peaks.h"



Peaks::Peaks()
{
	peak_start_stop_poits.clear();
	avr_peak_time.clear();
	peak_time.clear();
	peak_amp.clear();
	peak_area.clear();
	local_baseline_v.clear();
}


Peaks::~Peaks()
{
	;
}

ClassImp(Peaks)