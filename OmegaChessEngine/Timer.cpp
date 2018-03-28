#include "Timer.h"
#include <chrono>
#include <ctime>


Timer::Timer()
{
}

Timer::Timer(uint64_t msBeforeTimeout) : msBeforeTimeout{ msBeforeTimeout }
{
}

void Timer::reset()
{
	start = std::chrono::system_clock::now();
}


bool Timer::isTimeOut()
{
	if (this->isForcedTimeOut == true) return true;
	end = std::chrono::system_clock::now();
	uint64_t msPassed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
	return  msPassed > msBeforeTimeout;
}


void Timer::setTimeMax(uint64_t msBeforeTimeout)
{
	this->msBeforeTimeout = msBeforeTimeout;
}

void Timer::forceTimeOut()
{
	this->isForcedTimeOut = true;
}

