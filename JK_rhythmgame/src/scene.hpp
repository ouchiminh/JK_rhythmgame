#pragma once
#include <cstdint>
#include "SFML/Window/Event.hpp"

namespace jk {
	enum SCENEFLAG{
		NOTYET, RUNNING, FINISHED
	};

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

	enum SCENE_LIST : std::intptr_t {
		Main_Menu
	};

	class scene {
		virtual void finish() = 0;
	public:
		virtual bool free_resource() noexcept = 0;

		virtual SCENEFLAG render() = 0;
		virtual std::intptr_t get_next_scene() const noexcept = 0;
		virtual void input(const sf::Event & e) noexcept = 0;
	};
}
