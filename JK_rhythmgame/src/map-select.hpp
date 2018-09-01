#pragma once
#include <filesystem>
#include "scene.hpp"
#include "ui-component.hpp"

namespace jk {
	class map_select : public scene {
		sf::RenderWindow * w_;
		std::filesystem::directory_iterator di_;
		ui_mng components_;

		bool did_initialized_;
	public:
		map_select() : did_initialized_{ false } {}
		void init(HMODULE hm, sf::RenderWindow & w) noexcept override;
		bool free_resource() noexcept override;
		
		SCENEFLAG render() override;
		SCENE_LIST get_next_scene() const noexcept override;
		void input(const sf::Event & e) noexcept override;
	};
}
