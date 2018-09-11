#include <string>
#include "exit-scene.hpp"
#include "sfml-button.hpp"
#include "button_effect.hpp"
#include "sfmlUtl.hpp"

inline std::int32_t jk::exit_scene::on_mouse_click(const sf::Event & e, sf::Sprite &, sf::Text &, jk::button & b) {
	if (!b.get_rect().contains(sf::Vector2f((float)e.mouseButton.x, (float)e.mouseButton.y)))
		return 0;
	flag_ = SCENEFLAG::FINISHED;
}

std::int32_t jk::exit_scene::on_mouse_click_yes(const sf::Event & e, sf::Sprite &, sf::Text &, jk::button & b) {
	if (!b.get_rect().contains(sf::Vector2f((float)e.mouseButton.x, (float)e.mouseButton.y)))
		return 0;
	w_->close();
	return 0;
}

void jk::exit_scene::finish() {}

void jk::exit_scene::init_ui() {
	using namespace std::literals::string_literals;
	jk::change_color_effect<jk::on_mouse_hover> e(jk::color::theme_color, &mtx_);
	
	sf::Text button_title[2] = { sf::Text(sf::String("yes"), f_), sf::Text(sf::String("no"), f_) };
	for (auto i = 0; i < 2; i++) {
		auto screen = w_->getSize();
		screen.x /= 2 ;
		button_title[i].setFillColor(jk::color::str_color);
		jk::adjust_pos(button_title[i], w_->getSize(), jk::ADJUSTFLAG::VCENTER | (!i ? jk::ADJUSTFLAG::LEFT : jk::ADJUSTFLAG::RIGHT));
	}
	for (auto i = 0; i < 2; i++) ui_mng_.create<jk::button>(button_title[i]);
}

void jk::exit_scene::init(HMODULE hm, sf::RenderWindow & w) {
	if (w_) return;
	w_ = &w;
	f_.loadFromFile(".\\res\\fonts\\meiryo.ttc");
	verification_message_.setFont(f_);
	verification_message_.setString("exit?");
	verification_message_.setFillColor(jk::color::str_color);
	jk::adjust_pos(verification_message_, *w_, jk::ADJUSTFLAG::TOP | jk::ADJUSTFLAG::HCENTER);
	init_ui();
}

bool jk::exit_scene::free_resource() noexcept {
	finish();
	ui_mng_.get_list().clear();
	f_ = sf::Font();
	verification_message_ = sf::Text();
	w_ = nullptr;
	return true;
}

jk::SCENEFLAG jk::exit_scene::render() {
	w_->clear(jk::color::bkg_color);
	w_->draw(verification_message_);
	ui_mng_.draw(*w_);
	w_->display();
	return flag_;
}

jk::SCENE_LIST jk::exit_scene::get_next_scene() const noexcept {
	return jk::SCENE_LIST::Null;
}

void jk::exit_scene::input(const sf::Event & e) noexcept {
	ui_mng_.event_procedure(e);
}
