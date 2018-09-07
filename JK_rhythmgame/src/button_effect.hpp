#pragma once
#include <shared_mutex>
#include <type_traits>
#include <chrono>
#include <optional>
#include "effect.hpp"
#include "SFML/Window/Event.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include "sfml-button.hpp"
#include "sfmlUtl.hpp"
#include "event-handlers.hpp"

namespace jk {
	using button_effect = basic_effect<const sf::Event &, sf::Sprite &, sf::Text &, button &>;

	class on_mouse_hover : virtual public button_effect {
	protected:
		virtual bool judge(const sf::Event & e, sf::Sprite &, sf::Text &, button & b) const override final {
			if (e.type != sf::Event::EventType::MouseMoved) return false;
			sf::Vector2f p((float)e.mouseMove.x, (float)e.mouseMove.y);
			return b.get_rect().contains(p);
		}
	};

	template<class T>
	class change_color_effect : virtual public T {
	public:
		enum Target : int { BKG = 1, TXT = 1 << 1 };
	private:
		static_assert(std::is_base_of_v<button_effect, T>);
		std::shared_mutex *mtx_;
		sf::Color dest_color_;
		std::optional<sf::Color> original_color_[2];
		Target tar_;
		using T::es_;
	protected:
		virtual void proceed(const sf::Event & e, sf::Sprite & s, sf::Text & t, button & b) noexcept override final {
			std::lock_guard<decltype(*mtx_)>(*mtx_);
			if (!original_color_[0].has_value()) {
				original_color_[0] = s.getColor();
				original_color_[1] = t.getColor();
			}
			if (tar_ & Target::BKG) s.setColor(dest_color_);
			if (tar_ & Target::TXT) t.setColor(dest_color_);
		}
		virtual void finish(const sf::Event & e, sf::Sprite & s, sf::Text & t, button & b) noexcept override final {
			std::lock_guard<decltype(*mtx_)>(*mtx_);
			if (tar_ & Target::BKG) s.setColor(original_color_[0].value_or(sf::Color(255,255,255,0)));
			if (tar_ & Target::TXT) t.setColor(original_color_[1].value_or(sf::Color(255,255,255,0)));
		}
	public:
		
		change_color_effect(sf::Color && to, std::shared_mutex * mtx, Target tar = Target::TXT) :
			dest_color_{ to }, mtx_{ mtx }, tar_{ tar }
		{}
		change_color_effect(const sf::Color & to, std::shared_mutex * mtx, Target tar = Target::TXT) :
			dest_color_{ to }, mtx_{ mtx }, tar_{ tar }
		{}
	};

	template<class T>
	class expand_effect : virtual public T {
		static_assert(std::is_base_of_v<button_effect, T>);
		std::chrono::steady_clock clock_;
		std::chrono::milliseconds duration_;
		float lower_lim_;
		float upper_lim_;
		std::shared_mutex * mtx_;

		using T::es_;
	protected:
		virtual void proceed(const sf::Event & e, sf::Sprite & s, sf::Text & t, button & b) noexcept override final {
			using namespace std::chrono;
			mtx_->lock_shared();
			const float d_dt = (upper_lim_ - lower_lim_) / duration_.count();
			const auto start = clock_.now();
			const float beg_scale = s.getScale().x;
			float scale = beg_scale;
			mtx_->unlock_shared();
			es_ = EFFECT_STATE::WORKING;
			while (scale < upper_lim_ && es_ == EFFECT_STATE::WORKING) {
				scale = std::min(duration_cast<milliseconds>(clock_.now() - start).count() * d_dt + beg_scale, upper_lim_);
				mtx_->lock();

				s.setScale(scale, scale);
				t.setScale(scale, scale);
				
				mtx_->unlock();
			}
		}

		virtual void finish(const sf::Event & e, sf::Sprite & s, sf::Text & t, button & b) noexcept override final {
			using namespace std::chrono;
			mtx_->lock_shared();
			const float d_dt = (lower_lim_ - upper_lim_) / duration_.count();
			const auto start = clock_.now();
			const float beg_scale = s.getScale().x;
			float scale = beg_scale;
			mtx_->unlock_shared();
			es_ = EFFECT_STATE::FINISHING;
			while (scale > lower_lim_ && es_ == EFFECT_STATE::FINISHING) {
				scale = std::max(duration_cast<milliseconds>(clock_.now() - start).count() * d_dt + beg_scale, lower_lim_);
				mtx_->lock();

				s.setScale(scale, scale);
				t.setScale(scale, scale);
	
				mtx_->unlock();
			}
			es_ = EFFECT_STATE::NOT_WORKING;
		}

	public:
		expand_effect(std::shared_mutex & mtx, float lower = 1.0f, float upper = 1.2f, std::chrono::milliseconds duration = std::chrono::milliseconds{150}) :
			mtx_{ &mtx }, lower_lim_{ lower }, upper_lim_{ upper }, duration_{ duration }
		{}
		expand_effect() = delete;
		using T::operator();
	};
}
