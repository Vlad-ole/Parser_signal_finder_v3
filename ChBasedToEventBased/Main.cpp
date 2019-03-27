//c++
#include <iostream>
#include <sstream>
#include <vector>
#include <iomanip>
#include <fstream>
#include <omp.h> 

//my
#include "ReadData_CAEN_v2.h"

using namespace std;

int main(int argc, char **argv)
{
	
	//Read DAQ_info.txt
	const unsigned short ns_per_point = 16;
	const unsigned int points_per_event = 9999;
	const unsigned short N_events_per_file = 1000;
	//vector<unsigned short> ch_list= {0,1,2,3,4,5,6,7,32,33,34,35,36,37,38,39,40,41,42,43,44,48,49,50,51,52,53,54,55,56,57,58,59};
	//vector<unsigned short> ch_list = {0, 1, 2, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59};
	vector<unsigned short> ch_list = { 0, 1, 2};
	
	unsigned int start_run_number = 323;
	unsigned int stop_run_number = 324;

	
	unsigned int file_counter = 0;
	for (int run_number = start_run_number; run_number <= stop_run_number; run_number++)
	{
		cout << "run_" << run_number << endl;	
		cout << "\t Read file" << endl;

		vector< vector< vector<short int> > > data_ch_event_point;
		data_ch_event_point.resize(ch_list.size());		
		
		for (int ch = 0; ch < ch_list.size(); ch++)
		{
			cout << "\t \t ch_" << ch_list[ch] << endl;
			
			data_ch_event_point[ch].resize(N_events_per_file);
			
			string common_path = "D:\\Data_work\\Reco_test\\171123_caen_raw\\x_ray_20kV_PMT550_0dB_coll_2mm\\";
			ostringstream file_name_raw_in_osst;
			file_name_raw_in_osst << common_path << "run_" << run_number << "__ch_" << ch_list[ch] << ".dat";
			string file_name_raw_in = file_name_raw_in_osst.str();

			ReadData_CAEN_v2 rdt(file_name_raw_in, N_events_per_file, points_per_event);
			rdt.Read();

			std::vector< std::vector<short int> > data_ev_point = rdt.GetDataEvPoint();

			
			
			for (int ev = 0; ev < N_events_per_file; ev++)
			{
				data_ch_event_point[ch][ev].resize(points_per_event);
				
				for (int point = 0; point < points_per_event; point++)
				{
					data_ch_event_point[ch][ev][point] = data_ev_point[ev][point];
				}
			}
			
			
			//data_ch_event[ch][]
		}

		cout << "\t Write file" << endl;
		file_counter++;
		string common_path_out = "D:\\Data_work\\Reco_test\\171123_caen_raw\\x_ray_20kV_PMT550_0dB_coll_2mm_mod\\";
		ostringstream file_name_raw_out_osst;
		file_name_raw_out_osst << common_path_out << "file_" << file_counter << ".dat";
		string file_name_raw_out = file_name_raw_out_osst.str();
		FILE *f_out = fopen(file_name_raw_out.c_str(), "wb");
		unsigned int file_size = N_events_per_file * ch_list.size() * points_per_event;
		unsigned int file_block_size = ch_list.size() * points_per_event;
		//ofstream f_out(file_name_raw_out);
		
		//v1
		/*for (int ev = 0; ev < N_events_per_file; ev++)
		{
			cout << "\t ev_" << ev << endl;
			for (int ch = 0; ch < ch_list.size(); ch++)
			{
				for (int point = 0; point < points_per_event; point++)
				{
					f_out << data_ch_event_point[ch][ev][point] << endl;
				}
			}
		}	*/

		//v2
		//vector< vector< vector<short int> > > data_event_ch_point;
		//data_event_ch_point.resize(N_events_per_file);
		//for (int ev = 0; ev < N_events_per_file; ev++)
		//{
		//	data_event_ch_point[ev].resize(ch_list.size());
		//	for (int ch = 0; ch < ch_list.size(); ch++)
		//	{
		//		data_event_ch_point[ev][ch].resize(points_per_event);
		//	}
		//}

		//for (int ch = 0; ch < ch_list.size(); ch++)
		//{
		//	cout << "\t ch_" << ch_list[ch] << endl;
		//	for (int ev = 0; ev < N_events_per_file; ev++)
		//	{
		//		for (int point = 0; point < points_per_event; point++)
		//		{
		//			//f_out << data_ch_event_point[ch][ev][point] << endl;
		//			data_event_ch_point[ev][ch][point] = data_ch_event_point[ch][ev][point];
		//		}
		//	}
		//}

		for (int ev = 0; ev < N_events_per_file; ev++)
		{
			if (ev % 100 == 0)
				cout << "\t \t ev_" << ev << endl;
			for (int ch = 0; ch < ch_list.size(); ch++)
			{
				fwrite(&data_ch_event_point[ch][ev][0], sizeof(vector<short int>::value_type), points_per_event, f_out);
			}
		}	
		
		
		fclose(f_out);

	}

	cout << "all is ok" << endl;
	system("pause");

	
	return 0;
}