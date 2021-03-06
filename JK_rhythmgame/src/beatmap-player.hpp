﻿#pragma once
#include <filesystem>
#include <optional>
#include "SFML/System/Time.hpp"
#include "SFML/Window/Keyboard.hpp"
#include "ui-component.hpp"
#include "event-handlers.hpp"
#include "beatmap.hpp"
#include "timeKeeper.h"
#include "continuum-state.hpp"

namespace jk::key {

enum KeyAssignableAct : unsigned {
	LANE_COUNT = jk::MAX_LANE_CNT, PAUSE
};

}

namespace jk {

	template<class Type>
	struct sum {
		Type total_ = 0;
		unsigned count_ = 0;
		sum() = default;
		Type operator+=(Type value) { return count_++, total_ += value; }
		Type operator+=(std::optional<Type> value) {
			if (!value) return total_;
			return count_++, total_ += value.value();
		}
		template<class ResultType = Type>
		ResultType avg() const { return static_cast<ResultType>(total_) / count_; }
	};

	class lane_key_map {
		//		<keycode,			lane	>
		std::map<sf::Keyboard::Key, unsigned> keymap_;

		// DO NOT FORGET UPDATE VERSION WHEN MODIFY THIS CLASS
		static inline constexpr int version = 0;

	public:
		lane_key_map() = default;
		lane_key_map(std::filesystem::path && config_file, unsigned lane_cnt);
		void set_default(unsigned lane_cnt) noexcept;

		using iterator = std::map<sf::Keyboard::Key, unsigned>::iterator;
		using const_iterator = std::map<sf::Keyboard::Key, unsigned>::const_iterator;
		iterator begin();
		iterator end();

		const_iterator cbegin() const;
		const_iterator cend() const;
		// if failed, set default and returns false
		bool load_config(std::filesystem::path && config_file, unsigned lane_cnt) noexcept;
		[[nodiscard]] std::optional<unsigned> get_lane(sf::Keyboard::Key key) const noexcept;
		[[nodiscard]] std::optional<sf::Keyboard::Key> get_key(unsigned lane) const noexcept;
	};
	
	class judge_viewer : public sf::Drawable, public sf::Transformable {
		timeKeeper disp_;
		sf::Text judge_[4];	// perfect, good, ok, miss
		unsigned current_ = sizeof(judge_) / sizeof(*judge_);
	public:
		judge_viewer(sf::Font & f, sf::Vector2u const & res);
		judge_viewer() = default;
		void init(sf::Font & f, sf::Vector2u const & res);
		void init(sf::Vector2u const & res);

		/// <summary>
		/// CALL ONLY WHEN HIT NOTES
		/// </summary>
		/// <param name="score">retval of note::hit</param>
		void update(float score);
		void update(std::optional<float> score);
		void draw(sf::RenderTarget & rt, sf::RenderStates s = sf::RenderStates::Default) const override;
	};

	class beatmap_player : public ui_component, public sf::Transformable {
		friend ui_mng;

		beatmap b_;
		lane_key_map lkm_;
		std::map<sf::Keyboard::Key, utl::continuum_state<bool>> key_state_;
		sf::Time notes_visible_duration_;
		judge_viewer jv_;
		sum<float> sum_;

		sf::RenderTexture screen_;
		sf::Sprite spr_;

		std::vector<sf::RectangleShape> hit_lines_;
	private:
		beatmap_player(beatmap & b, sf::Vector2u const & resolution);

		void draw_notes();
		void lightup_lane();
	public:
		result_t event_procedure(const sf::Event & e) override;

		void update() override;
		void draw(sf::RenderTarget & rt, sf::RenderStates rs = sf::RenderStates::Default) const override;
		[[deprecated]] sf::FloatRect get_rect() const noexcept override;
		float get_score() const noexcept;
		std::weak_ptr<sf::Music> get_music() const noexcept;
		bool is_end() const noexcept;
	};
}
