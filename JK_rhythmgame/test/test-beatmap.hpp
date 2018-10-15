#pragma once
#include <thread>
#include "../src/beatmap.hpp"
#include "test.hpp"

namespace jk::test {
	DEFINE_TEST(beatmap_load_fail_test) {
		std::shared_ptr<sf::Music> p;
		jk::beatmap b{ ".\\beatmap\\template\\any_name.map", p};
		std::exception_ptr ep;
		
		std::thread t([&b, &ep]() {b.load(ep); });
		t.join();
		CHECK_TRUE(ep);
	}

	DEFINE_TEST(beatmap_load_test) {
		std::shared_ptr<sf::Music> p;
		jk::beatmap b{ ".\\beatmap\\harujion\\harujion.map", p};
		std::exception_ptr ep;
		
		std::thread t([&b, &ep]() {b.load(ep); });
		t.join();
		CHECK_TRUE(ep == false);
		if (ep) {
			try { std::rethrow_exception(ep); }
			catch (std::exception & e) { LOG(e.what()); }
		}
	}
}
