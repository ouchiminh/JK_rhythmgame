#include "game-scene.hpp"

void jk::game_scene::change_renderer() {
	if (cur_renderer_ == &map_select_) {
		auto selected_bm = map_select_.get_selected();
		if (selected_bm) {
			cur_renderer_ = &beatmap_play_;
			beatmap_play_.init(std::move(selected_bm.value()), *rw_);
		} else {
			status_ = jk::SCENEFLAG::FINISHED;
			next_scene_ = jk::SCENE_LIST::Null;
		}
	} else if (cur_renderer_ == &beatmap_play_) {
		cur_renderer_ = &map_select_;
	}
}

void jk::game_scene::finish() {
	status_ = jk::SCENEFLAG::FINISHED;
}

void jk::game_scene::init(HMODULE hm, sf::RenderWindow & w) {
	map_select_.init(&w);
	cur_renderer_ = &map_select_;
	status_ = jk::SCENEFLAG::NOTYET;
	next_scene_ = jk::SCENE_LIST::Game_Play;
	rw_ = &w;
}

bool jk::game_scene::free_resource() noexcept {
	map_select_.free_resource();
	beatmap_play_.free_resource();
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
