#pragma once
#define NOMINMAX
#include <chrono>
#include <functional>
#include <Windows.h>
#include "SFML/Window/Event.hpp"
#include "SFML/Graphics.hpp"
#include "sfml-button.hpp"
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
		sf::Image logo_[CNT];
		sf::Sprite logoSpr_[CNT];
		sf::Texture tx_[CNT];
		sf::RenderWindow * w_;
		std::chrono::steady_clock::time_point logo_started_;
	public:
		[[nodiscard]] SCENEFLAG operator() ();
		void init(HMODULE hm, sf::RenderWindow & w);
		void free_resource() noexcept;
	};

	class bkg_renderer {
		sf::RenderWindow * w_;
		sf::Time time_;
		sf::Clock clock_;
		bool started_;
	public:
		[[nodiscard]] SCENEFLAG operator() ();
		void init(sf::RenderWindow & w, sf::Time render_time = sf::seconds(1.0f));
	};

	class menu_renderer {
		sf::Texture bkg_tx_;
		sf::Sprite bkg_;
		sf::Font f;
		ui_mng ui_mng_;

		sf::RenderWindow * w_;

		bool did_initialized_;
		SCENEFLAG flag_;
	public:
		menu_renderer();

		[[nodiscard]] SCENEFLAG operator() ();
		void init(HMODULE hm, sf::RenderWindow & w);
		void free_resource() noexcept;
		std::uint32_t input(const sf::Event & e);
	private:
		std::int32_t on_button_click(const sf::Event & e, sf::Sprite & s, sf::Text & t, button & b);
	};

	class mainmenu : public scene {
		logo_renderer logo_;
		bkg_renderer bkg_;
		menu_renderer menu_;
		SCENEFLAG(mainmenu::*cur_renderer)();

		SCENEFLAG render_logo();
		SCENEFLAG render_bkg();
		SCENEFLAG render_menu();

		SCENE_LIST next_scene_;
	protected:
		void finish() override;
	public:
		void init(HMODULE hm, sf::RenderWindow & w) override;
		virtual bool free_resource() noexcept override;

		virtual SCENEFLAG render() override;
		[[nodiscard]] virtual SCENE_LIST get_next_scene() const noexcept override;
		virtual void input(const sf::Event & e) noexcept override;

		~mainmenu() = default;
	};
}
