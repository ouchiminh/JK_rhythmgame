﻿#include <string>
#include "color-manager.hpp"
#include "exit-scene.hpp"
#include "sfml-button.hpp"
#include "button_effect.hpp"
#include "sfmlUtl.hpp"
#include "resource_dic.hpp"

inline std::int32_t jk::exit_scene::on_mouse_click(const sf::Event & e, sf::Sprite &, sf::Text &, jk::button & b) {
	if (!b.get_rect().contains(sf::Vector2f((float)e.mouseButton.x, (float)e.mouseButton.y)) ||
		e.mouseButton.button != sf::Mouse::Button::Left)
		return 0;
	flag_ = SCENEFLAG::FINISHED;
	return 0;
}

std::int32_t jk::exit_scene::on_mouse_click_yes(const sf::Event & e, sf::Sprite &, sf::Text &, jk::button & b) {
	if (!b.get_rect().contains(sf::Vector2f((float)e.mouseButton.x, (float)e.mouseButton.y)) ||
		e.mouseButton.button != sf::Mouse::Button::Left)
		return 0;
	w_->close();
	return 0;
}

void jk::exit_scene::finish() {}

void jk::exit_scene::init_ui() {
	auto f = font_dictionary.at(".\\res\\fonts\\Perfograma.otf");
	jk::change_color_effect<jk::on_mouse_hover> effect(jk::color::color_mng::get("Data.theme_color").value_or(theme_color), &mtx_);
	sf::Text button_title[2] = { sf::Text(sf::String("Yes"), f, 50), sf::Text(sf::String("No"), f, 50) };
	for (auto i = 0; i < 2; i++) {
		auto screen = w_->getSize();
		screen.x /= 2;
		button_title[i].setFillColor(jk::color::color_mng::get("Data.str_color").value_or(str_color));
		jk::adjust_pos(button_title[i], *w_, jk::ADJUSTFLAG::VCENTER | (!i ? jk::ADJUSTFLAG::LEFT : jk::ADJUSTFLAG::RIGHT), w_->getSize().x * (!i?0.3f:-0.3f));

		auto & evh = ui_mng_.create<jk::button>(button_title[i])->handlers_;
		evh << std::make_pair(sf::Event::EventType::MouseMoved, effect);
		if (!i) evh << std::make_pair(sf::Event::EventType::MouseButtonPressed, [this](const auto &e, auto &s, auto &t, auto &b) {return on_mouse_click_yes(e, s, t, b); });
		else evh << std::make_pair(sf::Event::EventType::MouseButtonPressed, [this](const auto &e, auto &s, auto &t, auto &b) {return on_mouse_click(e, s, t, b); });
	}
}

void jk::exit_scene::init(HMODULE hm, sf::RenderWindow & w) {
	std::lock_guard<decltype(mtx_)> l(mtx_);
	flag_ = jk::SCENEFLAG::RUNNING;
	if (w_) return;
	w_ = &w;
	jk::load_from_file<sf::Font>(".\\res\\fonts\\Perfograma.otf");
	verification_message_.setFont(font_dictionary.at(".\\res\\fonts\\Perfograma.otf"));
	verification_message_.setString(L"Are you sure you want to exit?");
	verification_message_.setFillColor(jk::color::color_mng::get("Data.str_color").value_or(str_color));
	verification_message_.setCharacterSize(80);
	jk::adjust_pos(verification_message_, *w_, jk::ADJUSTFLAG::TOP | jk::ADJUSTFLAG::HCENTER, 0, static_cast<float>(w_->getSize().y * 0.2f));
	init_ui();
}

bool jk::exit_scene::free_resource() noexcept {
	std::lock_guard<decltype(mtx_)> l(mtx_);
	finish();
	ui_mng_.get_list().clear();
	verification_message_ = sf::Text();
	w_ = nullptr;
	return true;
}

jk::SCENEFLAG jk::exit_scene::render() {
	w_->clear(jk::color::color_mng::get("Data.bkg_color").value_or(bkg_color));
	w_->draw(verification_message_);

	mtx_.lock_shared();
	ui_mng_.draw(*w_);
	mtx_.unlock_shared();

	w_->display();
	return flag_;
}

jk::SCENE_LIST jk::exit_scene::get_next_scene() const noexcept {
	return jk::SCENE_LIST::Null;
}

void jk::exit_scene::input(const sf::Event & e) noexcept {
	ui_mng_.event_procedure(e);
}
