#include<stdio.h>
//#define _CRT_SECURE_NO_WARNINGS 

int main(void) {
	char str1[30] = "score : ";
	char str2[30] = "300";
	int score = 300;

	int i = strlen(str1), j = 0;

	while (str2[j] != '\0') {
		str1[i] = str2[j];
		i++, j++;
	}//str1 에 str2붙이기

	str1[i] = '\0'; //마지막에 null
	
	printf("%s", str1);
	return 0;
}
