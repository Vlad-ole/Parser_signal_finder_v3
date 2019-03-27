#pragma once

#include "TObject.h"

#include <vector>

using namespace std;


class EventMainCh : public TObject
{
public:
	EventMainCh();
	~EventMainCh();

	double min_y;
	double max_y;


	ClassDef(EventMainCh, 2);
};

