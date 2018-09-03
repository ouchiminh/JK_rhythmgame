#include "game-scene.hpp"

namespace {
	template<class T>
	int get_renderer_idx(T*) noexcept { return -1; }

	template<>
	int get_renderer_idx<jk::map_select_renderer>(jk::map_select_renderer*) noexcept { return 0; }
}