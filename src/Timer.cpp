#include "../include/Timer.h"

Timer::Timer() {
	
}

Timer::~Timer() {
	
}

void Timer::start() {
	startTime = std::chrono::steady_clock::now();
}

long long Timer::elapsedNano() {
	std::chrono::time_point<std::chrono::steady_clock> now = std::chrono::steady_clock::now();
	return static_cast<long long>(std::chrono::duration_cast<std::chrono::nanoseconds>(now - startTime).count());
}

long long Timer::elapsedMicro() {
	std::chrono::time_point<std::chrono::steady_clock> now = std::chrono::steady_clock::now();
	return static_cast<long long>(std::chrono::duration_cast<std::chrono::microseconds>(now - startTime).count());
}

long long Timer::elapsedMilli() {
	std::chrono::time_point<std::chrono::steady_clock> now = std::chrono::steady_clock::now();
	return static_cast<long long>(std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime).count());
}

double Timer::elapsedSec() {
	std::chrono::time_point<std::chrono::steady_clock> now = std::chrono::steady_clock::now();
	auto dur = std::chrono::duration_cast<std::chrono::microseconds>(now - startTime).count();
	return (static_cast<double>(dur) / 1000000.0);
}
