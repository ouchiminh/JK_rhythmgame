#pragma once
#include <memory>
#include "SFML/Audio.hpp"
#include "aes/aes-utl.hpp"

namespace jk {
	class note {
		std::shared_ptr<sf::Music> music_;
	public:
		note(float sec, unsigned lane, std::shared_ptr<sf::Music> m);

		/// <summary>
		/// When user hit the note, call this function.
		/// </summary>
		/// <returns>timing accuracy between 0 & 1.0</returns>
		float hit();
		
	};
}
