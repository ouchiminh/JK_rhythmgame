#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <Windows.h>
#include "SFML/Graphics.hpp"
#include "src/scene-mng.hpp"
#include "src/timeKeeper.h"

#ifdef _DEBUG
#include "test/test_archiver.hpp"
#endif

int WINAPI WinMain(
	HINSTANCE hInstance,      // 現在のインスタンスのハンドル
	HINSTANCE hPrevInstance,  // 以前のインスタンスのハンドル
	LPSTR lpCmdLine,          // コマンドライン
	int nCmdShow              // 表示状態
){
	sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "orange game", sf::Style::None);
	jk::scene_mng scenes{ hInstance, window };
	timeKeeper fps(std::chrono::microseconds(1'000'000) / 30);
#ifdef _DEBUG	
	jk::test::test_base::test();
#endif
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