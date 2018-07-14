#pragma once
#include <cstdint>
#include "SFML\Window\Event.hpp"

namespace jk {
	enum SCENEFLAG{
		NOTYET, RUNNING, FINISHED
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
