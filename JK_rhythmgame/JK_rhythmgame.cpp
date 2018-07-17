#define NOMINMAX
#include <Windows.h>
#include "SFML/Graphics.hpp"
#include "src\menu_scene.hpp"

int WINAPI WinMain(
	HINSTANCE hInstance,      // 現在のインスタンスのハンドル
	HINSTANCE hPrevInstance,  // 以前のインスタンスのハンドル
	LPSTR lpCmdLine,          // コマンドライン
	int nCmdShow              // 表示状態
){
	sf::RenderWindow window(sf::VideoMode(1280, 720), "orange game");
	jk::mainmenu mm;
	mm.init(hInstance, window);
	
	sf::Event event;
	while (window.isOpen()) {
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}
		mm.render();
	}
	mm.free_resource();

	return 0;
}