#include<stdio.h>
#include<SFML/Graphics.hpp>
using namespace sf;

int main(void) {

	RenderWindow window(VideoMode(640, 480), "AfterSchool"); // ������ â ����
	window.setFramerateLimit(60);

	RectangleShape player;
	player.setSize(Vector2f(40, 40));
	player.setPosition(100, 100);
	player.setFillColor(Color::Red);
	
	while (window.isOpen())//�����찡 ���� ���� �� ���� â ���� 
	{
		Event event;

		while (window.pollEvent(event)) {

			switch (event.type) {

			//x��ư�� ������ ������ â ����
			case Event::Closed:
				window.close(); //�����츦 ����
				break;

			}

		}//while
		
		//����Ű�� ������ �� �÷��̾� ������
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