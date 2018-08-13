#pragma once
#define NOMINMAX
#include <chrono>
#include <functional>
#include <Windows.h>
#include "SFML/Window/Event.hpp"
#include "SFML/Graphics.hpp"
#include "ui-component.hpp"
#include "scene.hpp"
#include "timeKeeper.h"

namespace jk {
	
	class logo_renderer {
		using myduration = std::chrono::steady_clock::duration;
		static constexpr myduration fade_time =
			std::chrono::duration_cast<myduration>(std::chrono::milliseconds(1400));
		static constexpr myduration disp_time =
			std::chrono::duration_cast<myduration>(std::chrono::milliseconds(1000));
		static constexpr myduration logo_time = fade_time + disp_time + fade_time;

		enum LOGONUM : unsigned { SFML, MIKAN, CNT };
		sf::Image logo_[2];
		sf::Sprite logoSpr_[2];
		sf::Texture tx_[2];
		sf::RenderWindow * w_;
		std::chrono::steady_clock::time_point logo_started_;
	public:
		[[nodiscard]] SCENEFLAG operator() ();
		void init(HMODULE hm, sf::RenderWindow & w);
		void free_resource() noexcept;
	};

	class bkg_renderer {
		sf::RenderWindow * w_;
	public:
		[[nodiscard]] SCENEFLAG operator() ();
		void init(sf::RenderWindow & w);
	};

	class menu_renderer {
		sf::Texture bkg_;
		ui_mng ui_mng_;
	public:
		[[nodiscard]] SCENEFLAG operator() ();
		void init(HMODULE hm, sf::RenderWindow & w);
		void free_resource() noexcept;
	};

	class mainmenu : public scene {
		logo_renderer logo;
		bkg_renderer bkg;
		SCENEFLAG(mainmenu::*cur_renderer)();

		SCENEFLAG render_logo();
		SCENEFLAG render_bkg();
		SCENEFLAG render_menu();

		SCENE_LIST next_scene;
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
