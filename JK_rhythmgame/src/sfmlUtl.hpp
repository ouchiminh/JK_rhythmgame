#pragma once
#include <type_traits>
#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"

namespace jk {
	enum ADJUSTFLAG {
		NOCHANGE, LEFT = 1, RIGHT = 1 << 1, HCENTER = 1 << 2,
		VCENTER = 1 << 3, TOP = 1 << 4, BTM = 1 << 5, CENTER = VCENTER | HCENTER
	};	
	constexpr ADJUSTFLAG operator | (ADJUSTFLAG a, ADJUSTFLAG b) {
		return static_cast<ADJUSTFLAG>(static_cast<int>(a) | static_cast<int>(b));
	}
	
	template<class T, typename CoordElm>
	inline void adjust_pos(T & t, const sf::Vector2<CoordElm> & screen, ADJUSTFLAG af, float offsetx = 0.0f, float offsety = 0.0f) {
		static_assert(std::is_base_of_v<sf::Transformable, T>, "class T must be derived from sf::Transformable.");
		auto mvdpos = t.getPosition();
		const auto boundsbox = t.getGlobalBounds();
		switch (af & 0b111) {
		case ADJUSTFLAG::LEFT:
			mvdpos.x = 0;
			break;
		case ADJUSTFLAG::RIGHT:
			mvdpos.x = screen.x - boundsbox.width;
			break;
		case ADJUSTFLAG::HCENTER:
			mvdpos.x = (screen.x - boundsbox.width) / 2;
			break;
		}
		mvdpos.x += offsetx;
		switch (af & 0b111000) {
		case ADJUSTFLAG::TOP:
			mvdpos.y = 0;
			break;
		case ADJUSTFLAG::BTM:
			mvdpos.y = screen.y - boundsbox.height;
			break;
		case ADJUSTFLAG::VCENTER:
			mvdpos.y = (screen.y - boundsbox.height) / 2;
			break;
		}
		mvdpos.y += offsety;
		t.setPosition(mvdpos);
	}
	template<class T>
	inline void adjust_pos(T & t, const sf::RenderTarget & rt, ADJUSTFLAG af, float offsetx = 0.0f, float offsety = 0.0f) {
		adjust_pos(t, rt.getSize(), af, offsetx, offsety);
	}

	template<typename T, typename T2>
	constexpr T gradual_change(T begin, T end, T2 duration, T2 current) {
		auto min_width = (end - begin) / duration;
		if (current >= duration) return end;
		else if (current <= static_cast<T2>(0)) return begin;
		return static_cast<T>(begin + min_width * current);
	}
	template<typename T>
	inline sf::Vector2<T> get_center_pos(const sf::Rect<T> & r) {
		return sf::Vector2<T>{r.left + r.width / 2, r.top + r.height / 2};
	}
	template<typename T>
	inline sf::Vector2<T> shift_ceter_pos(const sf::Rect<T> & target, const sf::Vector2<T> & dest) {
		auto buf = get_center_pos(target);
		return sf::Vector2<T>{target.left + dest.x - buf.x, target.top + dest.y - buf.y};
	}
	template<class T, typename Type>
	inline void set_center_pos(T & target, const sf::Vector2<Type> & dest) {
		auto new_pos{ shift_ceter_pos<Type>(target.getGlobalBounds(), dest) };
		target.setPosition(new_pos);
	}

	template<class Type>
	[[nodiscard]] sf::Vector2<Type> normalize_coord(sf::Vector2<Type> point, sf::Vector2<Type> size) {
		return (point * static_cast<Type>(2) - size) / std::min(size.x, size.y);
	}
}
