#pragma once
#include <string>
#include <filesystem>
#include <atomic>
#include <shared_mutex>
#include "SFML/Graphics/Color.hpp"
#include "boost/property_tree/ptree.hpp"

namespace jk {
	inline namespace color {
		 const inline sf::Color bkg_color	= sf::Color(0);
		 const inline sf::Color str_color	= sf::Color(238, 238, 238);
		 const inline sf::Color theme_color	= sf::Color(0xFF, 0xA0, 0x00);
	}

	inline namespace color {
		// all method is thread safe. maybe...
		class color_mng {
			static inline constexpr int version = 0;

			inline static std::shared_mutex mtx;
			inline static std::filesystem::path cfg_file_path;
			static boost::property_tree::ptree cfg_tree;
		public:
			static void set(std::filesystem::path && cfg_file);
			static void set(std::filesystem::path const & cfg_file);

			static void reload();

			[[nodiscard]] static std::optional<sf::Color> get(std::string const & key);
			[[nodiscard]] static std::optional<sf::Color> get(std::string && key);
		};

		[[nodiscard]] sf::Color convert(std::string_view color_str);
	}
}
