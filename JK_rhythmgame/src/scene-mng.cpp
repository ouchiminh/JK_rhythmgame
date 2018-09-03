#include "scene-mng.hpp"
#include "menu_scene.hpp"
jk::scene_mng::scene_mng(HMODULE hm, sf::RenderWindow & w) : hm_{ hm }, rw_{ w } 
{}

void jk::scene_mng::init() {
	scene_list_.insert_or_assign(SCENE_LIST::Main_Menu, std::make_shared<mainmenu>());
	cur_scene_ = SCENE_LIST::Main_Menu;
	scene_list_[SCENE_LIST::Main_Menu]->init(hm_, rw_);
}

void jk::scene_mng::input(const sf::Event & e) noexcept {
	if (!scene_list_.count(cur_scene_)) return;
	auto & s = scene_list_.at(cur_scene_);
	s->input(e);
	return;
}

void jk::scene_mng::render() {
	if (!scene_list_.count(cur_scene_)) return;
	auto endflag = scene_list_.at(cur_scene_)->render();
	if (endflag == SCENEFLAG::FINISHED) {
		cur_scene_ = scene_list_.at(cur_scene_)->get_next_scene();
		if(scene_list_.count(cur_scene_))scene_list_.at(cur_scene_)->init(hm_, rw_);
	}
}

const std::shared_ptr<jk::scene> jk::scene_mng::get(SCENE_LIST key) const noexcept {
	if (scene_list_.count(key)) return scene_list_.at(key);
	return std::shared_ptr<jk::scene>(nullptr);
}
