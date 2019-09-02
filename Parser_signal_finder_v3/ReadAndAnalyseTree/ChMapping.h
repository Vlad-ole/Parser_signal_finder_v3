#pragma once

//c++
#include <vector>
#include <utility>      // std::pair, std::make_pair

class ChMapping
{
public:
	ChMapping(std::vector<int> ch_list, std::vector<int> ch_list_for_view);
	~ChMapping();

	std::vector<int> &GetChIndexList();
	std::vector<int> &GetChOrderedNumberList();
private:
	std::vector<int> ch_index_list;
	std::vector<int> ch_ordered_number_list;
};

