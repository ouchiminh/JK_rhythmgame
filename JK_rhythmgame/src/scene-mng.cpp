#include "scene-mng.hpp"
#include "exit-scene.hpp"
#include "menu_scene.hpp"
jk::scene_mng::scene_mng(HMODULE hm, sf::RenderWindow & w) : hm_{ hm }, rw_{ w } 
{}

void jk::scene_mng::init() {
	scene_list_.insert_or_assign(SCENE_LIST::Main_Menu, std::make_shared<jk::mainmenu>());
	scene_list_.insert_or_assign(SCENE_LIST::Exit, std::make_shared<jk::exit_scene>());
	cur_scene_.push(SCENE_LIST::Exit);
	cur_scene_.push(SCENE_LIST::Main_Menu);
	scene_list_[SCENE_LIST::Main_Menu]->init(hm_, rw_);
}

void jk::scene_mng::input(const sf::Event & e) noexcept {
	if (!scene_list_.count(cur_scene_.top())) return;
	auto & s = scene_list_.at(cur_scene_.top());
	s->input(e);
	return;
}

void jk::scene_mng::render() {
	if (!scene_list_.count(cur_scene_.top())) return;
	jk::SCENEFLAG endflag;
	try {
		 endflag = scene_list_.at(cur_scene_.top())->render();
	} catch (...) {}
	if (endflag == SCENEFLAG::FINISHED) {
		const auto scene_buf = scene_list_.at(cur_scene_.top())->get_next_scene();
		scene_buf == SCENE_LIST::Null ?
			cur_scene_.pop() :
			cur_scene_.push(scene_buf);
		if(scene_list_.count(cur_scene_.top()))scene_list_.at(cur_scene_.top())->init(hm_, rw_);
	}
}

const std::shared_ptr<jk::scene> jk::scene_mng::get(SCENE_LIST key) const noexcept {
	if (scene_list_.count(key)) return scene_list_.at(key);
	return std::shared_ptr<jk::scene>(nullptr);
}
