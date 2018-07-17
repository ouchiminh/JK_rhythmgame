#pragma once
#include <chrono>
class timeKeeper {
	mutable std::chrono::steady_clock::time_point m_lastCalled;
	std::chrono::steady_clock::duration m_duration;

	std::chrono::steady_clock::duration remainingTime() const;
public:
	timeKeeper(std::chrono::microseconds d = std::chrono::microseconds(1'000'000) / 60);
	~timeKeeper() = default;

	void start() const;
	bool isTime() const;
	void sleep() const;
	template<typename Clock, typename Period>
	void setDuration(std::chrono::duration<Clock, Period> d);
};

template<typename Clock, typename Period>
void timeKeeper::setDuration(std::chrono::duration<Clock, Period> d) {
	m_duration = std::chrono::duration_cast<decltype(m_duration)>(d);
}
