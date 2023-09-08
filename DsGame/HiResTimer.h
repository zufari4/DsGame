#pragma once

#include <inttypes.h>

class HiResTimer 
{
public:
	HiResTimer();
	double now() const;

protected:
	double frequency_;
};

