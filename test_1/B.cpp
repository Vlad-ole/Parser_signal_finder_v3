#include "B.h"


B::B()
{
	peak_amp.clear();
}

B::B(const B& right)
{
	peak_amp = right.peak_amp;
}

const B& B::operator=(const B& right)
{
	peak_amp = right.peak_amp;
	return *this;
}

int B::operator==(const B& right) const
{
	return (peak_amp != right.peak_amp) ? 0 : 1;
}


B::~B()
{
}

ClassImp(B)