#include <chrono>
#include <thread>
#include "timeKeeper.h"

using namespace std::chrono;

timeKeeper::timeKeeper(microseconds d) {
	m_duration = duration_cast<nanoseconds>(d);
}

void timeKeeper::start() {
	m_lastCalled = steady_clock::now();
}


bool timeKeeper::isTime() const {
	return remainingTime().count() <= 0;
}

void timeKeeper::sleep() {
	auto remaining = remainingTime();
	if (remaining > nanoseconds(0)) std::this_thread::sleep_for(remaining);
	m_lastCalled = steady_clock::now();
}

std::chrono::steady_clock::duration timeKeeper::remainingTime() const {
	auto difference = (steady_clock::now() - m_lastCalled);
	return nanoseconds(m_duration.count() - difference.count());
}

