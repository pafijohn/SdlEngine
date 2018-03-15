#pragma once

#include <sys/time.h>
#include <cstdint>
#include <chrono>

class Timer
{
	bool valid;
	timeval startTime;
	uint32_t timeDiff;
public:
	Timer();
	
	void Start( uint32_t msTimeout );
	double GetDiff();
	bool IsExpired();
	
	void Invalidate();
	bool IsValid();
};