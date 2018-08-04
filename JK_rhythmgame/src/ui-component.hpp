#pragma once
#include <memory>
#include <list>
#include <cstdint>
#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"

namespace jk {
	enum fall_through : std::uint16_t {
		NO_FALLTHROUGH = 1, FALLTHROUGH
	};

	union PROCESSED {
		std::uint64_t all;
		std::uint32_t each[2];	// [0]:fall_through		[1]:unique retvalue
		std::uint16_t mini[4];	// [0]:fall_through		[2]:did process?(true,false)	[3],[4]:unique retvalue
	};

	class ui_component : public sf::Drawable {
	protected:
		bool focused_;
	public:
		ui_component() : focused_{ false } {}
		virtual ~ui_component() = default;
		virtual void draw(sf::RenderTarget &, sf::RenderStates) const override = 0;
		virtual PROCESSED recieve_event(const sf::Event & e) noexcept {}
		bool is_focused() const noexcept { return focused_; }
		void focus(bool b) noexcept { focused_ = b; }
	};

	class ui_mng {
		std::list<std::unique_ptr<ui_component>> ui_list;
		
	public:
		std::uint32_t recieve_event(const sf::Event & e);
		void draw(sf::RenderTarget & rt, sf::RenderStates rs) const;
	};
}