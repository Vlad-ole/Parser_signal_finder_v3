#include "ReadData.h"

ReadData::ReadData()
{
}

std::vector< std::vector< std::vector<double> > >& ReadData::GetDataDouble()
{
	return data_ev_ch_points;
}

std::vector<double>& ReadData::GetTimeArray()
{
	return time;
}

ReadData::~ReadData()
{
}
