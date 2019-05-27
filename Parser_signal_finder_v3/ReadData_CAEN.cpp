#include "ReadData_CAEN.h"


using namespace std;



ReadData_CAEN::ReadData_CAEN(string file_name_raw, unsigned int N_events_per_file, unsigned int N_ch, unsigned int points_per_event_per_ch)
{
	//vectors initialisation
	data_ev_ch_points.resize(N_events_per_file);
	
	for (int ev = 0; ev < N_events_per_file; ev++)
	{ 
		data_ev_ch_points[ev].resize(N_ch);
		for (int ch = 0; ch < N_ch; ch++)
		{
			data_ev_ch_points[ev][ch].resize(points_per_event_per_ch);
		}
	}
		

	const int run_size = N_events_per_file * N_ch * points_per_event_per_ch;
	

		vector<short int> data_tmp;		
		//data_tmp.resize(1000*1000*200);
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
		else
		{
			cout << "File was opened correctly: " << file_name_raw.c_str() << endl;
		}

		fread(&data_tmp[0], sizeof(vector<short int>::value_type), run_size, f);
		fclose(f);

		//y points to volts
		for (int ev = 0; ev < N_events_per_file; ev++)
		{
			for (int ch = 0; ch < N_ch; ch++)
			{
				for (int point = 0; point < points_per_event_per_ch; point++)
				{
					const int abs_point = point + ch * points_per_event_per_ch + ev * points_per_event_per_ch * N_ch;
					data_ev_ch_points[ev][ch][point] = ((2 / 4095.0) * data_tmp[abs_point] - 1) * 1000;
				}
			}			
		}		

	//}

}


ReadData_CAEN::~ReadData_CAEN()
{
}
