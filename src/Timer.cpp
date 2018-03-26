#include "Timer.h"

using namespace std;

void Timer::start() {
	start_time_ = chrono::system_clock::now();
}

bool Timer::elapsed(const chrono::time_point<std::chrono::system_clock>& time) {
	return (time - start_time_).count() > 0;
}