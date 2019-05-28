#pragma once

//c++
#include <iostream>
#include <sstream>
#include <vector>
#include <iomanip>
#include <fstream>

//my
#include "CalcDer.h" 

class CalcDer;

class Calc
{
public:
	Calc(std::vector<double> &data_points, double ns_per_point, bool is_positive_polarity = true);
	~Calc();
	
	const bool is_positive_polarity;
	const double ns_per_point;

	//double ymin;
	//double ymax;

	double GetBaselineMean(double from_ns, double to_ns);
	double GetYmin();
	double GetYmax();
	std::vector<double> &GetFilteredWaveformGolay(const int param_n_points, const int order_of_derivative);
	
private:
	std::vector<double> &data_points;
	CalcDer *calc_der;
};

