#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <Windows.h>
#include <stdlib.h>
#include <stdio.h>
#include "SFML/Graphics.hpp"
#include "src/scene-mng.hpp"
#include "src/timeKeeper.h"

#define ONLY_TEST
#ifdef _DEBUG
#include "test/test-beatmap_directory.hpp"
#include "test/test-aes_utl.hpp"
#include "test/test-lane_key_map.hpp"
#include "test/test-color.hpp"
#include "test/test-map_select_renderer.hpp"
#endif

int WINAPI WinMain(
	HINSTANCE hInstance,      // 現在のインスタンスのハンドル
	HINSTANCE hPrevInstance,  // 以前のインスタンスのハンドル
	LPSTR lpCmdLine,          // コマンドライン
	int nCmdShow              // 表示状態
){
#ifdef _DEBUG
	jk::test::test_base::test();
#	ifdef ONLY_TEST
	return 0;
#	endif
#endif
	auto mode = sf::VideoMode::getDesktopMode();
	mode.height += 1;
	sf::RenderWindow window(mode, "orange game", sf::Style::None);
	jk::scene_mng scenes{ hInstance, window };
	timeKeeper fps(std::chrono::microseconds(1'000'000) / 30);
	scenes.init();

	sf::Event event;
	while (window.isOpen()) {
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
			else scenes.input(event);
		}
		if (!window.hasFocus()) fps.sleep();
		scenes.render();
	}
	return 0;
}
