#include<stdio.h>
#include<SFML/Graphics.hpp>
using namespace sf;

int main(void) {

	RenderWindow window(VideoMode(640, 480), "AfterSchool"); // 윈도우 창 생성

	while (window.isOpen())//윈도우가 열려 있을 때 까지 창 유지 
	{
		Event event;
		while (window.pollEvent(event)) {

			if (event.type == Event::Closed) {
				window.close();
			}//x버튼을 누르면 윈도우 창 종료
		}
	}

	return 0;
}