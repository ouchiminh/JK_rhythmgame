#pragma once
#include "SFML\Window\Event.hpp"
namespace jk {
	enum SCENEFLAG{};

	class scene {
		virtual void finish() = 0;
	public:
		virtual bool free_resource() noexcept = 0;

		virtual SCENEFLAG render() = 0;
		virtual SCENEFLAG get_next_scene() const noexcept = 0;
		virtual void input(const sf::Event & e) noexcept = 0;
	};
}
