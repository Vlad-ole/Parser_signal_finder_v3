#pragma once

#include "ReadData.h"
#include <string>
#include <iostream>
#include <vector>

class ReadData_CAEN :
	public ReadData
{
public:
	ReadData_CAEN(std::string file_name_raw, int N_events_per_file, int points_per_event);
	~ReadData_CAEN();
};

