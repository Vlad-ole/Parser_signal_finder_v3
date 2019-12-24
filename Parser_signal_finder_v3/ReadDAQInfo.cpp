#include "ReadDAQInfo.h"

using namespace std;

ReadDAQInfo::ReadDAQInfo(std::string file_name_info)
{
	file_input.open(file_name_info, std::ifstream::in);
	if (file_input.is_open())
	{
		cout << "File has been opened correctly: " << file_name_info.c_str() << endl;
	}
	else
	{
		cout << "Can't open this file: " << file_name_info.c_str() << endl;
		system("pause");
		exit(1);
	}
}

void ReadDAQInfo::Read()
{
	std::string line;

	while (std::getline(file_input, line))
	{
		if (line[0] != '#')
		{
			std::istringstream iss(line);
			iss >> ns_per_point >> points_per_event_per_ch >> N_events_per_file_input >> N_events_per_file_output;
		}
	}
}

void ReadDAQInfo::Show()
{
	cout << "ns_per_point = " << ns_per_point << endl;
	cout << "points_per_event_per_ch = " << points_per_event_per_ch << endl;
	cout << "N_events_per_file_input = " << N_events_per_file_input << endl;
	cout << "N_events_per_file_output = " << N_events_per_file_output << endl;
}


double ReadDAQInfo::GetNsPerPoint()
{
	return ns_per_point;
}

int ReadDAQInfo::GetPointsPerEventPerCh()
{
	return points_per_event_per_ch;
}

int ReadDAQInfo::GetNEventsPerFileInput()
{
	return N_events_per_file_input;
}

int ReadDAQInfo::GetNEventsPerFileOutput()
{
	return N_events_per_file_output;
}


ReadDAQInfo::~ReadDAQInfo()
{
}
