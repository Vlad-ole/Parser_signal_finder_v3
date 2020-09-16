#include "Path.h"

using namespace std;

Path::Path()
{
	string date = "200910";
	subfolder_name = "f29";
	number_of_input_files = 210;

	string year = "20" + date.substr(0, 2);
	first_part_of_path = "E:\\" + year + "\\" + date + "\\" + date;	

	file_name_info = first_part_of_path + "_caen_raw\\info\\" + subfolder_name + "_info.txt";
	file_name_daq_info = first_part_of_path + "_caen_raw\\info\\daq_info.txt";
	path_raw_input = first_part_of_path + "_caen_raw\\" + subfolder_name + "\\";
	path_raw_mod_output = first_part_of_path + "_caen_raw\\" + subfolder_name + "_mod\\";
}

Path::~Path()
{
}

int Path::GetNumberOfInputFiles()
{
	return number_of_input_files;
}

string Path::GetFileNameInfo()
{
	return file_name_info;
}

string Path::GetFileNameDAQInfo()
{
	return file_name_daq_info;
}

string Path::GetPathRawInput()
{
	return path_raw_input;
}

string Path::GetPathRawModOutput()
{
	return path_raw_mod_output;
}

string Path::GetFirstPartOfPath()
{
	return first_part_of_path;
}

std::string Path::GetSubFolderName()
{
	return subfolder_name;
}