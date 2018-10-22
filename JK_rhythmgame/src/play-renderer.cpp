#include "play-renderer.hpp"

jk::SCENEFLAG jk::beatmap_play_renderer::operator()() {
	rw_->clear();
	mng_.draw(*rw_);
	rw_->display();

	return bp_->get_music().lock()->getStatus() == sf::SoundSource::Status::Stopped ?
		jk::SCENEFLAG::FINISHED : jk::SCENEFLAG::RUNNING;
}

std::uint32_t jk::beatmap_play_renderer::input(const sf::Event & e) {
	return mng_.event_procedure(e);
}

void jk::beatmap_play_renderer::free_resource() noexcept {
	mng_.get_list().clear();
	rw_ = nullptr;
}

void jk::beatmap_play_renderer::init(beatmap && b, sf::RenderWindow & rw) {
	mng_.get_list().clear();
	bp_ = mng_.create<jk::beatmap_player>(b, rw.getSize());
	rw_ = &rw;

	bp_->get_music().lock()->play();
}
