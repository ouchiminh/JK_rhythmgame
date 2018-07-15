#define NOMINMAX
#include <windows.h>
#include <stdexcept>
#include "../resource.h"
#include "menu_scene.hpp"
#include "sfmlUtl.hpp"

jk::SCENEFLAG jk::mainmenu::render_logo() {
	using namespace std::chrono;
	unsigned render_target = 0;
	auto current_pod = logo_started_;
	auto current_time = steady_clock::now();
	if (current_time > logo_time * 2 + logo_started_) { 
		cur_renderer = &jk::mainmenu::render_menu;
		return SCENEFLAG::RUNNING;
	}
	else if (current_time > logo_time + logo_started_) {
		render_target++;
		current_pod = logo_started_ + logo_time;
	}
	auto alpha = gradual_change((std::uint8_t)0, (std::uint8_t)255, 
		(double)fade_time.count(), (double)(current_time - current_pod).count());
	logoSpr_[render_target].setColor(sf::Color{ 255,255,255,alpha });
	w_->clear(sf::Color::White);
	w_->draw(logoSpr_[render_target], sf::BlendAlpha);
	w_->display();
	fps_.sleep();
	return SCENEFLAG::RUNNING;
}

jk::SCENEFLAG jk::mainmenu::render_menu() {
	return SCENEFLAG();
}

void jk::mainmenu::finish() {}

void jk::mainmenu::init(HMODULE hm, sf::RenderWindow & w) {
	const HRSRC hres[2]{ FindResource(hm, MAKEINTRESOURCE(IDB_SFMLLOGO), TEXT("PNG")), FindResource(hm, MAKEINTRESOURCE(IDB_ORANGELOGO), TEXT("PNG")) };
	for(unsigned i = 0; i < LOGONUM::CNT; i++) {
		auto res{ LockResource(LoadResource(hm, hres[i])) };
		if (!res) throw std::runtime_error("cannot find resource");
		logo_[i].loadFromMemory(res, SizeofResource(hm, hres[i]));
		tx_[i] = sf::Texture{};
		tx_[i].create(logo_[i].getSize().x, logo_[i].getSize().y);
		tx_[i].update(logo_[i]);
		logoSpr_[i].setTexture(tx_[i]);
		jk::adjust_pos(logoSpr_[i], w, ADJUSTFLAG::CENTER);
	}
	cur_renderer = &jk::mainmenu::render_logo;
	w_ = &w;
	fps_.start();
	logo_started_ = std::chrono::steady_clock::now();
}

bool jk::mainmenu::free_resource() noexcept {
	return true; 
}

jk::SCENEFLAG jk::mainmenu::render() {
	if (!w_) return (SCENEFLAG)-1;
	(this->*cur_renderer)();
	return SCENEFLAG();
}

std::intptr_t jk::mainmenu::get_next_scene() const noexcept { return SCENEFLAG(); }

void jk::mainmenu::input(const sf::Event & e) noexcept {}
