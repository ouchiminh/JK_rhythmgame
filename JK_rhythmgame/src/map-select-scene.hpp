#pragma once
#include <optional>
#include "scene.hpp"
#include "SFML/Graphics.hpp"
#include "arcfile.hpp"

namespace jk {
	class map_select_renderer : public renderer {
	public:
		SCENEFLAG operator() () override;
		std::uint32_t input(const sf::Event & e) noexcept override;
		void free_resource() noexcept override;

		jk::archive::file_view get_selected() const;
	};
}
