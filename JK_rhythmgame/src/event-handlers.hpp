#pragma once
#include <cstdint>
#include <functional>
#include <map>
#include "SFML/Window/Event.hpp"

namespace jk {
	template<typename ret_t, class ...Args>
	using event_handler_t = std::function<ret_t(const sf::Event &, Args...)>;

	template<class ...Args, typename ret_t = std::uint32_t>
	class event_handlers {
		std::map<sf::Event::EventType, event_handler_t<ret_t, Args...>> handlers_;

	public:
		event_handlers & operator << (std::pair<sf::Event::EventType, event_handler_t> && pack) {
			handlers_.insert_or_assign(pack.first, pack.second);
			return *this;
		}
		event_handlers & operator -=(sf::Event::EventType key) {
			handlers_.erase(key);
			return *this;
		}
		std::uint32_t operator () (const sf::Event & e, Args ...args) {
			if (handlers_.count(e.type)) return handlers_.at(e.type)(e, args...);
			return 0;
		}

		void clear();
	};
}