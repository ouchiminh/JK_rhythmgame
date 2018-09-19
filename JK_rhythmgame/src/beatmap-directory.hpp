#pragma once
#include <list>
#include <filesystem>
#include "beatmap.hpp"

namespace jk {
	class beatmap_directory {
		std::list<beatmap> beatmap_list_;
	public:
		using iterator = std::list<beatmap>::iterator;
		using const_iterator = std::list<beatmap>::const_iterator;

		beatmap_directory() = default;
		beatmap_directory(std::filesystem::path const & path) noexcept(false);
		beatmap_directory(std::filesystem::path && path) noexcept(false);
		void set_directory(std::filesystem::path && path) noexcept(false);

		/// <summary>
		/// scan concrete beatmap directory ./beatmap/[beatmap name]
		/// </summary>
		/// <param name="path">path to the concrete beatmap directory. this directory should include "config.json"</param>
		/// <returns></returns>
		void set_directory(std::filesystem::path const & path) noexcept(false);

		void clear() noexcept;

		iterator begin();
		iterator end();
		const_iterator cbegin() const;
		const_iterator cend() const;
	};
}
