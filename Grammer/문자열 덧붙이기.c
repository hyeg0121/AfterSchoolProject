#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define _CRT_SECURE_NO_WARNINGS 

int main(void) 
{
	char str1[30] = "score : ";
	int score = 300;

	char str2[30];
	itoa(score, str2, 10);

	strcat(str1, str2);

	printf("%s", str1);

	return 0;
}
