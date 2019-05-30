#include "A.h"


A::A()
{
}

void A::Clear()
{
	for (size_t i = 0; i<peaks.size(); i++)
		delete peaks.at(i);
	peaks.clear();

	id.clear();
}


A::~A()
{
}

ClassImp(A)