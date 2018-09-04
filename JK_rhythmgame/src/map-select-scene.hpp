#pragma once
#include <optional>
#include <filesystem>
#include "SFML/Graphics.hpp"
#include "scene.hpp"
#include "ui-component.hpp"
#include "aes/aes-utl.hpp"
#include "archiver.hpp"
#include "arcfile.hpp"

namespace jk {
	class map_select_renderer : public renderer {
		std::filesystem::directory_iterator di_;
		jk::archive::archiver archiver_;
		enc::aes_utl encoder_;
		ui_mng components_;
		
	public:
		[[nodiscard]] SCENEFLAG operator() () override;
		std::uint32_t input(const sf::Event & e) noexcept override;
		void free_resource() noexcept override;

		[[nodiscard]] jk::archive::file_view get_selected() const;
	};
}
