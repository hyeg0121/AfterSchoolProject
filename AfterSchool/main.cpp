#include<stdio.h>
#include<SFML/Graphics.hpp>
using namespace sf;

int main(void) {

	RenderWindow window(VideoMode(640, 480), "AfterSchool"); // 윈도우 창 생성
	window.setFramerateLimit(60);

	RectangleShape player;
	player.setSize(Vector2f(40, 40));
	player.setPosition(100, 100);
	player.setFillColor(Color::Red);
	
	while (window.isOpen())//윈도우가 열려 있을 때 까지 창 유지 
	{
		Event event;

		while (window.pollEvent(event)) {

			switch (event.type) {

			//x버튼을 누르면 윈도우 창 종료
			case Event::Closed:
				window.close(); //윈도우를 닫음
				break;

			}

		}//while
		
		//방향키를 눌렀을 때 플레이어 움직임
		if (Keyboard::isKeyPressed(Keyboard::Left)) {
			player.move(-1, 0);
		}else if (Keyboard::isKeyPressed(Keyboard::Up)) {
			player.move(0, -1);
		}else if (Keyboard::isKeyPressed(Keyboard::Right)) {
			player.move(1, 0);
		}else if (Keyboard::isKeyPressed(Keyboard::Down)) {
			player.move(0, 1);
		}

		window.clear();
		window.draw(player);
		window.display();
		
	}//while

	return 0;
}