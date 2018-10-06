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

		using iterator = std::map<sf::Keyboard::Key, unsigned>::iterator;
		using const_iterator = std::map<sf::Keyboard::Key, unsigned>::const_iterator;
		iterator begin();
		iterator end();

		const_iterator cbegin() const;
		const_iterator cend() const;
		// if failed, set default and returns false
		bool load_config(std::filesystem::path && config_file, unsigned lane_cnt) noexcept;
		[[nodiscard]] std::optional<unsigned> get_lane(sf::Keyboard::Key key) const noexcept;
		[[nodiscard]] std::optional<sf::Keyboard::Key> get_key(unsigned lane) const noexcept;
	};
	
	class beatmap_player : public ui_component, public sf::Transformable {
		beatmap b_;
		lane_key_map lkm_;
		sf::Time notes_visible_duration_;

		sf::RenderTexture screen_;
		sf::Sprite spr_;

	private:
		beatmap_player(beatmap && b, sf::Vector2i resolution);

		void draw_notes();
		void draw_gage();
		void lightup_lane();
	public:
		// this class does not proceed event.
		// result_t event_procedure(const sf::Event & e) override;

		void update();
		void draw(sf::RenderTarget &, sf::RenderStates = sf::RenderStates::Default) const override;
		[[deprecated]] sf::FloatRect get_rect() const noexcept override;
	};
}