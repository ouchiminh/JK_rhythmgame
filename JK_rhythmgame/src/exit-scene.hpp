#pragma once
#include <shared_mutex>
#include "SFML/Graphics.hpp"
#include "scene.hpp"
#include "ui-component.hpp"
#include "sfml-button.hpp"

namespace jk {
	class exit_scene : public scene {
		ui_mng ui_mng_;
		event_handlers<> handlers_;
		sf::Text verification_message_;
		std::shared_mutex mtx_;
		SCENEFLAG flag_;

		sf::RenderWindow * w_;

		std::int32_t on_mouse_click(const sf::Event & e, sf::Sprite &, sf::Text &, jk::button & b);
		std::int32_t on_mouse_click_yes(const sf::Event & e, sf::Sprite &, sf::Text &, jk::button & b);

		virtual void finish();
		void init_ui();

		std::uint32_t on_key_down(const sf::Event & e);
		
	public:
		exit_scene() noexcept : w_{ nullptr } {}
		virtual void init(HMODULE hm, sf::RenderWindow & w);
		virtual bool free_resource() noexcept;

		virtual SCENEFLAG render();
		virtual SCENE_LIST get_next_scene() const noexcept;
		virtual void input(const sf::Event & e) noexcept;
	};
}
