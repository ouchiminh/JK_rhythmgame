#include "sfml-button.hpp"

jk::button::button(const sf::Text & t, const sf::Texture & bkg) : title_{ t }, texture_{ bkg } {
	bkg_.setTexture(texture_, true);
}

jk::button::button(const sf::Text & t, std::string_view bkg_filename) : title_ {t} {
	texture_.loadFromFile(bkg_filename.data());
	bkg_.setTexture(texture_, true);
}

jk::button::button(const sf::Text & t) : title_{ t } {}

void jk::button::draw(sf::RenderTarget & rt, sf::RenderStates rs) const {
	rt.draw(bkg_, rs);
	rt.draw(title_, rs);
}

sf::FloatRect jk::button::get_rect() const noexcept {
	if (texture_.getNativeHandle() == 0) return title_.getGlobalBounds();
	return bkg_.getGlobalBounds();
}

jk::result_t jk::button::event_procedure(const sf::Event & e) {
	return handlers_(e, bkg_, title_, *this);
}
