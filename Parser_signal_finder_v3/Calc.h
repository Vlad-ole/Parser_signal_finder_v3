#pragma once

//c++
#include <iostream>
#include <sstream>
#include <vector>
#include <iomanip>
#include <fstream>


class Calc
{
public:
	Calc(std::vector<double> &data_points, double ns_per_point, bool is_positive_polarity = true);
	~Calc();
	
	const bool is_positive_polarity;
	const double ns_per_point;

	//double ymin;
	//double ymax;

	double GetYmin();
	double GetYmax();

private:
	std::vector<double> &data_points;

};

