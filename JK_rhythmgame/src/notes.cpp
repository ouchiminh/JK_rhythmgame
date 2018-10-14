#include "notes.hpp"

jk::note::note(float sec, unsigned lane, std::shared_ptr<sf::Music> m) : time_{ sf::seconds(sec) }, lane_{ lane }, music_{ m } 
{}

std::optional<float> jk::note::hit() {
	return std::optional<float>(std::nullopt);
}

sf::Time jk::note::get_time_diff() const noexcept {
	return time_ - music_->getPlayingOffset();
}
