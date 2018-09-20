#pragma once
#include "scene.hpp"
#include "map-select-renderer.hpp"
#include "SFML/Graphics.hpp"

namespace jk {
	
	class game_scene : public scene {
		map_select_renderer map_select_;
	};
}
