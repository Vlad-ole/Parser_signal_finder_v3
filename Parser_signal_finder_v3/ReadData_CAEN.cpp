#include "ReadData_CAEN.h"


using namespace std;



ReadData_CAEN::ReadData_CAEN(string file_name_raw, int N_events_per_file, int points_per_event)
{
	//vectors initialisation
	data_double.resize(N_events_per_file);
	for (int temp_event_id = 0; temp_event_id < N_events_per_file; temp_event_id++)
	{
		data_double[temp_event_id].resize(points_per_event);
	}
		

	const int run_size = N_events_per_file * points_per_event;



		vector<short int> data_tmp;
		data_tmp.resize(run_size);
	
		//I do not why, but c++ style of reading gives unexpected result, i.e. incorrect reading
		//so I chose usual c-style.
		//The problem in structures. I should use #pragma pack to write structures correctry to the disc.
		FILE *f = fopen(file_name_raw.c_str(), "rb");

		if (/*!file.is_open()*/f == NULL)
		{
			cout << "can't open this file: " << file_name_raw.c_str() << endl;
			system("pause");
			//fclose(f);
			exit(1);
		}

		fread(&data_tmp[0], sizeof(vector<short int>::value_type), run_size, f);
		fclose(f);

		//y points to volts
		for (int temp_event_id = 0; temp_event_id < N_events_per_file; temp_event_id++)
		{
			for (int j = 0; j < points_per_event; j++)
			{
				const int abs_point = j + temp_event_id * points_per_event;
				data_double[temp_event_id][j] = ((2 / 4095.0) * data_tmp[abs_point] - 1) * 1000;
				//cout << i << " " << j << " " << data[i][j] << " " <<  data_double[i][j] << endl;
				//if (ch == 0)
				//{
				//	f_out << j << "\t" << temp_event_id << "\t" << data_tmp[abs_point] << "\t" << data_double[temp_event_id][ch][j] << endl;
				//}
			}
		}		

	//}

}


ReadData_CAEN::~ReadData_CAEN()
{
}
