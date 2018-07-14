#pragma once
#define NOMINMAX
#include <Windows.h>
#include "SFML/Window/Event.hpp"
#include "SFML/Graphics.hpp"
#include "scene.hpp"

namespace jk {
	class mainmenu : public scene {
		sf::Image logo_[2];
		sf::Sprite logoSpr_[2];
		sf::Texture tx_[2];
		sf::RenderWindow * w_;
		void(*cur_renderer)();

		void render_logo();
		void render_manu();
	protected:
		void finish() override;
	public:
		void init(HMODULE hm, sf::RenderWindow & w);
		virtual bool free_resource() noexcept override;

		virtual SCENEFLAG render() override;
		virtual SCENEFLAG get_next_scene() const noexcept override;
		virtual void input(const sf::Event & e) noexcept override;
	};
}
