#pragma once
#include "scene.hpp"
#include "beatmap-player.hpp"

namespace jk {
	class beatmap_play_renderer : public renderer {
		sf::RenderWindow * rw_;
		
		ui_mng mng_;
		std::shared_ptr<beatmap_player> bp_;
	public:
		SCENEFLAG operator()() override;
		virtual std::uint32_t input(const sf::Event & e);
		void free_resource() noexcept override;
		void init(beatmap && b, sf::RenderWindow & rw);
	};
}
