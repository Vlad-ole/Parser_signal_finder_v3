#include "ReadData_CAEN_v2.h"

using namespace std;

ReadData_CAEN_v2::ReadData_CAEN_v2(std::string file_name_raw, int N_events_per_file, int points_per_event) : 
file_name_raw(file_name_raw), N_events_per_file(N_events_per_file), points_per_event(points_per_event)
{
	//vectors initialisation
	data_ev_point.resize(N_events_per_file);
	for (int temp_event_id = 0; temp_event_id < N_events_per_file; temp_event_id++)
	{
		data_ev_point[temp_event_id].resize(points_per_event);
	}

	run_size = N_events_per_file * points_per_event;	
	data_tmp.resize(run_size);
}

void ReadData_CAEN_v2::Read()
{
	FILE *f = fopen(file_name_raw.c_str(), "rb");

	if (f == NULL)
	{
		cout << "can't open this file: " << file_name_raw.c_str() << endl;
		system("pause");
		//fclose(f);
		exit(1);
	}

	fread(&data_tmp[0], sizeof(vector<short int>::value_type), run_size, f);
	fclose(f);


	for (int temp_event_id = 0; temp_event_id < N_events_per_file; temp_event_id++)
	{
		for (int j = 0; j < points_per_event; j++)
		{
			const int abs_point = j + temp_event_id * points_per_event;
			data_ev_point[temp_event_id][j] = data_tmp[abs_point];
		}
	}
}

std::vector< std::vector<short int> >& ReadData_CAEN_v2::GetDataEvPoint()
{
	return data_ev_point;
}


ReadData_CAEN_v2::~ReadData_CAEN_v2()
{
}
