﻿#pragma once
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
		/// <para>This method locks beatmap file in order to load contents safely.</para>
		/// This method is assumed to work with multithreading, but you should not access this class in the meantime.
		/// </summary>
		/// <param name="ep">pointer to the exception.</param>
		void load(std::exception_ptr & ep) noexcept;
		std::exception_ptr load() noexcept;
		void free() noexcept;

		[[nodiscard]] unsigned get_lane_cnt() const noexcept;

		[[nodiscard]] std::weak_ptr<sf::Music> get_music() const noexcept;
		[[nodiscard]] std::filesystem::path const & get_path() const noexcept;

		[[deprecated]] [[nodiscard]] const std::vector<std::deque<note>> & get_notes() const noexcept;
		[[deprecated]] [[nodiscard]] std::vector<std::deque<note>> & get_notes() noexcept;

		[[nodiscard]] const std::deque<note> & get_notes(unsigned lane) const;
		[[nodiscard]] std::deque<note> & get_notes(unsigned lane);

		[[nodiscard]] note & get_current_note(unsigned lane);
		[[nodiscard]] auto get_current_note_itr(unsigned lane) noexcept ->std::deque<note>::iterator;
		[[nodiscard]] auto get_current_note_itr(unsigned lane) const noexcept -> const std::deque<note>::iterator;
		[[nodiscard]] auto end(unsigned lane)->std::deque<note>::iterator;
		[[nodiscard]] auto cend(unsigned lane) const-> std::deque<note>::const_iterator;
		void forward_note(unsigned lane);

		[[nodiscard]] bool operator!() const noexcept;
	};
}
