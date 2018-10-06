#pragma once

#include "sfml-button.hpp"
#include "beatmap.hpp"

namespace jk {
	class musicButton : public button {
		friend ui_mng;

	private:
		// beatmap_directoryでbeatmapが生成されるときに、
		// make_sharedされていないのでshared_ptrで管理できない。(もしすると終了時に二重解放)
		beatmap* beatmap_ = nullptr;

	public:
		musicButton() noexcept = default;
		musicButton(beatmap * beatmap, const sf::Text & t, const sf::Texture & bkg);
		musicButton(beatmap * beatmap, const sf::Text & t, std::string_view bkg_filename);
		musicButton(beatmap * beatmap, const sf::Text & t);

		beatmap* get_beatmap() const;

	};
}