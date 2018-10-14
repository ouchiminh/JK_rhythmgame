#include "notes.hpp"

jk::note::note(float sec, unsigned lane, std::shared_ptr<sf::Music> m) : time_{ sf::seconds(sec) }, lane_{ lane }, music_{ m } 
{}

std::optional<float> jk::note::hit() {
	const auto diff = get_time_diff().asMilliseconds();
	if (diff > jk::acceptable_range::OK_DURATION.asMilliseconds())
		return std::optional<float>(std::nullopt);
	return std::abs(diff) < jk::acceptable_range::PERFECT_DURATION.asMilliseconds() ?
		1.f :
		std::abs(diff) < jk::acceptable_range::GOOD_DURATION.asMilliseconds() ?
		.6f :
		std::abs(diff) < jk::acceptable_range::OK_DURATION.asMilliseconds() ?
		.3f : .0f;
}

sf::Time jk::note::get_time_diff() const noexcept {
	return time_ - music_->getPlayingOffset();
}
