#pragma once
#include <cstdint>
#include <memory>
#include "SFML/Window.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <Windows.h>

namespace jk {
	inline namespace color {
		const sf::Color bkg_color	= sf::Color(0);
		const sf::Color str_color	= sf::Color(238, 238, 238);
		const sf::Color theme_color	= sf::Color(0xFF, 0xA0, 0x00);
	}

	enum SCENEFLAG{
		NOTYET, RUNNING, FINISHED
	};

	enum SCENE_LIST : std::intptr_t {
		Null = 0, Exit, Main_Menu, Game_Play 
	};

	class renderer {
	public:
		virtual SCENEFLAG operator()() = 0;
		virtual std::uint32_t input(const sf::Event & e) { return 0; }
		virtual void free_resource() noexcept = 0;
	};

	class scene : public virtual std::enable_shared_from_this<scene> {
		virtual void finish() = 0;
	protected:
		scene() = default;
	public:
		virtual void init(HMODULE hm, sf::RenderWindow & w) = 0;
		virtual bool free_resource() noexcept = 0;
		virtual ~scene() = default;

		virtual SCENEFLAG render() = 0;
		virtual SCENE_LIST get_next_scene() const noexcept = 0;
		virtual void input(const sf::Event & e) noexcept = 0;
	};
}

namespace IKD {
	template <typename T>
	class MethodExec {
	public:
		MethodExec(MethodExec<T>(T::*f)()) : func_(f) {};
		MethodExec<T> exec(T* obj) {
			return (obj->*func_)();
		}
	private:
		MethodExec<T>(T::*func_)(); // 実行するT型クラスのメソッドポインタ
	};
}
