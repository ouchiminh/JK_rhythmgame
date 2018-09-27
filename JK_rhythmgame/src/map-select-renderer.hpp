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
		sf::RenderWindow* window_;
		ui_mng components_;
		// commented by ouchiminh
		// buttonはstd::enable_shared_from_thisを継承しているのでテンプレート引数変えました。
		std::vector<std::shared_ptr<button>> musicButtons_;						
		std::vector<std::filesystem::path> mapPaths_;		//musicButton_と同時に操作しなければならない
		std::vector<std::shared_ptr<button>>::iterator musicButtonsItr_;
				
	public:
		void init(sf::RenderWindow* window);
		[[nodiscard]] SCENEFLAG operator() () override;
		std::uint32_t input(const sf::Event & e) noexcept override;
		void free_resource() noexcept override;

		[[nodiscard]] std::optional<jk::beatmap> get_selected() const;
	
	private:
		void AddButton(jk::beatmap_directory& bd);
		const sf::Text MakeButtonName(const jk::beatmap& b) const;
		void InitButtonPos();
	};
}
