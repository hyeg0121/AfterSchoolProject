#include<stdio.h>

//함수의 구성 요소 : 반환형 함수이름(매개변수){}
int f(int x) {
	return 2 * x + 1;
}

int main(void) {
	printf("%d\n", f(1));
	return 0;
}