#pragma once
#include <chrono>
class timeKeeper {
	std::chrono::steady_clock::time_point m_lastCalled;
	std::chrono::steady_clock::duration m_duration;

	std::chrono::steady_clock::duration remainingTime();
	bool f_started;
public:
	timeKeeper(std::chrono::microseconds d = std::chrono::microseconds(1'000'000) / 60);
	~timeKeeper();

	void start();
	void finish();
	bool isTime();
	void sleep();
	template<typename Clock, typename Period>
	void setDuration(std::chrono::duration<Clock, Period> d);
};

template<typename Clock, typename Period>
void timeKeeper::setDuration(std::chrono::duration<Clock, Period> d) {
	m_duration = std::chrono::duration_cast<decltype(m_duration)>(d);
}
