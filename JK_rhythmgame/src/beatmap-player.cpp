#include <string>
#include <utility>
#include <algorithm>
#include "boost/property_tree/json_parser.hpp"
#include "boost/foreach.hpp"
#include "SFML/Audio.hpp"
#include "beatmap-player.hpp"
#include "color-manager.hpp"
#include "scene.hpp"
#include "sfmlUtl.hpp"

namespace {
	[[nodiscard]] constexpr unsigned calc_lane_center_xcoord(unsigned lane, unsigned total_lane, unsigned component_res) {
		auto lane_width = component_res / static_cast<float>(total_lane);
		return static_cast<unsigned>(lane_width * lane + lane_width / 2.0);
	}
	[[nodiscard]] constexpr unsigned calc_lane_left_xcoord(unsigned lane, unsigned total_lane, unsigned component_res) {
		auto lane_width = component_res / static_cast<float>(total_lane);
		return static_cast<unsigned>(lane_width * lane);
	}
}


namespace {
	// these coord values is on DISPLAY. not on ui_component

	constexpr std::pair<double, double> GAGE_COORD = { 24 / 128.0, 2/72.0 };
	constexpr std::pair<double, double> GAGE_SIZE = { (32 + 16 * 3) / 128.0, 6.0 };

	constexpr std::pair<double, double> LANE_COORD = { 24 / 128.0, (GAGE_COORD.second + GAGE_SIZE.second + 3) / 72.0 };
	constexpr std::pair<double, double> LANE_SIZE = { 80 / 128.0,(51 + 7) / 72.0 };
	inline namespace bm_screen {
		constexpr auto LANE_HIT_LEVEL = (51 / 58.0f);
		constexpr auto NOTE_THICKNESS = 10;
	}

	constexpr std::pair<double, double> COMBO_DISP_COORD = { (LANE_COORD.first + LANE_SIZE.first + 4) / 128.0, 36 / 72.0 };
	constexpr std::pair<double, double> COMBO_DISP_SIZE = { 10 / 128.0, 8 / 72.0 };
}

namespace {
	constexpr auto NOTE_DISP_TIME = 0.8f;	// sec
	constexpr auto NOTE_SPEED = 1.0f / NOTE_DISP_TIME;	// 1.0を移動距離とした時の速さ(distance/sec)
}

jk::lane_key_map::lane_key_map(std::filesystem::path && config_file, unsigned lane_cnt) { load_config(std::move(config_file), lane_cnt); }

void jk::lane_key_map::set_default(unsigned lane_cnt) noexcept {
	keymap_.clear();
	sf::Keyboard::Key candidate[] = {
		sf::Keyboard::Key::S,
		sf::Keyboard::Key::D,
		sf::Keyboard::Key::F,
		sf::Keyboard::Key::J,
		sf::Keyboard::Key::K,
		sf::Keyboard::Key::L,
	};
	sf::Keyboard::Key center = sf::Keyboard::Key::Space;

	// レーン数が奇数の場合スペースキーを追加するため、lane_cntを超えない最大の偶数回で処理を止める。
	for (unsigned i = 0; i < lane_cnt - (lane_cnt & 1); i++) keymap_.emplace(candidate[i], i);
	if (lane_cnt & 1) keymap_.emplace(center, lane_cnt);
}

jk::lane_key_map::iterator jk::lane_key_map::begin() {
	return keymap_.begin();
}

jk::lane_key_map::iterator jk::lane_key_map::end() {
	return keymap_.end();
}

jk::lane_key_map::const_iterator jk::lane_key_map::cbegin() const {
	return keymap_.cbegin();
}

jk::lane_key_map::const_iterator jk::lane_key_map::cend() const {
	return keymap_.cend();
}

bool jk::lane_key_map::load_config(std::filesystem::path && config_file, unsigned lane_cnt) noexcept {
	boost::property_tree::ptree pt;
	keymap_.clear();
	try {
		boost::property_tree::read_json(config_file.string(), pt);
		if (pt.get<int>("version") != version) throw std::runtime_error("this config file is not compatible with this version.");

		unsigned lane = 0;
		auto keys = pt.get<std::string>("Data." + std::to_string(lane_cnt));
		for (auto const & i : keys) {
			if (i == ' ') keymap_.emplace(sf::Keyboard::Key::Space, lane++);
			else if (i >= 'A' && i <= 'Z') keymap_.emplace(static_cast<sf::Keyboard::Key>(i - 'A'), lane++);
			else throw std::out_of_range("keys must be A-Z or space");
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

std::optional<sf::Keyboard::Key> jk::lane_key_map::get_key(unsigned lane) const noexcept {
	auto result = std::find_if(std::begin(keymap_), std::end(keymap_), [lane](auto val) { return val.second == lane; });
	if (result == std::end(keymap_)) return std::nullopt;
	return result->first;
}

jk::beatmap_player::beatmap_player(jk::beatmap & b, sf::Vector2u const & resolution) :
	b_{ std::move(b) }, notes_visible_duration_{ sf::seconds(1.0f) }
{
	b_.load();
	b_.get_music().lock()->setPlayingOffset(sf::microseconds(0));
	lkm_.load_config(".\\setting\\keycfg.json", b_.get_lane_cnt());
	screen_.create(static_cast<unsigned>(resolution.x * LANE_SIZE.first), static_cast<unsigned>(resolution.y * LANE_SIZE.second));
	spr_.setTexture(screen_.getTexture());
	jk::adjust_pos(spr_, resolution, jk::ADJUSTFLAG::CENTER);
	
	for (auto i = 0u; i < b_.get_lane_cnt(); i++) {
		hit_lines_.emplace_back(sf::Vector2f{ static_cast<float>(screen_.getSize().x / b_.get_lane_cnt()), static_cast<float>(NOTE_THICKNESS) });
		hit_lines_.back().setPosition(sf::Vector2f
									  {
										  static_cast<float>(calc_lane_left_xcoord(i, b_.get_lane_cnt(), screen_.getSize().x)),
										  static_cast<float>(LANE_HIT_LEVEL * screen_.getSize().y)
									  });
	}
	for (const auto & i : lkm_) key_state_.emplace(std::make_pair(i.first, utl::continuum_state<bool>(false)));
}

void jk::beatmap_player::draw_notes() {
	auto lane_cnt = b_.get_lane_cnt();
	sf::VertexArray notes;
	auto hit_level = screen_.getSize().y * bm_screen::LANE_HIT_LEVEL;
	notes.setPrimitiveType(sf::PrimitiveType::Quads);
	for (auto i = 0u; i < b_.get_lane_cnt(); i++) {
		auto itr = b_.get_current_note_itr(i);
		for (; itr != b_.end(i); itr++) {
			auto y = hit_level - itr->get_time_diff().asSeconds() * NOTE_SPEED * screen_.getSize().y;
			if (y < -NOTE_THICKNESS) break;
			// 画面外(下方向)にはみ出たノーツをこれ以降処理しない
			if (y >= screen_.getSize().y) { b_.forward_note(i); continue; }
			auto const & hit_line = hit_lines_.at(i);
			notes.append(sf::Vertex(sf::Vector2f{ hit_line.getPosition().x, y }));
			notes.append(sf::Vertex(sf::Vector2f{ hit_line.getPosition().x + hit_line.getGlobalBounds().width, y }));
			notes.append(sf::Vertex(sf::Vector2f{ hit_line.getPosition().x + hit_line.getGlobalBounds().width, y + NOTE_THICKNESS }));
			notes.append(sf::Vertex(sf::Vector2f{ hit_line.getPosition().x, y + NOTE_THICKNESS }));
		}
	}
	screen_.draw(notes);
}

void jk::beatmap_player::lightup_lane() {
	// get pushed key
	for (auto const & i : lkm_) {
		if (auto is_key_hit = sf::Keyboard::isKeyPressed(i.first); key_state_.at(i.first) = is_key_hit) {
			// レーン光らせる
			hit_lines_[i.second].setFillColor(jk::color::color_mng::get("Data.lane_color." + std::to_string(i.second)).value_or(jk::color::theme_color));

			// notes_に問い合わせ
			auto score = b_.get_current_note_itr(i.second) != b_.end(i.second) && key_state_.at(i.first).is_not_continuum(true) ?
				b_.get_current_note(i.second).hit():
				std::optional<float>(std::nullopt);
			sum_ += score;
			if (score)
				b_.forward_note(i.second);
		} else {
			hit_lines_[i.second].setFillColor(jk::color::color_mng::get("Data.str_color").value_or(jk::color::str_color));
		}
	}
	for (auto const & i : hit_lines_) screen_.draw(i);
}

void jk::beatmap_player::update() {
	if (auto m = b_.get_music().lock()) m->getPlayingOffset();
	screen_.clear(jk::color::color_mng::get("Data.lane_color.surface").value_or(jk::color::bkg_color));
	lightup_lane();
	draw_notes();
	screen_.display();
	spr_.setTexture(screen_.getTexture());
}

void jk::beatmap_player::draw(sf::RenderTarget & rt, sf::RenderStates rs) const {
	rs.transform *= getTransform();
	rt.draw(spr_, rs);
}

sf::FloatRect jk::beatmap_player::get_rect() const noexcept { return spr_.getGlobalBounds(); }

float jk::beatmap_player::get_score() const noexcept {
	return sum_.avg<float>();
}

std::weak_ptr<sf::Music> jk::beatmap_player::get_music() const noexcept {
	return b_.get_music();
}

bool jk::beatmap_player::is_end() const noexcept {
	bool f_end = false;
	for (auto i = 0u; i < b_.get_lane_cnt() && !f_end; i++) f_end = (b_.cend(i) == b_.get_current_note_itr(i));
	return f_end;
}
