#include "musicButton.hpp"

void jk::musicButton::update()
{
	switch (state_)
	{
	case jk::musicButtonState::NOT_SELECTED:	//��I����
		break;

	case jk::musicButtonState::SELECTED:		//�I����
		break;

	case jk::musicButtonState::CLICKED:			//�N���b�N��
		break;

	}
}

jk::musicButton::musicButton(jk::beatmap* beatmap, const sf::Text & t, const sf::Texture & bkg)
	:jk::button(t, bkg)
	, beatmap_(beatmap){}

jk::musicButton::musicButton(jk::beatmap* beatmap, const sf::Text & t, std::string_view bkg_filename)
	: jk::button(t, bkg_filename)
	, beatmap_(beatmap) {}

jk::musicButton::musicButton(jk::beatmap* beatmap, const sf::Text & t)
	: jk::button(t)
	, beatmap_(beatmap){}

jk::beatmap* jk::musicButton::get_beatmap() const
{
	return beatmap_;
}

void jk::musicButton::setState(musicButtonState::STATE state)
{
	state_ = state;
}
