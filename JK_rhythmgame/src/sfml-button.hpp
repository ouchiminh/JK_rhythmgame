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
		event_handlers handlers_;

	public:
		button() = default;
		button(const sf::Text & t, const sf::Sprite bkg);
		button(const sf::Text & t, const sf::Color bkg = sf::Color::Black);

		void draw(sf::RenderTarget & rt, sf::RenderStates rs) const override;
		PROCESSED recieve_event(const sf::Event & e) noexcept override;
	private:
		bool is_processable(sf::Event & e) const noexcept;
	};
}
