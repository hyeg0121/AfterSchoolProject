#include<stdio.h>
#define _CRT_SECURE_NO_WARNINGS 

int main(void) 
{
	char str1[30] = "score : ";
	char str2[30] = "300";
	
	strcat(str1, str2);

	printf("%s", str1);

	return 0;
}
