#pragma once
#include "SFML\Graphics.hpp"
#include "scene.hpp"

namespace jk {
	class mainmenu : public scene {
		void(*cur_renderer)();

		void render_logo();
		void render_manu();
	public:
		void init();
		virtual bool free_resource() override;

		virtual SCENEFLAG render() override;
		virtual SCENEFLAG get_next_scene() override;
	};
}
