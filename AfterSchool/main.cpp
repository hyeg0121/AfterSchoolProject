#include<stdio.h>
#include<SFML/Graphics.hpp>
#include<stdlib.h>
#include<time.h>
#include<SFML/Audio.hpp>

using namespace sf;

struct Player {
	RectangleShape sprite; //�׸��� �Ǵ� �κ�
	int speed;
	int speed_max;
	int score;
	int life;
	int width = 100, height = 100;
	float x, y; //�÷��̾� ��ǥ
};

struct Bullet {
	RectangleShape sprite;
	int is_fired;
};

struct Enemy {
	RectangleShape sprite;
	int life;
	int speed;
	int score;
};

enum item_type {
	SPEED,
	DELAY
};

struct Item {
	RectangleShape sprite;
	int delay;
	int is_presented;
	int speed;
	long presented_time;
	enum item_type type;
};

struct Textures {
	Texture bg;			
	Texture bullet;
	Texture enemy;		
	Texture gameover;	
	Texture item_delay;
	Texture item_speed;
	Texture player;		
};

struct SBuffer {
	SoundBuffer BGM;
	SoundBuffer explode;
	SoundBuffer earning_item;
	SoundBuffer bulletfiring;
};

//�Լ�
//obj1�� obj2�� �浹 ����, �浹�ϸ� 1�� ��ȯ, �浹���� ������ 0�� ��ȯ 
int is_collide(RectangleShape obj1, RectangleShape obj2) {
	if (obj1.getGlobalBounds().intersects(obj2.getGlobalBounds())) {
		return 1;
	}
	else {
		return 0;
	}
};

//��������
const int ENEMY_NUM = 10;					//enemy�� �ִ� ����
const int BULLET_NUM = 50;
const int ITEM_NUM = 2;
const int W_WIDTH = 1500, W_HEIGHT = 600;	//â�� ũ��
const int GO_WIDTH = 800, GO_HEIGHT = 600;	//gameover �׸��� ũ�� 

int main(void) {

	/* Textures */
	struct Textures t;
	t.bg.loadFromFile("./resources/images/background.jpg");
	t.gameover.loadFromFile("./resources/images/gameover.png");
	t.player.loadFromFile("./resources/images/player.png");
	t.enemy.loadFromFile("./resources/images/enemy.png");
	t.bullet.loadFromFile("./resources/images/bullet.png");
	t.item_delay.loadFromFile("./resources/images/delay.png");
	t.item_speed.loadFromFile("./resources/images/speed.png");

	/* SBuffer */
	struct SBuffer sb;
	sb.BGM.loadFromFile("./resources/sounds/bgm.ogg");
	sb.bulletfiring.loadFromFile("./resources/sounds/bulletfiring.wav");
	sb.earning_item.loadFromFile("./resources/sounds/earning_item.ogg");
	sb.explode.loadFromFile("./resources/sounds/explode.ogg");

	/* Window */
	RenderWindow window(VideoMode(W_WIDTH, W_HEIGHT), "AfterSchool"); // ������ â ����
	window.setFramerateLimit(60);

	srand(time(0));

	long start_time = clock();		//���� ���� �ð�
	long spent_time;				//���� ���� �ð�
	long fired_time = 0;			//�ֱٿ� bullet�� �߻��� �ð�
	int is_gameover = 0;			//gameover�� 1

	/* BGM */
	Sound BGM_sound;
	BGM_sound.setBuffer(sb.BGM);
	BGM_sound.setLoop(1);
	BGM_sound.play();

	/* Font */
	Font font; 
	font.loadFromFile("C:\\windows\\Fonts\\comicbd.ttf");

	/* Text */
	Text text; //score
	text.setFont(font);
	text.setCharacterSize(30);
	text.setFillColor(Color(255, 255, 255));
	text.setOutlineColor(Color::Black);
	text.setOutlineThickness(1);
	text.setPosition(0, 0);
	text.setString("score : time : life :");
	char info[50];

	/* Background */
	Sprite bg_sprite;
	bg_sprite.setTexture(t.bg);
	bg_sprite.setPosition(0, 0);

	/* gameover */
	Sprite gameover_sprite;
	gameover_sprite.setTexture(t.gameover);
	gameover_sprite.setPosition((W_WIDTH-GO_WIDTH)/2, (W_HEIGHT-GO_HEIGHT)/2);

	/* Player */
	struct Player player;
	player.sprite.setTexture(&t.player);
	player.sprite.setSize(Vector2f(player.width, player.height));
	player.sprite.setPosition(100, W_HEIGHT/2);
	player.x = player.sprite.getPosition().x;
	player.y = player.sprite.getPosition().y;
	player.speed = 5; //�÷��̾��� ������ �ӵ�
	player.speed_max = 15;
	player.score = 0; //�÷��̾��� ����
	player.life = 10;

	/* Bullet */
	int bullet_speed = 20;
	int bullet_idx = 0;
	int bullet_delay = 500;
	int bullet_delay_max = 100;
	int bullet_width = 70, bullet_height = 30;
	Sound bullet_firing_sound;
	bullet_firing_sound.setBuffer(sb.bulletfiring);

	struct Bullet bullet[BULLET_NUM];
	for (int i = 0; i < BULLET_NUM; i++) {
		bullet[i].sprite.setSize(Vector2f(bullet_width, bullet_height));
		bullet[i].sprite.setPosition(player.x + 90, player.y + 50); //�ӽ� �׽�Ʈ
		bullet[i].is_fired = 0;
		bullet[i].sprite.setTexture(&t.bullet);
	}

	/* Enmey */
	int enemy_width = 100, enemy_height = 100;
	Sound enemy_explosion_sound;
	enemy_explosion_sound.setBuffer(sb.explode);
	int enemy_respawn_time = 5;
	struct Enemy enemy[ENEMY_NUM];
	for (int i = 0; i < ENEMY_NUM; i++) {
		enemy[i].sprite.setSize(Vector2f(enemy_width, enemy_height));	
		enemy[i].score = rand() % 100 +1;
		enemy[i].life = 1;
		enemy[i].speed = -(rand() % 10 * 1);
		enemy[i].sprite.setPosition(rand() % 300 + W_WIDTH * 0.8, rand() % W_HEIGHT * 0.8);
		enemy[i].sprite.setTexture(&t.enemy);
	}

	/* item */
	struct Item item[ITEM_NUM];
	Sound earning_item_sound;
	earning_item_sound.setBuffer(sb.earning_item);
	/* �̵��ӵ� */
	item[0].sprite.setTexture(&t.item_speed);
	item[0].delay = 15000;
	item[0].type = SPEED;
	/* ���ݼӵ� */
	item[1].sprite.setTexture(&t.item_delay);
	item[1].delay = 25000;
	item[1].type = DELAY;

	for (int i = 0; i < 2; i++) {
		item[i].is_presented = 0;
		item[i].sprite.setSize(Vector2f(70, 70));
		item[i].speed = 10;
	}

	//���α׷� ���� ��
	while (window.isOpen())//�����찡 ���� ���� �� ���� â ���� 
	{
		Event event;

		//(x)��ư�� ������ ���� ����
		while (window.pollEvent(event)) {

			switch (event.type) {

			//x��ư�� ������ ������ â ����
			case Event::Closed:
				window.close(); //�����츦 ����
				break;

			}//switch

		}//while


		/* spent time */
		spent_time = clock() - start_time;

		/* player update */
		//����Ű�� ������ �� �÷��̾� ������
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
		}//����Ű end

		/* �÷��̾� ��ġ ���ϱ� */
		player.x = player.sprite.getPosition().x;
		player.y = player.sprite.getPosition().y;

		/* bullet update */
		//�Ѿ� �߻� 
		if (Keyboard::isKeyPressed(Keyboard::Space)) {
			//bullet delay
			if (spent_time - fired_time > bullet_delay) {
				// �Ѿ��� �߻�Ǿ����� �ʴٸ�
				if (!bullet[bullet_idx].is_fired)
				{
					bullet_firing_sound.play();
					bullet[bullet_idx].sprite.setPosition(player.x + 50, player.y + 15);
					bullet[bullet_idx].is_fired = 1;
					bullet[bullet_idx].sprite.setPosition(player.x + 50, player.y + 15);
					bullet[bullet_idx].is_fired = 1;
					bullet_idx++;	// �����Ѿ��� �߻��� �� �ֵ���

					if (bullet_idx == 49) { //��� �߻��� �� �ֵ���
						bullet_idx = 0;
					}

					fired_time = spent_time; //���� �ð�
				}
			}
			
		}

		//bullet�� ȭ�� ���� �����ϸ� �ٽ� �߻� ����
		for (int i = 0; i < BULLET_NUM; i++) {
			if (bullet[i].sprite.getPosition().x >= W_WIDTH) {
				bullet[i].is_fired = 0;
				bullet[i].sprite.setPosition(player.x + 70, player.y + 30);
			}
			//�Ѿ� ������
			if (bullet[i].is_fired) {
				bullet[i].sprite.move(bullet_speed, 0);
			}
		}



		/* enemy update*/
		for (int i = 0; i < ENEMY_NUM; i++) {

			//10�ʸ��� enemy�� ��
			if (spent_time % (1000*enemy_respawn_time) < 1000 / 60 + 1) {
				enemy[i].sprite.setSize(Vector2f(enemy_width, enemy_height));
				enemy[i].sprite.setPosition(rand() % W_WIDTH * 0.8 + 900, rand() % W_HEIGHT * 0.8 );
				enemy[i].life = 1;
				enemy[i].speed = -(rand() % 10 + 1 + spent_time%(1000*enemy_respawn_time));
				window.draw(enemy[i].sprite);
			}

			for (int j = 0; j < BULLET_NUM; j++) {
				//bullet�� enemy, player�� enemy�� �浹
				if (enemy[i].life > 0) {
					// TODO : �Ѿ��� �����ϴ� ���� ����
					if (is_collide(bullet[j].sprite, enemy[i].sprite))
					{
						printf("enemy[%d]�� �浹\n", i);
						enemy[i].life -= 1;
						player.score += enemy[i].score;
						bullet[j].is_fired = 0;
						bullet[j].sprite.setPosition(player.x + 70, player.y + 30);
					
						if(enemy[i].life == 0) {
							enemy_explosion_sound.play();
						}

					}//if
					//���� ���� ���� �����Ϸ��� ����
					else if (enemy[i].sprite.getPosition().x < 0) 
					{ 
						player.life -= 1;
						enemy[i].life = 0;
					}//else if


				}//if
			}//for j

			//�� ������
			enemy[i].sprite.move(enemy[i].speed, 0);		
		}//for i



		/* item update */
		for (int i = 0; i < 2; i++) 
		{
			//���� �ð��� ���� �� item ����
			if (spent_time % item[i].presented_time > item[i].delay)
			{
				item[i].sprite.setPosition(rand() % 300 + W_WIDTH * 0.9, rand() % 500);
				item[i].is_presented = 1;
			}
			if (item[i].is_presented) 
			{
				//item ������
				item[i].sprite.move(-item[i].speed, 0);

				//player�� item�� ȹ���� ���
				if (is_collide(player.sprite, item[i].sprite))
				{
					switch (item[i].type)
					{
					case SPEED: //�̵��ӵ�
						player.speed += 2;
						if (player.speed > player.speed_max)
							player.speed = player.speed_max;
						break;
					case DELAY: //���ݼӵ�
						bullet_delay -= 100;
						if (bullet_delay > bullet_delay_max)
							bullet_delay = bullet_delay_max;
						break;
					}

					item[i].is_presented = 0;
					item[i].presented_time = spent_time;
					earning_item_sound.play();
				}
			}

		}


		/* window update */
		window.clear(Color::Black);
		window.draw(bg_sprite);

		for (int i = 0; i < ENEMY_NUM; i++) //enemy�� ����������� draw
		{
			if (enemy[i].life > 0) 
			{
				window.draw(enemy[i].sprite);
			}
		}

		for (int i = 0; i < 2; i++) {
			if (item[i].is_presented) 
			{
				window.draw(item[i].sprite);
			}
		}
		if (player.life <= 0)
		{
			is_gameover = 1;
		}

		/* score */
		sprintf_s(info, "score : %d  time : %d  life : %d\n",
			player.score, spent_time/1000,player.life);
		text.setString(info);

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
			// TODO : ������ ���ߴ� ���� ������ ��
		}
		window.display();
		
	}//while

	return 0;
}