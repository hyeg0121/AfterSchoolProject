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

};

struct Enemy {

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
	text.setString("score : ");

	Texture bg_texture;
	bg_texture.loadFromFile("./resources/images/background.jpg");
	Sprite bg_sprite;
	bg_sprite.setTexture(bg_texture);
	bg_sprite.setPosition(0, 0);

	struct Player player;
	player.sprite.setSize(Vector2f(40, 40));
	player.sprite.setPosition(100, 100);
	player.sprite.setFillColor(Color::Red);
	player.speed = 5; //�÷��̾��� ������ �ӵ�
	player.score = 0; //�÷��̾��� ����
	char player_str[50];

	//enemy
	const int ENEMY_NUM = 10;
	RectangleShape enemy[ENEMY_NUM];
	int enemy_life[ENEMY_NUM];
	int enemy_score = 100; //enemy�� ����� �� ����
	SoundBuffer enemy_explosion_buffer;
	enemy_explosion_buffer.loadFromFile("./resources/sounds/rumble.flac");
	Sound enemy_explosion_sound;
	enemy_explosion_sound.setBuffer(enemy_explosion_buffer);
	int enemy_speed[ENEMY_NUM];
	for (int i = 0; i < ENEMY_NUM; i++) {
		enemy[i].setSize(Vector2f(70, 70));
		enemy[i].setPosition(rand()%300+300, rand()%385);
		enemy[i].setFillColor(Color::Yellow);
		enemy_life[i] = 1;
		enemy_speed[i] = -(rand() % 10 * 1); 
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
						enemy[i].setSize(Vector2f(70, 70));
						enemy[i].setPosition(rand() % 300 + 300, rand() % 385);
						enemy[i].setFillColor(Color::Yellow);
						enemy_life[i] = 1;
						enemy_speed[i] = -(rand() % 10 + 1);
						window.draw(enemy[i]);
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

		//���� �浹
		for (int i = 0; i < ENEMY_NUM; i++) {
			if (enemy_life[i] > 0) {

				if (player.sprite.getGlobalBounds().intersects(enemy[i].getGlobalBounds())) {
					printf("enemy[%d]�� �浹\n", i);
					enemy_life[i] -= 1;
					player.score += enemy_score;
					
					if(enemy_life[i] == 0) {
						enemy_explosion_sound.play();
					}

				}//if

				enemy[i].move(enemy_speed[i], 0);
			}//if
			
		}

		window.clear(Color::Black);
		window.draw(bg_sprite);

		for (int i = 0; i < ENEMY_NUM; i++) //enemy�� ����������� draw
		{
			if (enemy_life[i] > 0) {
				window.draw(enemy[i]);
			}
		}

		sprintf_s(player_str, "score : %d  time : %d\n",
			player.score, spent_time/1000);
		text.setString(player_str);

		//draw
		window.draw(player.sprite);
		window.draw(text);

		window.display();
		
	}//while

	return 0;
}