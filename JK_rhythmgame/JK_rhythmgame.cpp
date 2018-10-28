#ifndef NOMINMAX
#	define NOMINMAX
#endif
#include <Windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <thread>
#include "SFML/Graphics.hpp"
#include "src/scene-mng.hpp"
#include "src/color-manager.hpp"
#include "src/timeKeeper.h"
#include "src/resource_dic.hpp"

#define ONLY_TEST
//#define NOTEST
#ifndef NOTEST
#	include "test/test.hpp"
#	include "test/test-beatmap_directory.hpp"
#	include "test/test-beatmap.hpp"
#	include "test/test-aes_utl.hpp"
#	include "test/test-lane_key_map.hpp"
#	include "test/test-color.hpp"
//#	include "test/test-beatmap-player.hpp"
//#	include "test/test-map_select_renderer.hpp"
#	include "test/test_ouchi-log.hpp"
#endif

int WINAPI WinMain(
	HINSTANCE hInstance,      // 現在のインスタンスのハンドル
	HINSTANCE hPrevInstance,  // 以前のインスタンスのハンドル
	LPSTR lpCmdLine,          // コマンドライン
	int nCmdShow              // 表示状態
){
#ifndef NOTEST
	jk::test::test_base::test();
#endif
#ifndef ONLY_TEST
	auto mode = sf::VideoMode::getDesktopMode();
	mode.height += 1;
	sf::RenderWindow window(mode, "orange game", sf::Style::None);
	jk::scene_mng scenes{ hInstance, window };
	timeKeeper fps(std::chrono::microseconds(1'000'000) / 30);
	jk::color::color_mng::set(".\\setting\\color.json");
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
#endif
	return 0;
}
