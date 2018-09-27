#include "map-select-renderer.hpp"
#include "sfml-button.hpp"
#include <iostream>

void jk::map_select_renderer::init(sf::RenderWindow* window) {
	namespace fs = std::filesystem;
	
	window_ = window;
	for (const auto &p : fs::directory_iterator(".\\beatmap\\"))	{
		jk::beatmap_directory bd;
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
		// commented by ouchiminh
		// jk::ui_componentから派生するクラスはjk::ui_mng::createで生成されなければならない。
		// 実はui_mng内でshared_ptrを保持しているため、別で配列を作る必要はないが、ほかのコードの修正が少なく済むように既に定義してある変数は消していない。
		musicButtons_.push_back(components_.create<jk::button>(MakeButtonName(b)));
		mapPaths_.emplace_back(b.get_path());
	}
}

const sf::Text jk::map_select_renderer::MakeButtonName(const jk::beatmap& b) const {
	sf::Text name;
	auto sname = b.get_path().stem().generic_string();
	name.setString(sname);

	return name;
}

void jk::map_select_renderer::InitButtonPos() {
	sf::Vector2f pivotRe = { 0.578f, 0.216f};		//相対位置(ボタン左上をピボットポイントと想定)
	auto size = window_->getSize();
	for (auto button : this->musicButtons_) {
		// commented by ouchiminh
		// エラーをなくすためにmusicButtons_のテンプレート引数を変更したため、このコードも変更した。
		button->set_position({ size.x * pivotRe.x, size.y * pivotRe.y});
		pivotRe += {0.f, 0.040f};	
	}
}

std::optional<jk::beatmap> jk::map_select_renderer::get_selected() const {
	auto index = musicButtonsItr_  - musicButtons_.begin();		//今イテレータが指している場所のインデックスを取得
	auto selectPath = mapPaths_[index];
	beatmap_directory dir(selectPath);

	return dir.GetFirstBeatmapList();
}

jk::SCENEFLAG jk::map_select_renderer::operator() () {
	components_.draw(*window_);
}