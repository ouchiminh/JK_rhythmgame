#define NOMINMAX
#include <windows.h>
#include <stdexcept>
#include "../resource.h"
#include "menu_scene.hpp"
#include "sfmlUtl.hpp"

namespace {
	const timeKeeper fps;
	const sf::Color bkg_color = sf::Color::White;
}

jk::SCENEFLAG jk::mainmenu::render_logo() {
	jk::SCENEFLAG ret;
	if ((ret = logo()) == SCENEFLAG::FINISHED) cur_renderer = &jk::mainmenu::render_bkg;
	return ret;
}

jk::SCENEFLAG jk::mainmenu::render_bkg() {
	jk::SCENEFLAG ret;
	if ((ret = bkg()) == SCENEFLAG::FINISHED) cur_renderer = &jk::mainmenu::render_menu;
	return ret;
}

jk::SCENEFLAG jk::mainmenu::render_menu() {
	jk::SCENEFLAG ret{ SCENEFLAG::FINISHED };

	return ret;
}

void jk::mainmenu::finish() {}

void jk::mainmenu::init(HMODULE hm, sf::RenderWindow & w) {
	logo.init(hm, w);
	bkg.init(w);
	cur_renderer = &jk::mainmenu::render_logo;
	next_scene = SCENE_LIST::Main_Menu;
}

bool jk::mainmenu::free_resource() noexcept {
	logo.free_resource();
	return true;
}

jk::SCENEFLAG jk::mainmenu::render() {
	(this->*cur_renderer)();
	return SCENEFLAG();
}

std::intptr_t jk::mainmenu::get_next_scene() const noexcept { return SCENEFLAG(); }

void jk::mainmenu::input(const sf::Event & e) noexcept {}

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
	w_->clear(sf::Color::White);
	w_->draw(logoSpr_[render_target], sf::BlendAlpha);
	w_->display();
	fps.sleep();
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
	fps.start();
	logo_started_ = std::chrono::steady_clock::now();
}

inline void jk::logo_renderer::free_resource() noexcept {
	for (unsigned i = 0; i < LOGONUM::CNT; ++i) {
		logo_[i].~Image();
		logoSpr_[i].~Sprite();
		tx_[i].~Texture();
		w_ = nullptr;
	}
	return;
}

jk::SCENEFLAG jk::bkg_renderer::operator()() {
	w_->clear(bkg_color);
	w_->display();
	return SCENEFLAG::RUNNING;
}

void jk::bkg_renderer::init(sf::RenderWindow & w) { w_ = &w; }
