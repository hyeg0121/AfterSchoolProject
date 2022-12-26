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
	int player_speed = 5; //플레이어의 움직임 속도

	RectangleShape enemy[5];
	int enemy_life[5];
	for (int i = 0; i < 5; i++) {
		enemy[i].setSize(Vector2f(70, 70));
		enemy[i].setPosition(500, 100*i);
		enemy[i].setFillColor(Color::Yellow);
		enemy_life[i] = 1;
	}

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
			player.move(-player_speed, 0);
		}
		if (Keyboard::isKeyPressed(Keyboard::Up)) {
			player.move(0, -player_speed);
		}
		if (Keyboard::isKeyPressed(Keyboard::Right)) {
			player.move(player_speed, 0);
		}
		if (Keyboard::isKeyPressed(Keyboard::Down)) {
			player.move(0, player_speed);
		}

		//적과 충돌
		for (int i = 0; i < 5; i++) {
			if (enemy_life[i] > 0) {

				if (player.getGlobalBounds().intersects(enemy[i].getGlobalBounds())) {
					printf("enemy[%d]와 충돌\n", i);
					enemy_life[i] -= 1;
				}
			}
		}

		window.clear(Color::Black);

		for (int i = 0; i < 5; i++)
		{
			if (enemy_life[i] > 0) {
				window.draw(enemy[i]);
			}
		}

		window.draw(player);
		window.display();
		
	}//while

	return 0;
}