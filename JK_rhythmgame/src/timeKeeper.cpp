#include <chrono>
#include <thread>
#include "timeKeeper.h"

using namespace std::chrono;


timeKeeper::timeKeeper(microseconds d) {
	m_duration = duration_cast<nanoseconds>(d);
}

timeKeeper::~timeKeeper() {}

void timeKeeper::start() {
	if(f_started) m_lastCalled = steady_clock::now();
	f_started = true;
}

void timeKeeper::finish() {
	f_started = false;
}

bool timeKeeper::isTime() {
	bool istime = remainingTime() <= nanoseconds(0);
	if (istime) m_lastCalled = steady_clock::now();
	return istime;
}

void timeKeeper::sleep() {
	auto remaining = remainingTime();
	if (remaining > nanoseconds(0)) std::this_thread::sleep_for(remaining);
	m_lastCalled = steady_clock::now();
}

std::chrono::steady_clock::duration timeKeeper::remainingTime() {
	auto difference = (steady_clock::now() - m_lastCalled);
	return nanoseconds(m_duration.count() - difference.count());
}

