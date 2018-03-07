#ifndef Timer_H
#define Timer_H 

#include <thread>

class Timer
{
public:
	Timer();
	Timer(unsigned int msBeforeTimeout);
	bool isTimeOut();
	void forceTimeOut();
	void setTimeMax(unsigned int msBeforeTimeout);

	~Timer();
	void reset();

private:
	unsigned int msBeforeTimeout;
	std::chrono::time_point<std::chrono::system_clock> start, end;
	bool timerDestruction = false;
	std::thread thread;
	bool _isTimeOut = false;

};

#endif

