#pragma once
#include <unordered_map>
#include <functional>
#include "SFML/Graphics/Font.hpp"

namespace jk {

extern std::unordered_map<std::string, sf::Font> font_dictionary;

template<class SFML_Resource>
inline void load_from_file(std::string && path) { }

template<>
inline void load_from_file<sf::Font>(std::string && path){
	if (font_dictionary.count(path)) return;
	sf::Font f;
	f.loadFromFile(path);
	font_dictionary.insert_or_assign(path, f);
}

}
