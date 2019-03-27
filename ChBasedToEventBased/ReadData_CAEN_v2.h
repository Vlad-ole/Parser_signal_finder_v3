#pragma once

#include <vector>
#include <iostream>
#include <string>

class ReadData_CAEN_v2
{
public:
	ReadData_CAEN_v2(std::string file_name_raw, int N_events_per_file, int points_per_event);
	~ReadData_CAEN_v2();

	std::vector< std::vector<short int> >& GetDataEvPoint();
	void Read();

	std::vector< std::vector<short int> > data_ev_point;
	std::string file_name_raw;
	int N_events_per_file;
	int points_per_event;
	std::vector<short int> data_tmp;
	int run_size;
};

