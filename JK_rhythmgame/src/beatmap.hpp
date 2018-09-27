#pragma once
#include <filesystem>
#include <string>
#include <memory>
#include <deque>
#include <vector>
#include "SFML/Audio.hpp"
#include "notes.hpp"

namespace jk {

	constexpr unsigned MAX_LANE_CNT = 7;

	class beatmap {
		std::shared_ptr<sf::Music> music_;
		std::filesystem::path map_location_;

		std::vector<std::deque<note>> notes_;

		void fill_data(std::string & line);
	public:
		beatmap() noexcept = default;
		beatmap(std::filesystem::path const & path, std::shared_ptr<sf::Music> m);
		beatmap(std::filesystem::path && path, std::shared_ptr<sf::Music> m);
		beatmap(beatmap &&) = default;
		beatmap(beatmap const &) = default;

		void set(std::filesystem::path const & path, std::shared_ptr<sf::Music> m);
		void set(std::filesystem::path && path, std::shared_ptr<sf::Music> m);
		
		/// <summary>
		/// <para>this method locks beatmap file in order to load contents safely.</para>
		/// this function is thread safe, and declared as noexcept func because std::thread terminate process when throw.
		/// </summary>
		/// <param name="ep">pointer to the exception.</param>
		void load(std::exception_ptr & ep) noexcept;
		void free() noexcept;

		unsigned get_lane_cnt() const noexcept;
	public:
		std::weak_ptr<sf::Music> get_music() const noexcept;
		std::filesystem::path const & get_path() const noexcept;

		const std::vector<std::deque<note>> & get_notes() const noexcept;

		bool operator!() const noexcept;
	};
}
