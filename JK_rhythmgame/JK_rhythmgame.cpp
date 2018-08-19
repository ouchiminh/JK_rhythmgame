#define NOMINMAX
#include <Windows.h>
#include "SFML/Graphics.hpp"
#include "src/menu_scene.hpp"
#include "src/scene-mng.hpp"
#include "src/sfml-button.hpp"

int WINAPI WinMain(
	HINSTANCE hInstance,      // 現在のインスタンスのハンドル
	HINSTANCE hPrevInstance,  // 以前のインスタンスのハンドル
	LPSTR lpCmdLine,          // コマンドライン
	int nCmdShow              // 表示状態
){
	sf::RenderWindow window(sf::VideoMode(1280, 720), "orange game");
	jk::scene_mng scenes{ hInstance, window };
	window.setVerticalSyncEnabled(true);
	
	scenes.init();

	sf::Event event;
	while (window.isOpen()) {
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
			else scenes.input(event);
		}
		scenes.render();
	}
	return 0;
}