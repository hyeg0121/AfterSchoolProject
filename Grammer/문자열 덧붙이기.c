#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define _CRT_SECURE_NO_WARNINGS 

int main(void) 
{
	char str1[30] = "score : ";
	int score = 300;

	char str[30];
	sprintf(str, "score : %d ", score);

	printf("%s", str);

	return 0;
}
