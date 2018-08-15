#pragma once
#include <functional>
#include <map>
#include "ui-component.hpp"
#include "event-handlers.hpp"

namespace jk {

	class button : public ui_component{
		sf::Sprite bkg_;
		sf::Text title_;
	public:
		event_handlers<sf::Sprite &, sf::Text &, jk::button &> handlers_;

	public:
		button() = default;
		button(const sf::Text & t, const sf::Sprite & bkg);

		void draw(sf::RenderTarget & rt, sf::RenderStates rs) const override;
		PROCESSED event_procedure(const sf::Event & e) override;
		sf::FloatRect get_rect() const noexcept override;
	};
}
