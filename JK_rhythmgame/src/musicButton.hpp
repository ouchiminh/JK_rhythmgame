#pragma once

#include "sfml-button.hpp"
#include "beatmap.hpp"

namespace jk {
	class musicButton : public button {
		friend ui_mng;

	private:
		// beatmap_directory��beatmap�����������Ƃ��ɁA
		// make_shared����Ă��Ȃ��̂�shared_ptr�ŊǗ��ł��Ȃ��B(��������ƏI�����ɓ�d���)
		beatmap* beatmap_ = nullptr;

	public:
		musicButton() noexcept = default;
		musicButton(beatmap * beatmap, const sf::Text & t, const sf::Texture & bkg);
		musicButton(beatmap * beatmap, const sf::Text & t, std::string_view bkg_filename);
		musicButton(beatmap * beatmap, const sf::Text & t);

		beatmap* get_beatmap() const;

	};
}