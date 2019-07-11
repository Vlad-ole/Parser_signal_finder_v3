#pragma once

//c++
#include <iostream>
#include <sstream>
#include <vector>
#include <iomanip>
#include <fstream>
#include <string>

class ReadDAQInfo
{
public:
	ReadDAQInfo(std::string file_name_info);
	~ReadDAQInfo();

	void Read();
	void Show();

	double GetNsPerPoint();
	int GetPointsPerEventPerCh();
	int GetNEventsPerFileInput();
	int GetNEventsPerFileOutput();

private:
	std::ifstream file_input;

	double ns_per_point;
	int points_per_event_per_ch;
	int N_events_per_file_input;
	int N_events_per_file_output;	
};

