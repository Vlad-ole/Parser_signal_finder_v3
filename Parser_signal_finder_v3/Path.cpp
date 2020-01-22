#include "Path.h"

using namespace std;

Path::Path()
{
	string date = "181025";
	string year = "20" + date.substr(0, 2);
	first_part_of_path = "E:\\" + year + "\\" + date + "\\" + date;
	subfolder_name = "f5";

	file_name_info = first_part_of_path + "_caen_raw\\info\\" + subfolder_name + "_info.txt";
	file_name_daq_info = first_part_of_path + "_caen_raw\\info\\daq_info.txt";
	path_raw_input = first_part_of_path + "_caen_raw\\" + subfolder_name + "\\";
	path_raw_mod_output = first_part_of_path + "_caen_raw\\" + subfolder_name + "_mod\\";
}

Path::~Path()
{
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