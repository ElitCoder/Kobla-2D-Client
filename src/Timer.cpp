#include "Timer.h"

using namespace std;

void Timer::start() {
	start_time_ = chrono::system_clock::now();
}

long long Timer::elapsed() {
	auto milliseconds = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now() - start_time_);
	
	return milliseconds.count();
}