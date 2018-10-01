#include "color-manager.hpp"
#include "boost/property_tree/json_parser.hpp"

void jk::color::color_mng::set(std::filesystem::path && cfg_file) { set(cfg_file); }

void jk::color::color_mng::set(std::filesystem::path const & cfg_file) {
	static std::mutex localmtx;	// cfg_file_path guard
	std::lock_guard<decltype(localmtx)> lg(localmtx);
	cfg_file_path = cfg_file;
	reload();
}

void jk::color::color_mng::reload() {
	using namespace boost::property_tree;
	std::lock_guard<decltype(mtx)> lg(mtx);
	read_json(cfg_file_path.generic_string(), cfg_tree);

	// verify version
	auto value = cfg_tree.get_optional<int>("version");
	if (!value.has_value() || value.value() != version) {
		cfg_tree.clear();
		throw std::runtime_error("this file is not compatible with this version");
	}
}

std::optional<sf::Color> jk::color::color_mng::get(std::string const & key) {
	std::shared_lock<decltype(mtx)> lg(mtx);

	auto value = cfg_tree.get_optional<std::string>(key);
	if (!value) return std::nullopt;

	return jk::color::convert(value.get());
}

std::optional<sf::Color> jk::color::color_mng::get(std::string && key) {
	return get(key);
}

sf::Color jk::color::convert(std::string_view color_str) {
	auto code = std::stoi(color_str.data(), nullptr, 16);
	return sf::Color((code & 0xff0000) >> 16, (code & 0xff00) >> 8, (code & 0xff));
}
