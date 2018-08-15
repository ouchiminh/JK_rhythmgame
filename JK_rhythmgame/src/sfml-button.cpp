#include "sfml-button.hpp"

jk::button::button(const sf::Text & t, const sf::Sprite & bkg) : bkg_{ bkg }, title_{ t } {}

void jk::button::draw(sf::RenderTarget & rt, sf::RenderStates rs) const {
	rt.draw(bkg_, rs);
	rt.draw(title_, rs);
}

sf::FloatRect jk::button::get_rect() const noexcept {
	return bkg_.getGlobalBounds();
}

jk::PROCESSED jk::button::event_procedure(const sf::Event & e) {
	return handlers_(e, bkg_, title_, *this);
}
