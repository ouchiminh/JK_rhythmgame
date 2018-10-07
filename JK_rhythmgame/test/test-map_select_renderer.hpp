#pragma once
#include "test.hpp"
#include "../src/map-select-renderer.hpp"
#include "SFML/Graphics.hpp"

namespace jk::test {
	DEFINE_TEST(map_select_renderer_test) {
		sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "test");
		jk::map_select_renderer msr;

		msr.init(&window);

		sf::Event event;
		while (window.isOpen()) {
			while (window.pollEvent(event)) {
				if (event.type == sf::Event::Closed)
					window.close();
				else msr.input(event);
			}
			if (msr() == SCENEFLAG::FINISHED) return;
		}
	}
}