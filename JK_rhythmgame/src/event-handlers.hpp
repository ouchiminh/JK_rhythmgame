#pragma once
#include <cstdint>
#include <functional>
#include <map>
#include "SFML/Window/Event.hpp"

namespace jk {
	template<class ...Args>
	using event_handler_t = std::function<std::uint32_t(const sf::Event &, Args...)>;

	template<class ...Args>
	class event_handlers {
		std::map<sf::Event::EventType, event_handler_t<Args...>> handlers_;

	public:
		event_handlers & operator << (std::pair<sf::Event::EventType, event_handler_t<Args...>> && pack) {
			handlers_.insert_or_assign(pack.first, pack.second);
			return *this;
		}
		event_handlers & operator -=(sf::Event::EventType key) {
			handlers_.erase(key);
			return *this;
		}
		std::uint32_t operator () (const sf::Event & e, Args ...args, bool & did) {
			did = true;
			if (handlers_.count(e.type)) return handlers_.at(e.type)(e, args...);
			did = false;
			return 0;
		}

		void clear();
	};
}