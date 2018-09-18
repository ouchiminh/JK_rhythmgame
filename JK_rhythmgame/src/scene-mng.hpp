#pragma once
#include <stack>
#include <map>
#include "SFML/Window/Event.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "scene.hpp"

namespace jk {

	class scene_mng {
		std::map<SCENE_LIST, std::shared_ptr<scene>> scene_list_;
		std::stack<SCENE_LIST> cur_scene_;		// should be stack;

		const HMODULE hm_;
		sf::RenderWindow & rw_;

	public:
		scene_mng(HMODULE hm, sf::RenderWindow & w);
		void init();
		void input(const sf::Event & e) noexcept;
		void render();
		const std::weak_ptr<scene> get(SCENE_LIST key) const noexcept;
	};
}
