#pragma once

#include <optional>
#include <filesystem>
#include "SFML/Graphics.hpp"
#include "scene.hpp"
#include "ui-component.hpp"
#include "aes/aes-utl.hpp"
#include "beatmap-directory.hpp"
#include "sfml-button.hpp"

namespace jk {
	class map_select_renderer : public renderer {
		enc::aes_utl encoder_;
		ui_mng components_;
		std::list<button> buttons_;
		
	public:
		void init();
		[[nodiscard]] SCENEFLAG operator() () override;
		std::uint32_t input(const sf::Event & e) noexcept override;
		void free_resource() noexcept override;

		[[nodiscard]] std::optional<jk::beatmap> get_selected() const;
	
	private:
		void AddButton(jk::beatmap_directory& bd);
		const sf::Text MakeMusicName(const jk::beatmap& b) const;
	};
}
