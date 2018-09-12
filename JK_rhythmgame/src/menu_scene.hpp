#pragma once
#ifndef NOMIMMAX
#define NOMINMAX
#endif
#include <chrono>
#include <functional>
#include <Windows.h>
#include <shared_mutex>
#include <optional>
#include "SFML/Window/Event.hpp"
#include "SFML/Graphics.hpp"
#include "event-handlers.hpp"
#include "sfml-button.hpp"
#include "button_effect.hpp"
#include "scene.hpp"

namespace jk {
	
	class logo_renderer : public renderer {
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
		[[nodiscard]] SCENEFLAG operator() () override;
		void init(HMODULE hm, sf::RenderWindow & w);
		void free_resource() noexcept override;
	};

	class bkg_renderer : public renderer{
		sf::RenderWindow * w_;
		sf::Time time_;
		sf::Clock clock_;
		bool started_;
	public:
		[[nodiscard]] SCENEFLAG operator() () override;
		void free_resource() noexcept {}
		void init(sf::RenderWindow & w, sf::Time render_time = sf::seconds(1.0f));
	};

	class menu_renderer : public renderer{
		std::shared_mutex mtx_;
		sf::Clock clock_;
		sf::Shader frag_;
		sf::Texture bkg_tx_;
		sf::Sprite bkg_;
		sf::Font f_;
		ui_mng ui_mng_;
		sf::RenderWindow * w_;

		bool did_initialized_;
		SCENEFLAG scene_flag_;

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
		SCENEFLAG(mainmenu::*cur_renderer_)();

		SCENEFLAG render_logo();
		SCENEFLAG render_bkg();
		SCENEFLAG render_menu();
		
		std::int32_t on_key_down(const sf::Event & e);

		SCENE_LIST next_scene_;
		bool did_init_;
	public:
		event_handlers<> handlers_;
	protected:
		void finish() override;
	public:
		mainmenu() : did_init_{ false } {}

		void init(HMODULE hm, sf::RenderWindow & w) override;
		virtual bool free_resource() noexcept override;

		virtual SCENEFLAG render() override;
		[[nodiscard]] virtual SCENE_LIST get_next_scene() const noexcept override;
		virtual void input(const sf::Event & e) noexcept override;

		~mainmenu() = default;
	};
}
