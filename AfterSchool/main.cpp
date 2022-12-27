#include<stdio.h>
#include<SFML/Graphics.hpp>
#include<stdlib.h>
#include<time.h>

using namespace sf;

int main(void) {

	RenderWindow window(VideoMode(640, 480), "AfterSchool"); // 윈도우 창 생성
	window.setFramerateLimit(60);

	srand(time(0));

	Font font; 
	font.loadFromFile("C:\\windows\\Fonts\\arial.ttf");

	Text text;
	text.setFont(font);
	text.setCharacterSize(50);
	text.setFillColor(Color(255, 255, 255));
	text.setPosition(0, 0);
	text.setString("score");

	RectangleShape player;
	player.setSize(Vector2f(40, 40));
	player.setPosition(100, 100);
	player.setFillColor(Color::Red);
	int player_speed = 5; //플레이어의 움직임 속도
	int player_score = 0; //플레이어의 점수


	RectangleShape enemy[5];
	int enemy_life[5];
	int enemy_score = 100; //enemy를 잡았을 때 점수
	for (int i = 0; i < 5; i++) {
		enemy[i].setSize(Vector2f(70, 70));
		enemy[i].setPosition(rand()%300+300, rand()%385);
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

			//스페이스를 누르면 모든 enemy 초기화 
			case Event::KeyPressed:
			{
				if (event.key.code == Keyboard::Space) {
					for (int i = 0; i < 5; i++) {
						enemy[i].setSize(Vector2f(70, 70));
						enemy[i].setPosition(rand() % 300 + 300, rand() % 385);
						enemy[i].setFillColor(Color::Yellow);
						enemy_life[i] = 1;
						window.draw(enemy[i]);
					}
				}
			}

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
		}//방향키 end

		//적과 충돌
		for (int i = 0; i < 5; i++) {
			if (enemy_life[i] > 0) {

				if (player.getGlobalBounds().intersects(enemy[i].getGlobalBounds())) {
					printf("enemy[%d]와 충돌\n", i);
					enemy_life[i] -= 1;
					player_score += enemy_score;
				}
			}
		}

		printf("score : %d\n", player_score);

		window.clear(Color::Black);

		for (int i = 0; i < 5; i++) //enemy가 살아있을때만 draw
		{
			if (enemy_life[i] > 0) {
				window.draw(enemy[i]);
			}
		}

		//draw
		window.draw(player);
		window.draw(text);

		window.display();
		
	}//while

	return 0;
}