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

	public:
		lane_key_map() noexcept;
		lane_key_map(std::filesystem::path config_file);
		void set_default() noexcept;
		void load_config(std::filesystem::path config_file);
		[[nodiscard]] std::optional<unsigned> get_lane(sf::Keyboard::Key key) const noexcept;
		void change_or_add_key(sf::Keyboard::Key key, unsigned lane);
		void add_key(sf::Keyboard::Key key, unsigned lane);
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
