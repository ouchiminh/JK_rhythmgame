#pragma once
#include "test.hpp"
#include "../src/beatmap-player.hpp"
#include "../src/ui-component.hpp"
#include "SFML/Graphics.hpp"

namespace jk::test {
	DEFINE_TEST(beatmap_player_test) {
		sf::RenderWindow window(sf::VideoMode(1280,720), "test", sf::Style::Titlebar | sf::Style::Close);
		sf::Event event;
		std::exception_ptr ep;
		jk::ui_mng mng;
		auto p = std::make_shared<sf::Music>();
		p->openFromFile("beatmap\\harujion\\harujon.ogg");
		jk::beatmap b("beatmap\\harujion\\harujion.map", p);
		b.load(ep);
		auto player = mng.create<jk::beatmap_player>(b, sf::Vector2i(1280, 720));

		while (window.isOpen()) {
			while (window.pollEvent(event)) {
				if (event.type == sf::Event::Closed)
					window.close();
				else mng.event_procedure(event);
			}
			window.clear();
			player->update();
			mng.draw(window);
			window.display();
		}
	}
}
