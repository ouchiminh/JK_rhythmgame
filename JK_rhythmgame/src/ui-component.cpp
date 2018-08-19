#include "ui-component.hpp"
using namespace jk;
std::uint32_t jk::ui_mng::event_procedure(const sf::Event & e) {
	result_t r{ 0 };
	for (auto & i : ui_list_) {
		r = i->event_procedure(e);
		if (r.get_fallthrough() == NO_FALLTHROUGH && r.get_is_processed()) break;
	}
	return r.get_retv();
}

void jk::ui_mng::draw(sf::RenderWindow & rt, sf::RenderStates rs) const {
	for (auto & i : ui_list_) rt.draw(*i, rs);
}

std::list<std::shared_ptr<ui_component>> & jk::ui_mng::get_list() noexcept {
	return ui_list_;
}

const std::list<std::shared_ptr<ui_component>>& jk::ui_mng::get_list() const noexcept {
	return ui_list_;
}
