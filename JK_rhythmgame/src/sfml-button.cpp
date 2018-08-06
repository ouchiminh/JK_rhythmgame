#include "sfml-button.hpp"

jk::button::button(const sf::Text & t, const sf::Sprite & bkg) : bkg_{ bkg }, title_{ t } {}

void jk::button::draw(sf::RenderTarget & rt, sf::RenderStates rs) const {
	rt.draw(bkg_, rs);
	rt.draw(title_, rs);
}

jk::PROCESSED jk::button::recieve_event(const sf::Event & e) noexcept {
	jk::PROCESSED r{ 0 };
	bool did = false;
	r.each[1] = handlers_(e, bkg_, title_, did);
	r.mini[1] = did;
}