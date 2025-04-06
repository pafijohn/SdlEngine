#include "Timer.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <stdint.h>


int gettimeofday(struct timeval * tp, struct timezone * tzp)
{
    // Note: some broken versions only have 8 trailing zero's, the correct epoch has 9 trailing zero's
    // This magic number is the number of 100 nanosecond intervals since January 1, 1601 (UTC)
    // until 00:00:00 January 1, 1970 
    static const uint64_t EPOCH = ((uint64_t) 116444736000000000ULL);

    SYSTEMTIME  system_time;
    FILETIME    file_time;
    uint64_t    time;

    GetSystemTime( &system_time );
    SystemTimeToFileTime( &system_time, &file_time );
    time =  ((uint64_t)file_time.dwLowDateTime )      ;
    time += ((uint64_t)file_time.dwHighDateTime) << 32;

    tp->tv_sec  = (long) ((time - EPOCH) / 10000000L);
    tp->tv_usec = (long) (system_time.wMilliseconds * 1000);
    return 0;
}

Timer::Timer()
{
	this->startTime = {0, 0};
	this->timeDiff = 0;
	this->Invalidate();
}

void Timer::Start(uint32_t msTimeout)
{
	gettimeofday(&this->startTime, nullptr);
	this->timeDiff = msTimeout;
	this->valid = true;
}

double Timer::GetDiff()
{
	double elapsed;
	
	if (this->valid)
	{
		timeval now;
		gettimeofday(&now, nullptr);
		
		elapsed = (now.tv_sec - this->startTime.tv_sec) * 1000.0;      // sec to ms
		elapsed += (now.tv_usec - this->startTime.tv_usec) / 1000.0;   // us to ms
	}
	else
	{
		elapsed = 0;
	}
	
	return elapsed;
}

bool Timer::IsExpired()
{
	return this->GetDiff() >= this->timeDiff;
}

void Timer::Invalidate()
{
	this->valid = false;
}

bool Timer::IsValid()
{
	return this->valid;
}