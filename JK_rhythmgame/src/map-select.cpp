#include "map-select.hpp"

void jk::map_select::init(HMODULE hm, sf::RenderWindow & w) noexcept {
	w_ = &w;
	di_ = std::filesystem::directory_iterator(".\\res\\beatmaps");
}

bool jk::map_select::free_resource() noexcept {
	components_.get_list().clear();
	w_ = nullptr;
	return true;
}
