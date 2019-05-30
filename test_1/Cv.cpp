#include "Cv.h"


Cv::Cv()
{
	peak_amp.resize(2, 10);
}

std::vector<double> Cv::GetPeakAmp()
{
	return peak_amp;
}


Cv::~Cv()
{
}
