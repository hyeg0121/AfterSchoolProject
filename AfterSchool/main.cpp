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
	int width = 100, height = 100;
	float x, y; //플레이어 좌표
};

struct Enemy {
	RectangleShape sprite;
	const int NUM = 7;
	int life;
	int speed;
	int score;
	int width = 100, height = 100;
	SoundBuffer explosion_buffer;
	Sound explosion_sound;
	int respawn_time;

};

struct Bullet {
	RectangleShape sprite;
	Texture texture;
	Texture * texture_pointer;
	int speed;
	int is_fired;
	int width = 70, height = 30;
	SoundBuffer firing_buffer;
	Sound firing_sound;
};

struct Textures {
	Texture bg;			//배경 이미지
	Texture bullet;
	Texture enemy;		//enemy 이미지
	Texture gameover;	//게임 종료 이미지
	Texture player;		//player 이미지
};

//함수
//obj1과 obj2의 충돌 여부, 충돌하면 1을 반환, 충돌하지 않으면 0을 반환 
int is_collide(RectangleShape obj1, RectangleShape obj2) {
	if (obj1.getGlobalBounds().intersects(obj2.getGlobalBounds())) {
		return 1;
	}
	else {
		return 0;
	}
};

//전역변수
const int ENEMY_NUM = 8;					//enemy의 최대 개수
const int BULLET_NUM = 50;
const int W_WIDTH = 1500, W_HEIGHT = 600;	//창의 크기
const int GO_WIDTH = 800, GO_HEIGHT = 600;	//gameover 그림의 크기 

int main(void) {

	RenderWindow window(VideoMode(W_WIDTH, W_HEIGHT), "AfterSchool"); // 윈도우 창 생성
	window.setFramerateLimit(60);

	srand(time(0));

	long start_time = clock();		//게임 시작 시간
	long spent_time;				//게임 진행 시간
	long fired_time = 0;				//최근에 bullet을 발사한 시간
	int is_gameover = 0;

	//Textures
	struct Textures t;
	t.bg.loadFromFile("./resources/images/background.jpg");
	t.gameover.loadFromFile("./resources/images/gameover.png");
	t.player.loadFromFile("./resources/images/player.png");
	t.enemy.loadFromFile("./resources/images/enemy.png");
	t.bullet.loadFromFile("./resources/images/bullet.png");


	//BGM
	SoundBuffer BGM_buffer;
	BGM_buffer.loadFromFile("./resources/sounds/bgm.ogg");
	Sound BGM_sound;
	BGM_sound.setBuffer(BGM_buffer);
	BGM_sound.setLoop(1);
	BGM_sound.play();

	//Font
	Font font; 
	font.loadFromFile("C:\\windows\\Fonts\\comicbd.ttf");

	//Text
	Text text; //score
	text.setFont(font);
	text.setCharacterSize(30);
	text.setFillColor(Color(255, 255, 255));
	text.setOutlineColor(Color::Black);
	text.setOutlineThickness(1);
	text.setPosition(0, 0);
	text.setString("score : time : life :");

	//Background
	Sprite bg_sprite;
	bg_sprite.setTexture(t.bg);
	bg_sprite.setPosition(0, 0);

	//gameover
	Sprite gameover_sprite;
	gameover_sprite.setTexture(t.gameover);
	gameover_sprite.setPosition((W_WIDTH-GO_WIDTH)/2, (W_HEIGHT-GO_HEIGHT)/2);

	//Player
	struct Player player;
	player.sprite.setSize(Vector2f(player.width, player.height));
	player.sprite.setPosition(100, W_HEIGHT/2);
	player.x = player.sprite.getPosition().x;
	player.y = player.sprite.getPosition().y;
	player.sprite.setTexture(&t.player);
	player.speed = 8; //플레이어의 움직임 속도
	player.score = 0; //플레이어의 점수
	player.life = 10;
	char player_str[50];

	//Enemy
	struct Enemy enemy[ENEMY_NUM];
	for (int i = 0; i < ENEMY_NUM; i++) {

		// TODO : 굉장히 비효율적인 코드이무로 나중에 refactoring
		enemy[i].explosion_buffer.loadFromFile("./resources/sounds/explode.ogg");
		enemy[i].explosion_sound.setBuffer(enemy[i].explosion_buffer);
		enemy[i].score = 100;
		enemy[i].respawn_time = 7;
		enemy[i].life = 1;
		enemy[i].speed = -(rand() % 10 * 1);
		enemy[i].sprite.setSize(Vector2f(enemy[i].width, enemy[i].height));
		enemy[i].sprite.setPosition(rand()%300+W_WIDTH*0.9, rand()%385);
		enemy[i].sprite.setTexture(&t.enemy);
	}

	//총알
	struct Bullet bullet[BULLET_NUM];
	int bullet_speed = 20;
	int bullet_idx = 0;
	int bullet_delay = 250;
	for (int i = 0; i < BULLET_NUM; i++) {
		bullet[i].sprite.setSize(Vector2f(bullet[i].width, bullet[i].height));
		bullet[i].sprite.setPosition(player.x + 90, player.y + 50); //임시 테스트
		bullet[i].speed = 20;
		bullet[i].is_fired = 0;
		bullet[i].sprite.setTexture(&t.bullet);
		bullet[i].firing_buffer.loadFromFile("./resources/sounds/bulletfiring.wav");
		bullet[i].firing_sound.setBuffer(bullet[i].firing_buffer);
	}

	//프로그램 실행 중
	while (window.isOpen())//윈도우가 열려 있을 때 까지 창 유지 
	{
		Event event;

		//(x)버튼을 누르면 게임 종료
		while (window.pollEvent(event)) {

			switch (event.type) {

			//x버튼을 누르면 윈도우 창 종료
			case Event::Closed:
				window.close(); //윈도우를 닫음
				break;

			}//switch

		}//while



		//시간 구하기
		spent_time = clock() - start_time;



		/* player update */
		//방향키를 눌렀을 때 플레이어 움직임
		if (Keyboard::isKeyPressed(Keyboard::Left) ||
			Keyboard::isKeyPressed(Keyboard::A)) {
			if (player.sprite.getPosition().x >= 0) {
				 player.sprite.move(-player.speed, 0);
			}
		}
		if (Keyboard::isKeyPressed(Keyboard::Up) ||
			Keyboard::isKeyPressed(Keyboard::W)) {
			if (player.sprite.getPosition().y >= 0) {
				player.sprite.move(0, -player.speed);
			}
		}
		if (Keyboard::isKeyPressed(Keyboard::Right) ||
			Keyboard::isKeyPressed(Keyboard::D)) {
			if (player.sprite.getPosition().x <= W_WIDTH-player.width) {
				player.sprite.move(player.speed, 0);
			}
		}
		if (Keyboard::isKeyPressed(Keyboard::Down) ||
			Keyboard::isKeyPressed(Keyboard::S)) {
			if (player.sprite.getPosition().y <= W_HEIGHT-player.height) {
				player.sprite.move(0, player.speed);
			}
		}//방향키 end



		//플레이어 위치 구하기
		player.x = player.sprite.getPosition().x;
		player.y = player.sprite.getPosition().y;



		/* bullet update */
		//총알 발사 
		if (Keyboard::isKeyPressed(Keyboard::Space)) {
			//bullet delay
			if (spent_time - fired_time > bullet_delay) {
				// 총알이 발사되어있지 않다면
				if (!bullet[bullet_idx].is_fired)
				{
					bullet[bullet_idx].firing_sound.play();
					bullet[bullet_idx].sprite.setPosition(player.x + 50, player.y + 15);
					bullet[bullet_idx].is_fired = 1;
					bullet[bullet_idx].sprite.setPosition(player.x + 50, player.y + 15);
					bullet[bullet_idx].is_fired = 1;
					bullet_idx++;	// 다음총알이 발사할 수 있도록

					if (bullet_idx == 49) { //계속 발사할 수 있도록
						bullet_idx = 0;
					}

					fired_time = spent_time; //장전 시간
				}
			}
			
		}

		//bullet이 화면 끝에 도달하면 다시 발사 가능
		for (int i = 0; i < BULLET_NUM; i++) {
			if (bullet[i].sprite.getPosition().x >= W_WIDTH) {
				bullet[i].is_fired = 0;
				bullet[i].sprite.setPosition(player.x + 70, player.y + 30);
			}
			//총알 움직임
			if (bullet[i].is_fired) {
				bullet[i].sprite.move(bullet[i].speed, 0);
			}
		}





		/* enemy update*/
		for (int i = 0; i < ENEMY_NUM; i++) {

			//10초마다 enemy가 젠
			if (spent_time % (1000*enemy[i].respawn_time) < 1000 / 60 + 1) {
				enemy[i].sprite.setSize(Vector2f(enemy[i].width, enemy[i].height));
				enemy[i].sprite.setPosition(rand() % 300 + W_WIDTH * 0.9, rand() % 500);
				enemy[i].life = 1;
				enemy[i].speed = -(rand() % 10 + 4 + spent_time%(1000*enemy[i].respawn_time));
				window.draw(enemy[i].sprite);
			}

			for (int j = 0; j < BULLET_NUM; j++) {
				//bullet과 enemy, player와 enemy가 충돌
				if (enemy[i].life > 0) {
					// TODO : 총알이 관통하는 버그 수정
					if (is_collide(bullet[j].sprite, enemy[i].sprite))
					{
						printf("enemy[%d]와 충돌\n", i);
						enemy[i].life -= 1;
						player.score += enemy[i].score;
						bullet[j].is_fired = 0;
						bullet[j].sprite.setPosition(player.x + 70, player.y + 30);
					
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


				}//if
			}//for j

			//적 움직임
			enemy[i].sprite.move(enemy[i].speed, 0);		
		}//for i



		/* window update */
		window.clear(Color::Black);
		window.draw(bg_sprite);

		for (int i = 0; i < ENEMY_NUM; i++) //enemy가 살아있을때만 draw
		{
			if (enemy[i].life > 0) {
				window.draw(enemy[i].sprite);
			}
		}

		if (player.life <= 0)
		{
			is_gameover = 1;
		}


		/* score */
		sprintf_s(player_str, "score : %d  time : %d  life : %d\n",
			player.score, spent_time/1000,player.life);
		text.setString(player_str);

		/* draw */
		for (int i = 0; i < BULLET_NUM; i++) {
			if (bullet[i].is_fired) {
				window.draw(bullet[i].sprite);
			}
		}
		window.draw(player.sprite);
		window.draw(text);

		if (is_gameover) {
			window.draw(gameover_sprite);
			// TODO : 게임이 멈추는 것을 구현할 것
		}
		window.display();
		
	}//while

	return 0;
}