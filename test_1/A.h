#pragma once


//c++
#include <vector>
#include <utility>      // std::pair, std::make_pair

//root cern
#include "TObject.h"

//my
#include "B.h"

class A : public TObject
{
public:
	A();
	~A();

	void Clear();

	std::vector<B*> peaks;
	std::vector<double> id;

	ClassDef(A, 5);
};

