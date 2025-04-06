#pragma once

#include <cstdint>
#include <chrono>

typedef struct timeval {
    long tv_sec;
    long tv_usec;
} timeval;

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