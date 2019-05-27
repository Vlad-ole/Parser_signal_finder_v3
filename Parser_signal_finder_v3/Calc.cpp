//c++
#include <algorithm>

//my
#include "Calc.h"
#include "TypeConvertion.h"

Calc::Calc(std::vector<double> &data_points, double ns_per_point, bool is_positive_polarity) : 
data_points(data_points), ns_per_point(ns_per_point), is_positive_polarity(is_positive_polarity)
{
	if ( !is_positive_polarity )
	{
		data_points = TypeConvertion::GetVectorDoubleInvert(data_points);
	}
}

double Calc::GetYmin()
{
	return *min_element(data_points.begin(), data_points.end());
}

double Calc::GetYmax()
{
	return *max_element(data_points.begin(), data_points.end());
}

Calc::~Calc()
{
}
