#include "Timer.h"
#include <chrono>
#include <ctime>


Timer::Timer()
{
}

Timer::Timer(unsigned int msBeforeTimeout) : msBeforeTimeout{ msBeforeTimeout }
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
	return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() > msBeforeTimeout;
}


void Timer::setTimeMax(unsigned int msBeforeTimeout)
{
	this->msBeforeTimeout = msBeforeTimeout;
}

void Timer::forceTimeOut()
{
	this->isForcedTimeOut = true;
}

