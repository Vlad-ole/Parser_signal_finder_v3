#pragma once

#include "ReadData.h"
#include <string>
#include <iostream>
#include <vector>

class ReadData_CAEN :
	public ReadData
{
public:
	ReadData_CAEN(std::string file_name_raw, unsigned int N_events_per_file, unsigned int N_ch, unsigned int points_per_event_per_ch);
	~ReadData_CAEN();
};

