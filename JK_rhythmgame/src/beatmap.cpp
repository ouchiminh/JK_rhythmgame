#include "beatmap.hpp"

jk::beatmap::beatmap(std::filesystem::path const & path, std::shared_ptr<sf::Music> m) { set(path, m); }

jk::beatmap::beatmap(std::filesystem::path && path, std::shared_ptr<sf::Music> m) { set(path, m); }

void jk::beatmap::set(std::filesystem::path const & path, std::shared_ptr<sf::Music> m) {
	music_ = m;
	map_location_ = path;
}

void jk::beatmap::set(std::filesystem::path && path, std::shared_ptr<sf::Music> m) { set(path, m); }

std::weak_ptr<sf::Music> jk::beatmap::get_music() const noexcept {
	return music_;
}

std::filesystem::path const & jk::beatmap::get_path() const noexcept {
	return map_location_;
}
