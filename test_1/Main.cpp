//c++
#include <iostream>
#include <sstream>
#include <vector>
#include <iomanip>
#include <fstream>
#include <omp.h> 

//my
#include "CalcDer.h"


using namespace std;

int main(int argc, char **argv)
{
	unsigned int points_per_event_per_ch = 40000/*9999*/;	
	unsigned int ns_per_point = 4/*16*/;

	vector<double> yv_filtered/*(points_per_event_per_ch)*/;
	vector<double> xv(points_per_event_per_ch);
	for (int i = 0; i < points_per_event_per_ch; i++)
		xv[i] = i * ns_per_point;

	vector<double> yv(points_per_event_per_ch);
	for (int i = 0; i < points_per_event_per_ch; i++)
	{
		yv[i] = /*std::rand()*/ rand() / (RAND_MAX + 1.);
	}
		
	CalcDer calc(yv, 21, 0 );
	calc.CalcFilteredWaveformGolay();
	yv_filtered = calc.GetFilteredWaveformGolay();

	cout << "all is ok" << endl;
	system("pause");
}