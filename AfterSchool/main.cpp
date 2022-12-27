#include<stdio.h>
#include<SFML/Graphics.hpp>
#include<stdlib.h>
#include<time.h>
#include<SFML/Audio.hpp>

using namespace sf;

struct Player {
	RectangleShape sprite; //�׸��� �Ǵ� �κ�
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

	RenderWindow window(VideoMode(640, 480), "AfterSchool"); // ������ â ����
	window.setFramerateLimit(60);

	srand(time(0));

	long start_time = clock(); //���� ���� �ð�
	long spent_time;//���� ���� �ð�

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
	player.speed = 5; //�÷��̾��� ������ �ӵ�
	player.score = 0; //�÷��̾��� ����
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

	while (window.isOpen())//�����찡 ���� ���� �� ���� â ���� 
	{
		Event event;

		while (window.pollEvent(event)) {

			switch (event.type) {

			//x��ư�� ������ ������ â ����
			case Event::Closed:
				window.close(); //�����츦 ����
				break;

			//�����̽��� ������ ��� enemy �ʱ�ȭ 
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

		//����Ű�� ������ �� �÷��̾� ������
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
		}//����Ű end

		//enemy�� �浹
		for (int i = 0; i < ENEMY_NUM; i++) {
			if (enemy[i].life > 0) {

				if (player.sprite.getGlobalBounds().intersects(enemy[i].sprite.getGlobalBounds())) 
				{
					printf("enemy[%d]�� �浹\n", i);
					enemy[i].life -= 1;
					player.score += enemy[i].score;
					
					if(enemy[i].life == 0) {
						enemy[i].explosion_sound.play();
					}

				}//if
				//���� ���� ���� �����Ϸ��� ����
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

		for (int i = 0; i < ENEMY_NUM; i++) //enemy�� ����������� draw
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