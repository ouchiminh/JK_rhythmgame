#pragma once
#include <filesystem>
#include <optional>
#include "SFML/System/Time.hpp"
#include "SFML/Window/Keyboard.hpp"
#include "ui-component.hpp"
#include "event-handlers.hpp"
#include "beatmap.hpp"

namespace jk {

	class lane_key_map {
		//		<keycode,			lane	>
		std::map<sf::Keyboard::Key, unsigned> keymap_;

		// DO NOT FORGET UPDATE VERSION WHEN MODIFY THIS CLASS
		static inline constexpr int version = 0;

	public:
		lane_key_map() = default;
		lane_key_map(std::filesystem::path && config_file, unsigned lane_cnt);
		void set_default(unsigned lane_cnt) noexcept;

		// if failed, set default and returns false
		bool load_config(std::filesystem::path && config_file, unsigned lane_cnt) noexcept;
		[[nodiscard]] std::optional<unsigned> get_lane(sf::Keyboard::Key key) const noexcept;
	};
	
	class beatmap_player : public ui_component {
		beatmap b_;
		sf::Time notes_visible_duration_;
		event_handlers<beatmap &> handlers_;

	private:
		beatmap_player(beatmap && b);
	public:
		result_t event_procedure(const sf::Event & e) override;
		void draw(sf::RenderTarget &, sf::RenderStates) const override;
		sf::FloatRect get_rect() const noexcept override;

	};
}
