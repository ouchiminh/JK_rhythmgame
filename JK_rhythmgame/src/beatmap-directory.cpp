#include <optional>
#include "boost/property_tree/ptree.hpp"
#include "boost/foreach.hpp"
#include "boost/property_tree/json_parser.hpp"
#include "beatmap-directory.hpp"

jk::beatmap_directory::beatmap_directory(std::filesystem::path const & path) noexcept(false) {
	set_directory(path);
}

jk::beatmap_directory::beatmap_directory(std::filesystem::path && path) noexcept(false) {
	set_directory(path);
}

void jk::beatmap_directory::set_directory(std::filesystem::path && path) noexcept(false) {
	set_directory(path);
}

void jk::beatmap_directory::set_directory(std::filesystem::path const & path) noexcept(false) {
	using namespace boost::property_tree;
	ptree pt;
	auto m = std::make_shared<sf::Music>();
	auto cfg_file = path / "config.json";
	if (!std::filesystem::exists(cfg_file))
		throw std::filesystem::filesystem_error(
			"The following file could not be browsed.",
			cfg_file,
			std::make_error_code(std::errc::no_such_file_or_directory));

	read_json(cfg_file.string(), pt);
	beatmap_list_.clear();
	// read music file name.
	if (auto music_name = pt.get_optional<std::string>("Data.music.name"))
		m->openFromFile(music_name.value());

	BOOST_FOREACH(const ptree::value_type & child, pt.get_child("Data.map_list")) {
		const auto & info = child.second;
		beatmap_list_.emplace_back(path / info.data(), m);
	}
}

void jk::beatmap_directory::clear() noexcept { beatmap_list_.clear(); }

jk::beatmap_directory::iterator jk::beatmap_directory::begin() {
	return beatmap_list_.begin();
}

jk::beatmap_directory::iterator jk::beatmap_directory::end() {
	return beatmap_list_.end();
}

jk::beatmap_directory::const_iterator jk::beatmap_directory::cbegin() const {
	return beatmap_list_.cbegin();
}

jk::beatmap_directory::const_iterator jk::beatmap_directory::cend() const {
	return beatmap_list_.cend();
}

jk::beatmap jk::beatmap_directory::GetFirstBeatmapList() const {
	return *(beatmap_list_.begin());
}
