#pragma once
#include <filesystem>
#include <memory>
#include <SFML/Audio.hpp>

namespace jk {

	class beatmap {
		std::shared_ptr<sf::Music> music_;
		std::filesystem::path map_location_;
	public:
		beatmap() noexcept = default;
		beatmap(std::filesystem::path const & path, std::shared_ptr<sf::Music> m);
		beatmap(std::filesystem::path && path, std::shared_ptr<sf::Music> m);
		void set(std::filesystem::path const & path, std::shared_ptr<sf::Music> m);
		void set(std::filesystem::path && path, std::shared_ptr<sf::Music> m);
		
		void load() noexcept(false);
		void free() noexcept;

		std::weak_ptr<sf::Music> get_music() const noexcept;
		std::filesystem::path const & get_path() const noexcept;
	};
}
