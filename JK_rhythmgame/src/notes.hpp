#pragma once
#include <memory>
#include <optional>
#include "SFML/Audio.hpp"
#include "aes/aes-utl.hpp"

namespace jk {
	namespace acceptable_range {
		const sf::Time PERFECT_DURATION = sf::milliseconds(19);
		const sf::Time GOOD_DURATION = sf::milliseconds(30);
		const sf::Time OK_DURATION = sf::microseconds(60);
	}
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
