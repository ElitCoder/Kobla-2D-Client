#ifndef TIMER_H
#define TIMER_H

#include <chrono>

class Timer {
public:
	void start();
	long long elapsed();
	
private:
	std::chrono::time_point<std::chrono::system_clock> start_time_;
};

#endif