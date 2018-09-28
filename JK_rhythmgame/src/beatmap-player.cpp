#include <string>
#include <utility>
#include "boost/property_tree/json_parser.hpp"
#include "boost/foreach.hpp"
#include "beatmap-player.hpp"

jk::lane_key_map::lane_key_map() noexcept {
	set_default();
}

jk::lane_key_map::lane_key_map(std::filesystem::path && config_file, unsigned lane_cnt) { load_config(std::move(config_file), lane_cnt); }

void jk::lane_key_map::set_default(unsigned lane_cnt) noexcept {
	keymap_.clear();
	sf::Keyboard::Key candidate[] = {
		sf::Keyboard::Key::F,
		sf::Keyboard::Key::J,
		sf::Keyboard::Key::K,
		sf::Keyboard::Key::D,
		sf::Keyboard::Key::S,
		sf::Keyboard::Key::L
	};
	sf::Keyboard::Key center = sf::Keyboard::Key::Space;

	// レーン数が奇数の場合スペースキーを追加するため、lane_cntを超えない最大の偶数回で処理を止める。
	for (unsigned i = 0; i < lane_cnt - (lane_cnt & 1); i++) keymap_.emplace(candidate[i], i);
	if (lane_cnt % 2) keymap_.emplace(center, lane_cnt);
}

bool jk::lane_key_map::load_config(std::filesystem::path && config_file, unsigned lane_cnt) noexcept {
	boost::property_tree::ptree pt;
	keymap_.clear();
	try {
		boost::property_tree::read_json(config_file.generic_string(), pt);
		if (pt.get<int>("version") == version) throw std::runtime_error("this config file is not compatible with this version.");

		unsigned lane = 0;
		auto keys = pt.get<std::string>("Data." + std::to_string(lane_cnt));
		for (auto const & i : keys) {
			if (i == ' ') keymap_.emplace(sf::Keyboard::Key::Space, lane++);
			else if (i > 'A' || i < 'Z') keymap_.emplace(static_cast<sf::Keyboard::Key>(i - 'A'), lane++);
			throw std::out_of_range("keys must be A-Z or space");
		}
	} catch (...) {
		set_default(lane_cnt);
		return false;
	}
	return true;
}

std::optional<unsigned> jk::lane_key_map::get_lane(sf::Keyboard::Key key) const noexcept {
	if (keymap_.count(key)) return keymap_.at(key);
	return std::nullopt;
}

jk::beatmap_player::beatmap_player(beatmap && b) : b_{ std::move(b) } {}

jk::result_t jk::beatmap_player::event_procedure(const sf::Event & e) {
	return result_t();
}

void jk::beatmap_player::draw(sf::RenderTarget &, sf::RenderStates) const {}

sf::FloatRect jk::beatmap_player::get_rect() const noexcept {
	return sf::FloatRect();
}

