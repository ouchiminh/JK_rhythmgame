#pragma once
#include <string>
#include <filesystem>
#include <atomic>
#include <shared_mutex>
#include "SFML/Graphics/Color.hpp"
#include "boost/property_tree/ptree.hpp"

namespace jk {
	inline namespace color {
		// all method is thread safe. maybe...
		class color_mng {
			static inline constexpr int version = 0;

			inline static std::shared_mutex mtx;
			inline static std::filesystem::path cfg_file_path;
			inline static boost::property_tree::ptree cfg_tree;
		public:
			static void set(std::filesystem::path && cfg_file);
			static void set(std::filesystem::path const & cfg_file);

			static void reload();

			static std::optional<sf::Color> get(std::string const & key);
			static std::optional<sf::Color> get(std::string && key);
		};

		sf::Color convert(std::string_view color_str);
	}
}
