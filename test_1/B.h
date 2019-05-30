#pragma once

//c++
#include <vector>
#include <utility>      // std::pair, std::make_pair

//root cern
#include "TObject.h"


class B : public TObject
{
public:
	B();
	~B();

	std::vector<double> peak_amp;

	B(const B&); //copy constructor
	const B& operator=(const B&); //assignment
	int operator==(const B&) const; //equality


	ClassDef(B, 6);
};

