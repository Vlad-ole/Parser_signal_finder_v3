#pragma once

//c++
#include <iostream>
#include <sstream>
#include <vector>
#include <iomanip>
#include <fstream>


class CalcDer
{
public:
	CalcDer(std::vector<double> &yv, const int param_n_points, const int order_of_derivative);
	void CalculateCoeffDer(int points, const int order_of_derivative);	
	void CalcFilteredWaveformGolay();
	std::vector<double> &GetFilteredWaveformGolay();

	~CalcDer();

private:
	static unsigned int objectCount;
	static std::vector<std::vector<double> > C_i_der;
	std::vector<double> yv;
	std::vector<double> yv_filtered;
	const int param_n_points;
	const int order_of_derivative;
};

