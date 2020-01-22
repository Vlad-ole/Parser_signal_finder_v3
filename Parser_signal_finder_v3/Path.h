#pragma once

#include <iostream>
#include <string>

class Path
{
public:
	Path();
	~Path();

	std::string GetFileNameInfo();
	std::string GetFileNameDAQInfo();
	std::string GetPathRawInput();
	std::string GetPathRawModOutput();
	std::string GetFirstPartOfPath();
	std::string GetSubFolderName();

private:
	std::string file_name_info;
	std::string file_name_daq_info;
	std::string path_raw_input;
	std::string path_raw_mod_output;
	std::string first_part_of_path;
	std::string subfolder_name;
};

