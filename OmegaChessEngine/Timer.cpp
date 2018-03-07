#include "Timer.h"
#include <chrono>
#include <ctime>


Timer::Timer()
{
}

Timer::Timer(unsigned int msBeforeTimeout) : msBeforeTimeout{ msBeforeTimeout }
{
	this->thread = std::thread{ [&]() 
	{
		start = std::chrono::system_clock::now();
		while (!timerDestruction)
		{
			end = std::chrono::system_clock::now();
			if (std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() > msBeforeTimeout)
			{
				_isTimeOut = true; 
				break;
			}

			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
	}
	};
}

bool Timer::isTimeOut()
{
	return this->_isTimeOut;
}

Timer::~Timer()
{
	timerDestruction = true;
	this->thread.join();
}

void Timer::reset()
{
	//Remove last thread;
	timerDestruction = true;
	this->thread.join();
	timerDestruction = false;

	//Begin another thread
	_isTimeOut = false;
	this->thread = std::thread{ [&]()
	{
		start = std::chrono::system_clock::now();
		while (!timerDestruction)
		{
			end = std::chrono::system_clock::now();
			if (std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() > msBeforeTimeout)
			{
				_isTimeOut = true;
				break;
			}

			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
	}
	};
}

void Timer::forceTimeOut()
{
	_isTimeOut = true;
	timerDestruction = true;
	this->thread.join();
}

void Timer::setTimeMax(unsigned int msBeforeTimeout)
{
	msBeforeTimeout = msBeforeTimeout;
}

