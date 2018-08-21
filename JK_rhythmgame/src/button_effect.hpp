#pragma once
#include <shared_mutex>
#include <type_traits>
#include <chrono>
#include "effect.hpp"
#include "SFML/Window/Event.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include "sfml-button.hpp"
#include "event-handlers.hpp"

namespace jk {
	using button_effect = basic_effect<const sf::Event &, sf::Sprite &, sf::Text &, button &>;

	class on_mouse_hover : virtual public button_effect {
	protected:
		virtual bool judge(const sf::Event & e, sf::Sprite &, sf::Text &, button & b) const override {
			if (e.type != sf::Event::EventType::MouseMoved) return false;
			sf::Vector2f p((float)e.mouseMove.x, (float)e.mouseMove.y);
			return b.get_rect().contains(p);
		}
	};

	template<class T>
	class expand_effect : virtual public T {
		static_assert(std::is_base_of_v<button_effect, T>);
		std::chrono::steady_clock clock_;
		std::chrono::milliseconds duration_;
		float scale_;
		std::shared_mutex * mtx_;

		using T::es_;
	protected:
		virtual void proceed(const sf::Event & e, sf::Sprite & s, sf::Text & t, button & b) noexcept override {
			using namespace std::chrono;
			mtx_->lock_shared();
			const float d_dt = (scale_ - 1.0f) / duration_.count();
			const auto start = clock_.now();
			const float beg_scale = s.getScale().x;
			float scale = beg_scale;
			mtx_->unlock_shared();
			es_ = EFFECT_STATE::WORKING;
			while (scale < scale_ && es_ == EFFECT_STATE::WORKING) {
				scale = duration_cast<milliseconds>(clock_.now() - start).count() * d_dt + beg_scale;
				mtx_->lock();
				s.setScale(scale, scale);
				t.setScale(scale, scale);
				mtx_->unlock();
			}
		}

		virtual void finish(const sf::Event & e, sf::Sprite & s, sf::Text & t, button & b) noexcept override {
			using namespace std::chrono;
			mtx_->lock_shared();
			const float d_dt = (1.0f - scale_) / duration_.count();
			const auto start = clock_.now();
			const float beg_scale = s.getScale().x;
			float scale = beg_scale;
			mtx_->unlock_shared();
			es_ = EFFECT_STATE::FINISHING;
			while (scale > 1.0f && es_ == EFFECT_STATE::FINISHING) {
				scale = duration_cast<milliseconds>(clock_.now() - start).count() * d_dt + beg_scale;
				mtx_->lock();
				s.setScale(scale, scale);
				t.setScale(scale, scale);
				mtx_->unlock();
			}
			es_ = EFFECT_STATE::NOT_WORKING;
		}

	public:
		expand_effect(std::shared_mutex & mtx, float scale = 1.2f, std::chrono::milliseconds duration = std::chrono::milliseconds{150}) :
			mtx_{ &mtx }, scale_{ scale }, duration_{ duration }
		{}
		expand_effect() = delete;
		using T::operator();
	};
}
