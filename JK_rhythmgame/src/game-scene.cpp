#include "game-scene.hpp"

void jk::game_scene::change_renderer() {
	if (cur_renderer_ == &map_select_) status_ = SCENEFLAG::FINISHED;
}

void jk::game_scene::init(HMODULE hm, sf::RenderWindow & w) {
	cur_renderer_ = &map_select_;
}

bool jk::game_scene::free_resource() noexcept {
	map_select_.free_resource();
	return true;
}

jk::SCENEFLAG jk::game_scene::render() {
	if (!cur_renderer_) return SCENEFLAG::FINISHED;
	if(cur_renderer_->operator() == SCENEFLAG::FINISHED) change_renderer();

	return status_;
}

jk::SCENE_LIST jk::game_scene::get_next_scene() const noexcept {
	return next_scene_;
}

void jk::game_scene::input(const sf::Event & e) noexcept {
	if (cur_renderer_) cur_renderer_->input(e);
	return;
}
