#ifndef Timer_H
#define Timer_H 

#include <thread>
#include <cinttypes>

class Timer
{
public:
	Timer();
	Timer(uint64_t msBeforeTimeout);
	bool isTimeOut();
	void setTimeMax(uint64_t msBeforeTimeout);
	void forceTimeOut();

	void reset();

private:
	uint64_t msBeforeTimeout;
	std::chrono::time_point<std::chrono::system_clock> start, end;
	bool isForcedTimeOut = false;
};

#endif

