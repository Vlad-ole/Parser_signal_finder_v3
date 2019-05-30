#pragma once

//c++
#include <vector>
#include <utility>      // std::pair, std::make_pair

class Cv
{
public:
	Cv();
	~Cv();

	std::vector<double> GetPeakAmp();

private:
	std::vector<double> peak_amp;
};

