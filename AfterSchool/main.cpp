#include<stdio.h>
#include<SFML/Graphics.hpp>
#include<stdlib.h>
#include<time.h>
#include<SFML/Audio.hpp>

using namespace sf;

struct Player {
	RectangleShape sprite; //�׸��� �Ǵ� �κ�
	Texture texture;
	Texture* texture_pointer;
	int speed;
	int score;
	int life;
};

struct Enemy {
	RectangleShape sprite;
	Texture texture;
	Texture* texture_pointer;
	const int NUM = 10;
	int life;
	int speed;
	int score;
	SoundBuffer explosion_buffer;
	Sound explosion_sound;
	int respawn_time;

};

//��������
const int ENEMY_NUM = 10;					//enemy�� �ִ� ����
const int W_WIDTH = 1200, W_HEIGHT = 480;	//â�� ũ��
const int GO_WIDTH = 480, GO_HEIGHT = 360;	//gameover �׸��� ũ�� 

int main(void) {

	RenderWindow window(VideoMode(W_WIDTH, W_HEIGHT), "AfterSchool"); // ������ â ����
	window.setFramerateLimit(60);

	srand(time(0));

	long start_time = clock(); //���� ���� �ð�
	long spent_time;//���� ���� �ð�
	int is_gameover = 0;

	//BGM
	SoundBuffer BGM_buffer;
	BGM_buffer.loadFromFile("./resources/sounds/bgm.ogg");
	Sound BGM_sound;
	BGM_sound.setBuffer(BGM_buffer);
	BGM_sound.setLoop(1);
	BGM_sound.play();

	Font font; 
	font.loadFromFile("C:\\windows\\Fonts\\comicbd.ttf");

	Text text; //score
	text.setFont(font);
	text.setCharacterSize(30);
	text.setFillColor(Color(255, 255, 255));
	text.setOutlineColor(Color::Black);
	text.setOutlineThickness(1);
	text.setPosition(0, 0);
	text.setString("score : time : life :");

	Texture bg_texture;
	bg_texture.loadFromFile("./resources/images/background.jpg");
	Sprite bg_sprite;
	bg_sprite.setTexture(bg_texture);
	bg_sprite.setPosition(0, 0);

	//gameover
	Texture gameover_texture;
	gameover_texture.loadFromFile("./resources/images/gameover.png");
	Sprite gameover_sprite;
	gameover_sprite.setTexture(gameover_texture);
	gameover_sprite.setPosition((W_WIDTH-GO_WIDTH)/2, (W_HEIGHT-GO_HEIGHT)/2);

	//Player
	struct Player player;
	player.sprite.setSize(Vector2f(70, 70));
	player.sprite.setPosition(100, 100);
	player.texture.loadFromFile("./resources/images/player.png");
	player.texture_pointer = &player.texture;
	player.sprite.setTexture(player.texture_pointer);
	player.speed = 5; //�÷��̾��� ������ �ӵ�
	player.score = 0; //�÷��̾��� ����
	player.life = 3;
	char player_str[50];

	//Enemy
	struct Enemy enemy[ENEMY_NUM];
	for (int i = 0; i < ENEMY_NUM; i++) {

		// TODO : ������ ��ȿ������ �ڵ��̹��� ���߿� refactoring
		enemy[i].explosion_buffer.loadFromFile("./resources/sounds/rumble.flac");
		enemy[i].explosion_sound.setBuffer(enemy[i].explosion_buffer);
		enemy[i].score = 100;
		enemy[i].respawn_time = 10;
		enemy[i].life = 1;
		enemy[i].speed = -(rand() % 10 * 1);

		enemy[i].sprite.setSize(Vector2f(70, 70));
		enemy[i].sprite.setPosition(rand()%300+W_WIDTH*0.9, rand()%385);
		enemy[i].texture.loadFromFile("./resources/images/enemy.png");
		enemy[i].texture_pointer = &enemy[i].texture;
		enemy[i].sprite.setTexture(enemy[i].texture_pointer);
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
						enemy[i].sprite.setPosition(rand() % 300 + W_WIDTH * 0.9, rand() % 385);
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

			//10�ʸ��� enemy�� ��
			if (spent_time % (1000*enemy[i].respawn_time) < 1000 / 60 + 1) {
				enemy[i].sprite.setSize(Vector2f(70, 70));
				enemy[i].sprite.setPosition(rand() % 300 + W_WIDTH * 0.9, rand() % 385);
				enemy[i].life = 1;
				enemy[i].speed = -(rand() % 10 + 1 + spent_time%(1000*enemy[i].respawn_time));
				window.draw(enemy[i].sprite);
			}

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

		if (player.life <= 0)
		{
			is_gameover = 1;
		}


		sprintf_s(player_str, "score : %d  time : %d  life : %d\n",
			player.score, spent_time/1000,player.life);
		text.setString(player_str);

		//draw
		window.draw(player.sprite);
		window.draw(text);

		if (is_gameover) {
			window.draw(gameover_sprite);
			
		}
		window.display();
		
	}//while

	return 0;
}