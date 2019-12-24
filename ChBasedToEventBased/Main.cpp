//c++
#include <iostream>
#include <sstream>
#include <vector>
#include <iomanip>
#include <fstream>
#include <omp.h> 

//my
#include "ReadData_CAEN_v2.h"
#include "ReadDAQInfo.h"
#include "ReadInfo.h"

using namespace std;

int main(int argc, char **argv)
{
	bool is_binary = true;
	string date = "191219";
	string year = "20" + date.substr(0, 2);
	string subfolder_name = "f1";
	string file_name_info = "E:\\" + year + "\\" + date + "\\" + date + "_caen_raw\\info\\" + subfolder_name + "_info.txt";
	string file_name_daq_info = "E:\\" + year + "\\" + date + "\\" + date + "_caen_raw\\info\\daq_info.txt";
	ReadDAQInfo rd_daq_inf(file_name_daq_info);
	rd_daq_inf.Read();
	string data_file_ending = is_binary ? ".dat" :".txt";
	
	//Read DAQ_info.txt
	const unsigned short ns_per_point = /*4*//*16*/ rd_daq_inf.GetNsPerPoint();
	const unsigned int points_per_event = /*40000*//*9999*/ rd_daq_inf.GetPointsPerEventPerCh();
	const unsigned short N_events_per_file_input = /*1000*/ rd_daq_inf.GetNEventsPerFileInput();
	const unsigned short N_events_per_file_output = /*50*/ rd_daq_inf.GetNEventsPerFileOutput();
	//vector<unsigned short> ch_list= {0,1,2,3,4,5,6,7,32,33,34,35,36,37,38,39,40,41,42,43,44,48,49,50,51,52,53,54,55,56,57,58,59};
	//vector<unsigned short> ch_list = {0, 1, 2, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59};
	//vector<unsigned short> ch_list = { 1, 2, 3, 4};
	//vector<unsigned short> ch_list = {0, 1, 2, 3, 4, 5, 8, 9, 10, 11, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59};
	ReadInfo rd_inf(file_name_info);
	rd_inf.Read();
	vector<int> ch_list = rd_inf.GetChList();

	rd_daq_inf.Show();
	rd_inf.Show();
	system("pause");

	unsigned int start_run_number = 1;
	unsigned int stop_run_number = 439;

	string common_path_input = "E:\\" + year + "\\" + date + "\\" + date + "_caen_raw\\" + subfolder_name + "\\";
	string common_path_out = "E:\\" + year + "\\" + date + "\\" + date + "_caen_raw\\" + subfolder_name + "_mod\\";

	
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
			
			data_ch_event_point[ch].resize(N_events_per_file_input);
			
			
			ostringstream file_name_raw_in_osst;
			file_name_raw_in_osst << common_path_input << "run_" << run_number << "__ch_" << ch_list[ch] << data_file_ending;
			string file_name_raw_in = file_name_raw_in_osst.str();

			ReadData_CAEN_v2 rdt(file_name_raw_in, N_events_per_file_input, points_per_event);
			rdt.Read(is_binary);

			std::vector< std::vector<short int> > data_ev_point = rdt.GetDataEvPoint();

			
			
			for (int ev = 0; ev < N_events_per_file_input; ev++)
			{
				data_ch_event_point[ch][ev].resize(points_per_event);
				
				for (int point = 0; point < points_per_event; point++)
				{
					data_ch_event_point[ch][ev][point] = data_ev_point[ev][point];
				}
			}
			
			
			//data_ch_event[ch][]
		}

		
				
		
		
		/*int event_from = 0;
		int event_to = N_events_per_file_output;
		ostringstream file_name_raw_out_osst;
		file_name_raw_out_osst << common_path_out << "file_" << file_counter << ".dat";
		string file_name_raw_out = file_name_raw_out_osst.str();
		FILE *f_out = fopen(file_name_raw_out.c_str(), "wb");*/


		//unsigned int file_size = N_events_per_file * ch_list.size() * points_per_event;
		//unsigned int file_block_size = ch_list.size() * points_per_event;
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

		//v3
		/*int event_from = 0;
		int event_to = N_events_per_file_output;
		ostringstream file_name_raw_out_osst;
		file_name_raw_out_osst << common_path_out << "file_" << file_counter << ".dat";
		string file_name_raw_out = file_name_raw_out_osst.str();
		FILE *f_out = fopen(file_name_raw_out.c_str(), "wb");

		for (int ev = 0; ev < N_events_per_file; ev++)
		{
			if (ev % 100 == 0)
				cout << "\t \t ev_" << ev << endl;
			for (int ch = 0; ch < ch_list.size(); ch++)
			{
				fwrite(&data_ch_event_point[ch][ev][0], sizeof(vector<short int>::value_type), points_per_event, f_out);
			}
		}		
		fclose(f_out);*/

		//v4
		cout << "\t Write file" << endl;		
		FILE *f_out = NULL; 

		for (int ev = 0; ev < N_events_per_file_input; ev++)
		{
			if (ev % N_events_per_file_output == 0)
			{
				cout << "\t \t ev_" << ev << endl;
				int event_from = ev + file_counter * N_events_per_file_input;
				int event_to = ev + N_events_per_file_output - 1 + file_counter * N_events_per_file_input;
				ostringstream file_name_raw_out_osst;
				file_name_raw_out_osst << common_path_out << std::setfill('0') << std::setw(6) << event_from << "__" << std::setfill('0') << std::setw(6) << event_to << ".dat";
				string file_name_raw_out = file_name_raw_out_osst.str();
				f_out = fopen(file_name_raw_out.c_str(), "wb");

				if (f_out == NULL)
				{
					cout << "can't open this file: " << file_name_raw_out.c_str() << endl;
					system("pause");
					//fclose(f);
					exit(1);
				}

			}
			for (int ch = 0; ch < ch_list.size(); ch++)
			{
				if (f_out == NULL)
				{
					cout << "can't open the file" << endl;
					system("pause");
					exit(1);
				}
				fwrite(&data_ch_event_point[ch][ev][0], sizeof(vector<short int>::value_type), points_per_event, f_out);
			}
			if ( (ev + 1) % N_events_per_file_output == 0)
			{
				fclose(f_out);
			}			
		}
		


		file_counter++;
	}

	cout << "all is ok" << endl;
	system("pause");

	
	return 0;
}