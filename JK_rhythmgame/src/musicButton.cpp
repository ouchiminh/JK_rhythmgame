#include "musicButton.hpp"
#include "color-manager.hpp"

void jk::musicButton::update()
{
	switch (state_)
	{
	case jk::musicButtonState::NOT_SELECTED:	//非選択時
		break;

	case jk::musicButtonState::SELECTED:		//選択時
		break;

	case jk::musicButtonState::CLICKED:			//クリック時
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
	if (state_ == musicButtonState::STATE::SELECTED)
		button::title_.setFillColor(jk::color::color_mng::get("Data.theme_color").value_or(jk::color::theme_color));
	else if (state_ == musicButtonState::STATE::NOT_SELECTED)
		button::title_.setFillColor(jk::color::color_mng::get("Data.str_color").value_or(jk::color::str_color));
}

auto jk::musicButton::getState() const noexcept->musicButtonState::STATE { return state_; }
