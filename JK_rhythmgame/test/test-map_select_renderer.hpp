#pragma once
#include "test.hpp"
#include "../src/map-select-renderer.hpp"
#include "SFML/Graphics.hpp"

namespace jk::test {
	DEFINE_TEST(map_select_renderer_test) {
		sf::RenderWindow rw(sf::VideoMode::getDesktopMode(), "test");
		jk::map_select_renderer msr;

	}
}