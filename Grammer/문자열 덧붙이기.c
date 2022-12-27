#include<stdio.h>
//#define _CRT_SECURE_NO_WARNINGS 

int main(void) {
	char str1[30] = "score : ";
	char str2[30] = "300";
	int score = 300;
	char score_str[100];

	int i = 0, j = 0;

	while (str1[i] != '\0') {
		i++;
	}//str1의 끝 인덱스 찾기 

	while (str2[j] != '\0') {
		str1[i] = str2[j];
		i++, j++;
	}//str1 에 str2붙이기

	str1[i] = '\0'; //마지막에 null
	
	//strcat(str1, str2);
	printf("%s", str1);
	return 0;
}

//char str[30] = "score : ";
//// str의 글자 수 구하기
//int str_len = 0;
//while (str[str_len] != '\0')
//str_len++;
//
//char str2[30] = "300";
//// str2의 글자 수 구하기
//int str2_len = 0;
//while (str[str_len] != '\0')
//str2_len++;
//
//for (int i = 0; i < str2_len; i++)
//	str[i + str_len] = str2[i];
//str[str_len + str2_len] = '\0'; // 문자의 끝엔 항상 null 값
//printf("%s", str);
//
//int score = 300;
//
//printf("%s\n", score);
