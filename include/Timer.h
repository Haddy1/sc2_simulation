#pragma once

#include <chrono>

class Timer {
public:
	Timer();
	~Timer();
	void start();
	void stop();
	long long elapsedNano();
	long long elapsedMicro();
	long long elapsedMilli();
	double elapsedSec();
private:
	std::chrono::time_point<std::chrono::steady_clock> startTime;
};