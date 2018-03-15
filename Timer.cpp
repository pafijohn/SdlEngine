#include "Timer.h"

Timer::Timer()
{
	this->startTime = { 0, 0 };
	this->timeDiff = 0;
	this->Invalidate();
}

void Timer::Start( uint32_t msTimeout )
{
	gettimeofday( &this->startTime, nullptr );
	this->timeDiff = msTimeout;
	this->valid = true;
}

double Timer::GetDiff()
{
	double elapsed;
	
	if ( this->valid )
	{
		timeval now;
		gettimeofday( &now, nullptr );
		
		elapsed = ( now.tv_sec - this->startTime.tv_sec ) * 1000.0;      // sec to ms
		elapsed += ( now.tv_usec - this->startTime.tv_usec ) / 1000.0;   // us to ms
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