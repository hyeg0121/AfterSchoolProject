#include<stdio.h>
#include<SFML/Graphics.hpp>
#include<stdlib.h>
#include<time.h>
#include<SFML/Audio.hpp>

using namespace sf;

struct Player {
	RectangleShape sprite; //그림이 되는 부분
	int speed;
	int score;
	int life;
};

struct Enemy {
	RectangleShape sprite;
	const int NUM = 10;
	int life;
	int speed;
	int score;
	SoundBuffer explosion_buffer;
	Sound explosion_sound;

};

int main(void) {

	RenderWindow window(VideoMode(640, 480), "AfterSchool"); // 윈도우 창 생성
	window.setFramerateLimit(60);

	srand(time(0));

	long start_time = clock(); //게임 시작 시간
	long spent_time;//게임 진행 시간

	Font font; 
	font.loadFromFile("C:\\windows\\Fonts\\arial.ttf");

	Text text; //score
	text.setFont(font);
	text.setCharacterSize(30);
	text.setFillColor(Color(255, 255, 255));
	text.setPosition(0, 0);
	text.setString("score : time : life :");

	Texture bg_texture;
	bg_texture.loadFromFile("./resources/images/background.jpg");
	Sprite bg_sprite;
	bg_sprite.setTexture(bg_texture);
	bg_sprite.setPosition(0, 0);

	//Player
	struct Player player;
	player.sprite.setSize(Vector2f(40, 40));
	player.sprite.setPosition(100, 100);
	player.sprite.setFillColor(Color::Red);
	player.speed = 5; //플레이어의 움직임 속도
	player.score = 0; //플레이어의 점수
	player.life = 3;
	char player_str[50];

	//Enemy
	const int ENEMY_NUM = 10;
	Enemy enemy[ENEMY_NUM];

	for (int i = 0; i < ENEMY_NUM; i++) {
		enemy[i].explosion_buffer.loadFromFile("./resources/sounds/rumble.flac");
		enemy[i].explosion_sound.setBuffer(enemy[i].explosion_buffer);
		enemy[i].score = 100;
		enemy[i].sprite.setSize(Vector2f(70, 70));
		enemy[i].sprite.setPosition(rand()%300+300, rand()%385);
		enemy[i].sprite.setFillColor(Color::Yellow);
		enemy[i].life = 1;
		enemy[i].speed = -(rand() % 10 * 1);
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
					for (int i = 0; i < ENEMY_NUM; i++) {
						enemy[i].sprite.setSize(Vector2f(70, 70));
						enemy[i].sprite.setPosition(rand() % 300 + 300, rand() % 385);
						enemy[i].sprite.setFillColor(Color::Yellow);
						enemy[i].life = 1;
						enemy[i].speed = -(rand() % 10 + 1);
						window.draw(enemy[i].sprite);
					}
				}
			}///case

			}//switch

		}//while
		
		spent_time = clock() - start_time;

		//방향키를 눌렀을 때 플레이어 움직임
		if (Keyboard::isKeyPressed(Keyboard::Left)) {
			player.sprite.move(-player.speed, 0);
		}
		if (Keyboard::isKeyPressed(Keyboard::Up)) {
			player.sprite.move(0, -player.speed);
		}
		if (Keyboard::isKeyPressed(Keyboard::Right)) {
			player.sprite.move(player.speed, 0);
		}
		if (Keyboard::isKeyPressed(Keyboard::Down)) {
			player.sprite.move(0, player.speed);
		}//방향키 end

		//enemy와 충돌
		for (int i = 0; i < ENEMY_NUM; i++) {
			if (enemy[i].life > 0) {

				if (player.sprite.getGlobalBounds().intersects(enemy[i].sprite.getGlobalBounds())) 
				{
					printf("enemy[%d]와 충돌\n", i);
					enemy[i].life -= 1;
					player.score += enemy[i].score;
					
					if(enemy[i].life == 0) {
						enemy[i].explosion_sound.play();
					}

				}//if
				//적이 왼쪽 끝에 진입하려는 순간
				else if (enemy[i].sprite.getPosition().x < 0) 
				{ 
					player.life -= 1;
					enemy[i].life = 0;
				}//else if

				enemy[i].sprite.move(enemy[i].speed, 0);
			}//if
			
		}

		window.clear(Color::Black);
		window.draw(bg_sprite);

		for (int i = 0; i < ENEMY_NUM; i++) //enemy가 살아있을때만 draw
		{
			if (enemy[i].life > 0) {
				window.draw(enemy[i].sprite);
			}
		}

		sprintf_s(player_str, "score : %d  time : %d  life : %d\n",
			player.score, spent_time/1000,player.life);
		text.setString(player_str);

		//draw
		window.draw(player.sprite);
		window.draw(text);

		window.display();
		
	}//while

	return 0;
}