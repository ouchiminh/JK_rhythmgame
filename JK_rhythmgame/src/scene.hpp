#pragma once
#include <cstdint>
#include <memory>
#include "SFML/Window.hpp"
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <Windows.h>

namespace jk {
	enum SCENEFLAG{
		NOTYET, RUNNING, FINISHED
	};

	enum class SCENE_LIST : std::intptr_t {
		Main_Menu, Map_Select
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
