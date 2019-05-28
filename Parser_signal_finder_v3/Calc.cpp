//c++
#include <algorithm>

//my
#include "Calc.h"
#include "CalcDer.h" 
#include "TypeConvertion.h"

using namespace std;

//int Calc::objectCount = 0;
//vector<vector<double> > Calc::C_i_der(2);

Calc::Calc(std::vector<double> &data_points, double ns_per_point, bool is_positive_polarity) : 
data_points(data_points), ns_per_point(ns_per_point), is_positive_polarity(is_positive_polarity)
{
	if ( !is_positive_polarity )
	{
		data_points = TypeConvertion::GetVectorDoubleInvert(data_points);
	}

	//if (objectCount == 0)
	//{

	//}

	//objectCount++;
}

double Calc::GetYmin()
{
	return *min_element(data_points.begin(), data_points.end());
}

double Calc::GetYmax()
{
	return *max_element(data_points.begin(), data_points.end());
}

double Calc::GetBaselineMean(double from_ns, double to_ns)
{
	const int from_point = from_ns / ns_per_point;
	const int to_point = to_ns / ns_per_point;
	const int n_points = to_point - from_point + 1;

	double baseline = 0;
	for (int i = from_point; i < to_point; i++)
	{
		baseline += data_points[i];
	}

	baseline /= n_points;
	
	return baseline;
}

vector<double> &Calc::GetFilteredWaveformGolay(const int param_n_points, const int order_of_derivative)
{
	calc_der = new CalcDer(data_points, param_n_points, order_of_derivative);
	calc_der->CalcFilteredWaveformGolay();

	return calc_der->GetFilteredWaveformGolay();
}



Calc::~Calc()
{
	delete calc_der;
}
