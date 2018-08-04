#pragma once
#include <cstdint>
#include <functional>
#include <map>
#include "SFML/Window/Event.hpp"

namespace jk {
	using event_handler_t = std::function<std::uint32_t(sf::Event)>;

	class event_handlers {
		std::map<sf::Event::EventType, event_handler_t> handlers_;

	public:
		event_handlers & operator << (std::pair<sf::Event::EventType, event_handler_t> && pack) {
			handlers_.insert_or_assign(pack.first, pack.second);
			return *this;
		}
		event_handlers & operator -=(sf::Event::EventType key) {
			handlers_.erase(key);
			return *this;
		}
		std::uint32_t operator () (const sf::Event & e) {
			if (handlers_.count(e.type)) handlers_.at(e.type)(e);
		}

		void clear();
	};
}