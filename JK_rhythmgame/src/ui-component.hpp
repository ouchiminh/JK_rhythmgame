#pragma once
#include <memory>
#include <list>
#include <cstdint>
#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"

namespace jk {
	enum fall_through : std::uint16_t {
		NO_FALLTHROUGH, FALLTHROUGH
	};

	union PROCESSED {
		std::uint64_t all;
		std::uint32_t each[2];	// [0]:fall_through		[1]:unique retvalue
		std::uint16_t mini[4];	// [0]:fall_through		[1]:did process?(true,false)	[2],[3]:unique retvalue
		void processed(bool f = true) noexcept { mini[1] = f; }
		void fallthrought(fall_through f = NO_FALLTHROUGH) noexcept { mini[0] = f; }
		void set_ret(uint32_t v = 0) noexcept { each[1] = v; }

		uint32_t get_retv() const noexcept { return each[1]; }
		fall_through get_fall_through() const noexcept { return static_cast<fall_through>(mini[0]); }
		bool get_is_processed() const noexcept { return mini[1] ? true : false; }
	};

	class ui_component : public sf::Drawable, public std::enable_shared_from_this<ui_component> {
	protected:
		bool focused_;
	protected:
		ui_component() : focused_{ false } {}
		virtual ~ui_component() = default;

	public:
		virtual void draw(sf::RenderTarget &, sf::RenderStates) const override = 0;
		virtual PROCESSED recieve_event(const sf::Event & e) noexcept { return PROCESSED(); }
		bool is_focused() const noexcept { return focused_; }
		void set_focus(bool b) noexcept { focused_ = b; }
		virtual sf::IntRect get_rect() const noexcept = 0;
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
		auto r{ std::make_shared<T, Arg...>(args...) };
		ui_list_.push_front(r);
		return r;
	}
}
