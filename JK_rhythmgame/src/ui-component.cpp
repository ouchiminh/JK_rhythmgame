#include "ui-component.hpp"
using namespace jk;
inline std::uint32_t jk::ui_mng::recieve_event(const sf::Event & e) {
	PROCESSED r{ 0 };
	for (auto & i : ui_list) {
		r = i->recieve_event(e);
		if (r.mini[0] == NO_FALLTHROUGH && r.mini[1] == true) break;
	}
	return r.each[1];
}

void jk::ui_mng::draw(sf::RenderWindow & rt, sf::RenderStates rs) const {
	for (auto & i : ui_list) rt.draw(*i, rs);
}

std::list<std::shared_ptr<ui_component>> jk::ui_mng::get_list() const noexcept {
	return ui_list;
}
