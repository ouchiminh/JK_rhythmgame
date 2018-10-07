#pragma once
#include <thread>
#include "../src/beatmap.hpp"
#include "test.hpp"

namespace jk::test {
	DEFINE_TEST(beatmap_load_test) {
		std::shared_ptr<sf::Music> p;
		jk::beatmap b{ ".\\beatmap\\template\\any_name.map", p};
		std::exception_ptr ep;
		
		std::thread t(&jk::beatmap::load, &b, ep);
		t.join();
		CHECK_TRUE(ep);
	}
}