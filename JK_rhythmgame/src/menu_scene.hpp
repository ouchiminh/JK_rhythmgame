#pragma once
#define NOMINMAX
#include <chrono>
#include <functional>
#include <Windows.h>
#include "SFML/Window/Event.hpp"
#include "SFML/Graphics.hpp"
#include "scene.hpp"
#include "timeKeeper.h"

namespace jk {
	class mainmenu : public scene {
		using myduration = std::chrono::steady_clock::duration;
		inline static const myduration fade_time =
			std::chrono::duration_cast<myduration>(std::chrono::milliseconds(1400));
		inline static const myduration disp_time =
			std::chrono::duration_cast<myduration>(std::chrono::milliseconds(1000));
		inline static const myduration logo_time = fade_time + disp_time;
		
		enum LOGONUM : unsigned { SFML, MIKAN, CNT };

		sf::Image logo_[2];
		sf::Sprite logoSpr_[2];
		sf::Texture tx_[2];
		sf::RenderWindow * w_;
		timeKeeper fps_;
		std::chrono::steady_clock::time_point logo_started_;

		SCENEFLAG (mainmenu::*cur_renderer)();

		SCENEFLAG render_logo();
		SCENEFLAG render_menu();	// メニューUIデザイン未定のため未実装
	protected:
		void finish() override;
	public:
		void init(HMODULE hm, sf::RenderWindow & w);
		virtual bool free_resource() noexcept override;

		virtual SCENEFLAG render() override;
		[[nodiscard]] virtual std::intptr_t get_next_scene() const noexcept override;
		virtual void input(const sf::Event & e) noexcept override;
	};
}
