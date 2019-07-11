#include "ChMapping.h"

using namespace std;

ChMapping::ChMapping(std::vector<int> ch_list, std::vector<int> ch_list_for_view)
{
	for (int i = 0; i < ch_list_for_view.size(); i++)
	{
		for (int j = 0; j < ch_list.size(); j++)
		{
			if (ch_list_for_view[i] == ch_list[j])
			{
				ch_index_list.push_back(j);
			}
		}		
	}
}

std::vector<int> &ChMapping::GetChIndexList()
{
	return ch_index_list;
}


ChMapping::~ChMapping()
{
}
