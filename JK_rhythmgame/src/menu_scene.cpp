#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include <stdexcept>
#include <mutex>
#include "../resource.h"
#include "menu_scene.hpp"
#include "sfmlUtl.hpp"
#include "sfml-button.hpp"

namespace {
	const sf::Color bkg_color = sf::Color(0x0);
}

jk::SCENEFLAG jk::mainmenu::render_logo() {
	jk::SCENEFLAG ret;
	if ((ret = logo_()) == SCENEFLAG::FINISHED) cur_renderer_ = &mainmenu::render_bkg;
	return SCENEFLAG::RUNNING;
}

jk::SCENEFLAG jk::mainmenu::render_bkg() {
	jk::SCENEFLAG ret;
	if ((ret = bkg_()) == SCENEFLAG::FINISHED) cur_renderer_ = &mainmenu::render_menu;
	return SCENEFLAG::RUNNING;
}

jk::SCENEFLAG jk::mainmenu::render_menu() {
	jk::SCENEFLAG ret{ menu_() };
	if(ret == jk::SCENEFLAG::FINISHED) next_scene_ = jk::SCENE_LIST::Map_Select;
	return ret;
}

void jk::mainmenu::finish() {}

void jk::mainmenu::init(HMODULE hm, sf::RenderWindow & w) {
	logo_.init(hm, w);
	bkg_.init(w);
	menu_.init(hm, w);
	cur_renderer_ = &mainmenu::render_logo;
	next_scene_ = SCENE_LIST::Main_Menu;
}

bool jk::mainmenu::free_resource() noexcept {
	logo_.free_resource();
	menu_.free_resource();
	return true;
}

jk::SCENEFLAG jk::mainmenu::render() {
	return (this->*cur_renderer_)();
}

jk::SCENE_LIST jk::mainmenu::get_next_scene() const noexcept { return next_scene_; }

void jk::mainmenu::input(const sf::Event & e) noexcept {
	menu_.input(e);
}

jk::SCENEFLAG jk::logo_renderer::operator()() {
	using namespace std::chrono;
	unsigned render_target = 0;
	auto current_pod = logo_started_;
	auto current_time = steady_clock::now();
	if (current_time > logo_time * 2 + logo_started_) {
		return SCENEFLAG::FINISHED;
	} else if (current_time > logo_time + logo_started_) {
		render_target++;
		current_pod = logo_started_ + logo_time;
	}
	auto alpha = current_time < current_pod + fade_time ?
		gradual_change((std::uint8_t)0, (std::uint8_t)255,
		(double)fade_time.count(), (double)(current_time - current_pod).count()) :
		gradual_change((std::uint8_t)255, (std::uint8_t)0,
		(double)fade_time.count(), (double)(current_time - (current_pod + fade_time + disp_time)).count());

	logoSpr_[render_target].setColor(sf::Color{ 255,255,255,alpha });
	w_->clear(bkg_color);
	w_->draw(logoSpr_[render_target], sf::BlendAlpha);
	w_->display();
	return SCENEFLAG::RUNNING;
}

void jk::logo_renderer::init(HMODULE hm, sf::RenderWindow & w) {
	const HRSRC hres[2]{ FindResource(hm, MAKEINTRESOURCE(IDB_SFMLLOGO), TEXT("PNG")), FindResource(hm, MAKEINTRESOURCE(IDB_ORANGELOGO), TEXT("PNG")) };
	for (unsigned i = 0; i < LOGONUM::CNT; i++) {
		auto res{ LockResource(LoadResource(hm, hres[i])) };
		if (!res) throw std::runtime_error("cannot find resource");
		logo_[i].loadFromMemory(res, SizeofResource(hm, hres[i]));
		tx_[i] = sf::Texture{};
		tx_[i].create(logo_[i].getSize().x, logo_[i].getSize().y);
		tx_[i].update(logo_[i]);
		logoSpr_[i].setTexture(tx_[i]);
		jk::adjust_pos(logoSpr_[i], w, ADJUSTFLAG::CENTER);
	}
	w_ = &w;
	logo_started_ = std::chrono::steady_clock::now();
}

void jk::logo_renderer::free_resource() noexcept {
	for (unsigned i = 0; i < LOGONUM::CNT; ++i) {
		logo_[i] = sf::Image{};
		logoSpr_[i] = sf::Sprite{};
		tx_[i] = sf::Texture{};
		w_ = nullptr;
	}
	return;
}

jk::SCENEFLAG jk::bkg_renderer::operator()() {
	if (!started_) {
		started_ = true;
		clock_.restart();
	}
	w_->clear(bkg_color);
	w_->display();
	return clock_.getElapsedTime() > time_ ? started_ = false, SCENEFLAG::FINISHED : SCENEFLAG::RUNNING;
}

void jk::bkg_renderer::init(sf::RenderWindow & w, sf::Time render_time) {
	w_ = &w;
	time_ = render_time;
	started_ = false;
}


jk::menu_renderer::menu_renderer() : did_initialized_{ false }, flag_{ SCENEFLAG::NOTYET } {}

jk::SCENEFLAG jk::menu_renderer::operator()() {
	std::shared_lock<std::shared_mutex> lg(mtx_);
	if (flag_ == jk::SCENEFLAG::NOTYET) flag_ = jk::SCENEFLAG::RUNNING;
	w_->clear(bkg_color);
	w_->draw(bkg_);
	ui_mng_.draw(*w_);
	w_->display();
	return flag_;
}

void jk::menu_renderer::init(HMODULE hm, sf::RenderWindow & w) {
	using namespace std::placeholders;
	std::lock_guard<std::shared_mutex> lg(mtx_);

	expand_effect<on_mouse_hover> ee{mtx_};
	
	if (did_initialized_) return;
	sf::Text button_title;
	event_handler_t<sf::Sprite&, sf::Text&, button&> eh;
	eh = ee;
	f.loadFromFile(".\\res\\fonts\\meiryo.ttc");
	{
		button_title.setFont(f);
		button_title.setString("Game Start");
		jk::adjust_pos(button_title, w, jk::ADJUSTFLAG::HCENTER);
		auto pos = button_title.getPosition();
		pos.y = w.getSize().y * 0.8f;
		button_title.setCharacterSize(static_cast<unsigned>(w.getSize().y * 0.04));
		button_title.setPosition(pos);
		button_title.setFillColor(sf::Color(0xFF, 0xA0, 0x00));
	}
	ui_mng_.create<jk::button>(button_title)->handlers_ <<
		std::make_pair(sf::Event::EventType::MouseButtonPressed, [&](const sf::Event & e, sf::Sprite & s, sf::Text & t, button & b) {
			return on_button_click(e, s, t, b);
		}) << 
		std::make_pair(sf::Event::EventType::MouseMoved, eh);
	w_ = &w;
	did_initialized_ = true;
	flag_ = jk::SCENEFLAG::NOTYET;
}

void jk::menu_renderer::free_resource() noexcept {
	std::lock_guard<std::shared_mutex> lg(mtx_);
	bkg_tx_ = sf::Texture{};
	bkg_ = sf::Sprite{};
	f = sf::Font{};
	ui_mng_.get_list().clear();
	did_initialized_ = false;
}

std::uint32_t jk::menu_renderer::input(const sf::Event & e){
	return ui_mng_.event_procedure(e);
}

std::int32_t jk::menu_renderer::on_button_click(const sf::Event & e, sf::Sprite & s, sf::Text & t, button & b) {
	std::lock_guard<std::shared_mutex> lg(mtx_);
	if (flag_ != jk::SCENEFLAG::RUNNING ||
		!b.get_rect().contains(sf::Vector2f{ (float)e.mouseButton.x, (float)e.mouseButton.y }) ||
		e.mouseButton.button != sf::Mouse::Button::Left)
		return 0;
	flag_ = FINISHED;
	return 0;
}
