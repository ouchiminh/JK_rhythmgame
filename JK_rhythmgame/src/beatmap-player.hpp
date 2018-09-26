#pragma once
#include "SFML/System/Time.hpp"
#include "ui-component.hpp"
#include "event-handlers.hpp"
#include "scene.hpp"
#include "beatmap.hpp"

namespace jk {
	
	class beatmap_player final : public ui_component{
		beatmap b_;
		sf::Time notes_visible_duration_;
		event_handlers<beatmap &> handlers_;

	private:
		beatmap_player(beatmap && b);
	public:
		result_t event_procedure(const sf::Event & e) override;
		void draw(sf::RenderTarget &, sf::RenderStates) const override;
		sf::FloatRect get_rect() const noexcept override;

	};
}
