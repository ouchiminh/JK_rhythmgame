#pragma once
#include "test.hpp"
#include "../src/beatmap-player.hpp"

namespace jk::test {
	DEFINE_TEST(lane_key_map_load_test) {
		jk::lane_key_map lkm;
		CHECK_TRUE(lkm.load_config("keycfg.json", 4));

		CHECK_TRUE(!lkm.get_lane(sf::Keyboard::Key::Space));
		CHECK_TRUE(lkm.get_lane(sf::Keyboard::Key::G));

		// set default
		CHECK_TRUE(!lkm.load_config("keycfg-v5000.json", 4));

		CHECK_TRUE(!lkm.get_lane(sf::Keyboard::Key::G));
		CHECK_TRUE(lkm.get_lane(sf::Keyboard::Key::K));
	}
}
