#include <chrono>
#include <thread>
#include "timeKeeper.h"

using namespace std::chrono;

timeKeeper::timeKeeper(microseconds d) {
	m_duration = duration_cast<nanoseconds>(d);
}

void timeKeeper::start() const {
	m_lastCalled = steady_clock::now();
}


bool timeKeeper::isTime() const {
	const bool istime = remainingTime().count() <= 0;
	if (istime) m_lastCalled = steady_clock::now();
	return istime;
}

void timeKeeper::sleep() const {
	auto remaining = remainingTime();
	if (remaining > nanoseconds(0)) std::this_thread::sleep_for(remaining);
	m_lastCalled = steady_clock::now();
}

std::chrono::steady_clock::duration timeKeeper::remainingTime() const {
	auto difference = (steady_clock::now() - m_lastCalled);
	return nanoseconds(m_duration.count() - difference.count());
}

