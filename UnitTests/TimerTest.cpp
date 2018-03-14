#include "stdafx.h"
#include "CppUnitTest.h"
#include "../OmegaChessEngine/Timer.h"
#include "../OmegaChessEngine/Timer.cpp"
#include <thread>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{
	TEST_CLASS(TimerTest)
	{
	public:

		TEST_METHOD(TimerValidity)
		{
			Timer timer;
			const unsigned int timeToTest[] = { 500, 1000, 2000 };
			for (const unsigned int& timerTime : timeToTest)
			{
				timer.setTimeMax(timerTime);
				std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
				timer.reset();

				while (true)
				{
					//Do some work ...
					std::this_thread::sleep_for(std::chrono::milliseconds(10));
					auto end = std::chrono::system_clock::now();
					int timePassed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
					if (timePassed > (timerTime))
					{
						Assert::IsTrue(timer.isTimeOut());
						break;
					}
					else
					{
						Assert::IsFalse(timer.isTimeOut());

					}

				}
			}
			
		}

		TEST_METHOD(ForcedTimeout)
		{
			Timer timer{ 2000 };
			timer.reset();
			std::thread thread{ [&]() {
				std::this_thread::sleep_for(std::chrono::milliseconds(1000));
				timer.forceTimeOut();
			} };

			std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
			while (true)
			{
				//Do some work ...
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
				if (timer.isTimeOut()) break;
			}

			//The timer should fail before 2 sec because the thread force it to stop
			thread.join();
			std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
			Assert::IsTrue(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() < 1050);
		}

		
	};
}