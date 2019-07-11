#include "ReadInfo.h"

using namespace std;

ReadInfo::ReadInfo(std::string file_name_info) : file_name_info(file_name_info)
{
	file_input.open(file_name_info, std::ifstream::in);
	if (file_input.is_open())
	{
		cout << "File has been opened correctly: " << file_name_info.c_str() << endl;
	}
	else
	{
		cout << "Can't open this file: " << file_name_info.c_str() << endl;
		exit(1);
	}
}

void ReadInfo::Read()
{
	std::string line;

	while (std::getline(file_input, line))
	{
		if (line[0] != '#')
		{
			std::istringstream iss(line);
			int ch;
			bool is_positive_polarity_type;
			double th;

			iss >> ch >> is_positive_polarity_type >> th;
			ch_list.push_back(ch);
			is_positive_polarity_type_list.push_back(is_positive_polarity_type);
			th_list.push_back(th);
		}		
	}
}

void ReadInfo::Show()
{
	cout << "ch_list.size() = " << ch_list.size() << endl;
	cout << "is_positive_polarity_type_list.size() = " << is_positive_polarity_type_list.size() << endl;
	cout << "th_list.size() = " << th_list.size() << endl;
	
	cout << "i \t ch \t is_positive_polarity_type \t th" << endl;
	for (int i = 0; i < ch_list.size(); i++)
	{
		cout << i << "\t" << ch_list[i] << "\t" << is_positive_polarity_type_list[i] << "\t" << th_list[i] << endl;
	}
}

std::vector<int> &ReadInfo::GetChList()
{
	return ch_list;
}

std::vector<bool> &ReadInfo::GetIsPositivePolarityTypeList()
{
	return is_positive_polarity_type_list;
}

std::vector<double> &ReadInfo::GetThList()
{
	return th_list;
}

ReadInfo::~ReadInfo()
{
	file_input.close();
}
