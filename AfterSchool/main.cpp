#include<stdio.h>
#include<SFML/Graphics.hpp>
using namespace sf;

int main(void) {

	RenderWindow window(VideoMode(640, 480), "AfterSchool"); // ������ â ����

	while (window.isOpen())//�����찡 ���� ���� �� ���� â ���� 
	{
		Event event;
		while (window.pollEvent(event)) {

			if (event.type == Event::Closed) {
				window.close();
			}//x��ư�� ������ ������ â ����
		}
	}

	return 0;
}