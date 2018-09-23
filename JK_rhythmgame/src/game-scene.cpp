#include "game-scene.hpp"

void jk::game_scene::change_renderer() {
	if (cur_renderer_ == &map_select_) status_ = SCENEFLAG::FINISHED;
}

void jk::game_scene::finish() {
	status_ = jk::SCENEFLAG::FINISHED;
}

void jk::game_scene::init(HMODULE hm, sf::RenderWindow & w) {
	cur_renderer_ = &map_select_;
	status_ = jk::SCENEFLAG::NOTYET;
}

bool jk::game_scene::free_resource() noexcept {
	map_select_.free_resource();
	return true;
}

jk::SCENEFLAG jk::game_scene::render() {
	if (!cur_renderer_) return finish(), SCENEFLAG::FINISHED;
	if((*cur_renderer_)() == SCENEFLAG::FINISHED) change_renderer();
	if (status_ == jk::SCENEFLAG::NOTYET) status_ = jk::SCENEFLAG::RUNNING;

	return status_;
}

jk::SCENE_LIST jk::game_scene::get_next_scene() const noexcept {
	return next_scene_;
}

void jk::game_scene::input(const sf::Event & e) noexcept {
	if (cur_renderer_) cur_renderer_->input(e);
	return;
}
