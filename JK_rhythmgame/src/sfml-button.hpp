#pragma once
#include <functional>
#include <map>
#include <string_view>
#include "ui-component.hpp"
#include "event-handlers.hpp"

namespace jk {

	class button : public ui_component{
		friend ui_mng;
	protected:
		sf::Texture texture_;
		sf::Sprite bkg_;
		sf::Text title_;
	public:
		event_handlers<sf::Sprite &, sf::Text &, jk::button &> handlers_;

	protected:
		button() = default;
		button(const sf::Text & t, const sf::Texture & bkg);
		button(const sf::Text & t, std::string_view bkg_filename);
		button(const sf::Text & t);
	public:
		void draw(sf::RenderTarget & rt, sf::RenderStates rs = sf::RenderStates::Default) const override;
		result_t event_procedure(const sf::Event & e) override;
		sf::FloatRect get_rect() const noexcept override;

		void set_text(const sf::Text & t);
		void set_bkg(const sf::Texture & t);
	};
}
