#include "map-select-renderer.hpp"
#include "sfml-button.hpp"
#include <iostream>

void jk::map_select_renderer::init(sf::RenderWindow* window) {
	namespace fs = std::filesystem;
	
	window_ = window;
	for (const auto &p : fs::directory_iterator(".\\beatmap\\"))	{
		jk:beatmap_directory bd;
		try {
			bd.set_directory(p);				
		}catch (fs::filesystem_error & e) {
			std::cout << e.what() << std::endl;
		}
		AddButton(bd);
	}
	
}

void jk::map_select_renderer::AddButton(jk::beatmap_directory& bd) {
	for (const auto& b : bd) {
		musicButtons_.emplace_back(MakeButtonName(b));
		musicPaths_.emplace_back(b.get_path());
	}
}

const sf::Text jk::map_select_renderer::MakeButtonName(const jk::beatmap& b) const {
	sf::Text name;
	auto sname = b.get_path().stem().generic_string();
	name.setString(sname);

	return name;
}

void jk::map_select_renderer::InitButtonPos() {
	sf::Vector2f pivotRe = { 0.578, 0.216};		//相対位置(ボタン左上をピボットポイントと想定)
	auto size = window_->getSize();
	for (auto button : this->musicButtons_) {
		button.set_position({ size.x * pivotRe.x, size.y * pivotRe.y});
		pivotRe += {3.0, 0.0};		//計算してない
	}
}

