#ifndef TIMER_H
#define TIMER_H

#include <chrono>

class Timer {
public:
	void start();
	bool elapsed(const std::chrono::time_point<std::chrono::system_clock>& time);
	
private:
	std::chrono::time_point<std::chrono::system_clock> start_time_;
};

#endif