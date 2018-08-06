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
		void set_focus(bool b) noexcept { focused_ = b; }
	};

	class ui_mng {
		std::list<std::shared_ptr<ui_component>> ui_list_;
		
	public:
		std::uint32_t recieve_event(const sf::Event & e);
		void draw(sf::RenderWindow & rt, sf::RenderStates rs) const;
		template<class T,typename ...Arg>
		std::shared_ptr<T> create(Arg ...args);
		std::list<std::shared_ptr<ui_component>> get_list() const noexcept;
	};

	template<class T, typename ...Arg>
	inline std::shared_ptr<T> ui_mng::create(Arg ...args) {
		static_assert(std::is_base_of_v<ui_component, T>);
		auto r{ std::make_shared<T>(args...) };
		ui_list_.push_front(r);
		return r;
	}
}