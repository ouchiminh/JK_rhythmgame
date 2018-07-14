#define NOMINMAX
#include <windows.h>
#include <stdexcept>
#include "../resource.h"
#include "menu_scene.hpp"
#include "sfmlUtl.hpp"

void jk::mainmenu::finish() {}

void jk::mainmenu::init(HMODULE hm, sf::RenderWindow & w) {
	const auto hres = FindResource(hm, MAKEINTRESOURCE(IDB_SFMLLOGO), TEXT("PNG"));
	auto res{ LockResource(LoadResource(hm, hres)) };
	if (!res) throw std::runtime_error("cannot find resource");
	logo_[0].loadFromMemory(res, SizeofResource(hm, hres));
	tx_[0] = sf::Texture{};
	tx_[0].create(logo_[0].getSize().x, logo_[0].getSize().y);
	tx_[0].update(logo_[0]);
	logoSpr_[0].setTexture(tx_[0]);
	jk::adjust_pos(logoSpr_[0], w, ADJUSTFLAG::CENTER);
	w_ = &w;
}

bool jk::mainmenu::free_resource() noexcept {
	return true; 
}

jk::SCENEFLAG jk::mainmenu::render() {
	if (!w_) return (SCENEFLAG)-1;
	w_->clear(sf::Color::White);
	w_->draw(logoSpr_[0]);
	w_->display();
	return SCENEFLAG();
}

jk::SCENEFLAG jk::mainmenu::get_next_scene() const noexcept { return SCENEFLAG(); }

void jk::mainmenu::input(const sf::Event & e) noexcept {}
