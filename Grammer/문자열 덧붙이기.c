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
	}//str1�� �� �ε��� ã�� 

	while (str2[j] != '\0') {
		str1[i] = str2[j];
		i++, j++;
	}//str1 �� str2���̱�

	str1[i] = '\0'; //�������� null
	
	//strcat(str1, str2);
	printf("%s", str1);
	return 0;
}

//char str[30] = "score : ";
//// str�� ���� �� ���ϱ�
//int str_len = 0;
//while (str[str_len] != '\0')
//str_len++;
//
//char str2[30] = "300";
//// str2�� ���� �� ���ϱ�
//int str2_len = 0;
//while (str[str_len] != '\0')
//str2_len++;
//
//for (int i = 0; i < str2_len; i++)
//	str[i + str_len] = str2[i];
//str[str_len + str2_len] = '\0'; // ������ ���� �׻� null ��
//printf("%s", str);
//
//int score = 300;
//
//printf("%s\n", score);
