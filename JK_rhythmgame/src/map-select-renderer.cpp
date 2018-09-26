#include "map-select-renderer.hpp"
#include "sfml-button.hpp"
#include <iostream>

void jk::map_select_renderer::init() {
	namespace fs = std::filesystem;

	for (const auto &p : fs::directory_iterator(".\\beatmap\\"))	{
		jk:beatmap_directory bd;
		try {
			bd.set_directory(p);				//beatmap’¼‰º‚ÌƒtƒHƒ‹ƒ_‚ğ‘–¸‚µ‚Äbd‚ÉSet‚·‚é
		}catch (fs::filesystem_error & e) {
			std::cout << e.what() << std::endl;
		}
		AddButton(bd);
	}
}

void jk::map_select_renderer::AddButton(jk::beatmap_directory& bd) {
	for (const auto& b : bd) {
		buttons_.emplace_back(MakeMusicName(b));
	}
}

const sf::Text jk::map_select_renderer::MakeMusicName(const jk::beatmap& b) const {
	sf::Text name;
	const std::string sname = b.get_path();
	name.setString(sname);

	return name;
}

