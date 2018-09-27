#include <utility>
#include "beatmap-player.hpp"

jk::lane_key_map::lane_key_map() noexcept {
	set_default();
}

void jk::lane_key_map::set_default() noexcept {
	sf::Keyboard::Key key_array[] = {
			sf::Keyboard::Key::F,
			sf::Keyboard::Key::G,
			sf::Keyboard::Key::H,
			sf::Keyboard::Key::J
	};
	unsigned lane_array[] = { 0,1,2,3 };

	for (int i = 0; i < sizeof(lane_array) / sizeof(*lane_array); i++) {
		keymap_.emplace(key_array[i], lane_array[i]);
	}
}

void jk::lane_key_map::load_config(std::filesystem::path config_file) {

}

std::optional<unsigned> jk::lane_key_map::get_lane(sf::Keyboard::Key key) const noexcept {
	if (keymap_.count(key)) return keymap_.at(key);
	return std::nullopt;
}

jk::beatmap_player::beatmap_player(beatmap && b) : b_{ std::move(b) } {}

