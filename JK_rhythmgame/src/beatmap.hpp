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
		std::vector<std::deque<note>::iterator> notes_itr_;

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
		/// This method is assumed to work with multithreading, but you should not access this class in the meantime.
		/// </summary>
		/// <param name="ep">pointer to the exception.</param>
		void load(std::exception_ptr & ep) noexcept;
		void free() noexcept;

		unsigned get_lane_cnt() const noexcept;

		std::weak_ptr<sf::Music> get_music() const noexcept;
		std::filesystem::path const & get_path() const noexcept;

		[[deprecated]][[nodiscard]] const std::vector<std::deque<note>> & get_notes() const noexcept;
		[[nodiscard]] note & get_current_note(unsigned lane);
		void forward_note(unsigned lane);

		bool operator!() const noexcept;
	};
}
