#pragma once
#include "scene.hpp"
#include "map-select-renderer.hpp"
#include "play-renderer.hpp"
#include "SFML/Graphics.hpp"

namespace jk {
	
	class game_scene : public scene {
		sf::RenderWindow * rw_;
		map_select_renderer map_select_;
		beatmap_play_renderer beatmap_play_;


		SCENE_LIST next_scene_;
		SCENEFLAG status_;
		renderer * cur_renderer_;

		void change_renderer();
		void finish() override;
	public:
		void init(HMODULE hm, sf::RenderWindow & w) override;
		bool free_resource() noexcept override;

		SCENEFLAG render() override;
		SCENE_LIST get_next_scene() const noexcept override;
		void input(const sf::Event & e) noexcept override;
	};
}
