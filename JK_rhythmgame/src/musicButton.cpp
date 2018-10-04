#include "musicButton.hpp"

jk::musicButton::musicButton(std::shared_ptr<beatmap> beatmap, const sf::Text & t, const sf::Texture & bkg)
	:jk::button(t, bkg)
	, beatmap_(beatmap){}

jk::musicButton::musicButton(std::shared_ptr<beatmap> beatmap, const sf::Text & t, std::string_view bkg_filename)
	: jk::button(t, bkg_filename)
	, beatmap_(beatmap) {}

jk::musicButton::musicButton(std::shared_ptr<beatmap> beatmap, const sf::Text & t)
	: jk::button(t)
	, beatmap_(beatmap){}

std::weak_ptr<jk::beatmap> jk::musicButton::get_beatmap() const
{
	return beatmap_;
}
