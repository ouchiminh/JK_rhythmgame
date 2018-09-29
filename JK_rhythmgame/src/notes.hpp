#pragma once
#include <memory>
#include <optional>
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
		/// <returns>
		/// @ 0.0 ~ 1.0	: sccessed. timing is good. 
		/// @ nullopt	: timing is not good.
		/// </returns>
		std::optional<float> hit();
		
	};
}
