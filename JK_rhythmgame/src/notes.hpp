#pragma once
#include <memory>
#include <optional>
#include "SFML/Audio.hpp"
#include "aes/aes-utl.hpp"

namespace jk {
	class note {
		std::shared_ptr<sf::Music> music_;

		sf::Time time_;
		unsigned lane_;
	public:
		note(float sec, unsigned lane, std::shared_ptr<sf::Music> m);

		/// <summary>
		/// When user hit the note, call this function.
		/// </summary>
		/// <returns>
		/// @ 0.0 ~ 1.0	: sccessed. timing is good. 
		/// @ nullopt	: timing is not good.
		/// </returns>
		[[nodiscard]] std::optional<float> hit();

		[[nodiscard]] sf::Time get_time_diff() const noexcept;
	};
}
