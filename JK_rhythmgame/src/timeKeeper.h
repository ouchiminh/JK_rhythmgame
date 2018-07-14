#pragma once
#include <chrono>
class timeKeeper {
	std::chrono::steady_clock::time_point m_lastCalled;
	std::chrono::steady_clock::duration m_duration;

	std::chrono::steady_clock::duration remainingTime();
public:
	timeKeeper(std::chrono::microseconds d = std::chrono::microseconds(1'000'000) / 60);
	~timeKeeper();

	bool isTime();
	void sleep();
	template<typename Clock>
	void setDuration(std::chrono::duration<Clock> d);
};

template<typename Clock>
void timeKeeper::setDuration(std::chrono::duration<Clock> d) {
	m_duration = d;
}
