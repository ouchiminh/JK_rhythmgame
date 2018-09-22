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

void jk::button::set_position(sf::Vector2f pos) noexcept(noexcept(std::declval<sf::Transformable>().setPosition(sf::Vector2f()))) {
	auto rect(get_rect());
	sf::Vector2f org_pos(rect.left, rect.top);
	auto difference = pos - org_pos;
	// bkg_
	{
		auto bkg_pos = bkg_.getPosition();
		bkg_pos += difference;
		bkg_.setPosition(bkg_pos);
	}
	// title_
	{
		auto title_pos = title_.getPosition();
		title_pos += difference;
		title_.setPosition(title_pos);
	}
}

void jk::button::set_text(const sf::Text & t) {
	title_ = t;
}

void jk::button::set_bkg(const sf::Texture & t) {
	texture_ = t;
	bkg_.setTexture(texture_);
}

sf::Sprite & jk::button::get_bkg() noexcept {
	return bkg_;
}

sf::Text & jk::button::get_title() noexcept {
	return title_;
}

sf::Sprite const & jk::button::get_bkg() const noexcept {
	return bkg_;
}

sf::Text const & jk::button::get_title() const noexcept {
	return title_;
}

jk::result_t jk::button::event_procedure(const sf::Event & e) {
	return handlers_(e, bkg_, title_, *this);
}
