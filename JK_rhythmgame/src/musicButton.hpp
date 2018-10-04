#pragma once

#include "sfml-button.hpp"
#include "beatmap.hpp"

namespace jk {
	class musicButton : public button {
		friend ui_mng;

	private:
		std::weak_ptr<beatmap> beatmap_;

	public:
		musicButton() noexcept = default;
		musicButton(std::shared_ptr<beatmap> beatmap, const sf::Text & t, const sf::Texture & bkg);
		musicButton(std::shared_ptr<beatmap> beatmap, const sf::Text & t, std::string_view bkg_filename);
		musicButton(std::shared_ptr<beatmap> beatmap, const sf::Text & t);

		std::weak_ptr<beatmap> get_beatmap() const;

	};
}