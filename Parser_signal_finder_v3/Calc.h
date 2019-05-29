#pragma once

//c++
#include <iostream>
#include <sstream>
#include <vector>
#include <iomanip>
#include <fstream>


//my
#include "CalcDer.h" 
#include "PeakFinder.h"

class CalcDer;

class Calc
{
public:
	Calc(std::vector<double> &data_points, double ns_per_point, bool is_positive_polarity = true);
	~Calc();
	
	const bool is_positive_polarity;
	const double ns_per_point;

	void CalcBaselineMeanSigma(double from_ns, double to_ns);
	void SubtractBaseline();

	double GetBaselineMean();
	double GetBaselineSigma();
	double GetYmin();
	double GetYmax();
	std::vector<double> &GetFilteredWaveformGolay(const int param_n_points, const int order_of_derivative);
	

private:
	std::vector<double> &data_points;
	CalcDer *calc_der;
	double baseline_mean;
	double baseline_sigma;
};

