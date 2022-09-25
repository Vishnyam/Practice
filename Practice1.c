/*
	������ �������� �� ctrl+d; ��� ������ ������ ��������� � ������; 
	������� ����� ��������� ���� �����/�����
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define memory 10

char *get_str(void); //������� ����� ������
void out_str(char**pointer, int i); //������� ������ ������ 
int compare(char* str1, char* str2); //������� ��������� �����
void sort(char **pointer, int n);//���������� �������� ���������
void freedom(char**pointer, int i);//������� ������������ ������

int compare(char* str1, char* str2){
	int minlen, flag = 0;
	if (strlen(str1) > strlen(str2)) minlen = strlen(str2);
	else minlen = strlen(str1);
	for (int i = 0; i < minlen; ++i)
		if (str1[i] != str2[i]){
			if (str1[i] > str2[i]) return 1;
			flag = 1;
			break;
		}
	if ((strlen(str2) == minlen) && (flag == 0)) return 1;
	return 0;
}


void sort(char **pointer, int n){
	for (int j = 1; j < n; ++j){
		char *key = pointer[j];
		int i = j - 1;
		while((i >= 0) && (compare(pointer[i], key))){
			pointer[i + 1] = pointer[i];
			i--;
		}
		pointer[i + 1] = key;
	}
}


char *get_str(void){
	int len, k = 0;
	char *str = malloc(sizeof(char*) * memory);
	if(!str) return NULL;
	while(fgets(str + k, memory, stdin)){
		len = strlen(str);
		if(str[len - 1] != '\n'){
			k = k + memory - 1;
			str = (char*)realloc(str, sizeof(char*) * (k + memory));
			if(!str) return NULL;
		}
		else{
			str[len-1] = '\0';
			return str;
		}
		}
	free(str);
	return NULL;
}


void out_str(char**pointer, int i){
	for (int j = 0; j < i; ++j)
		if (pointer[j]) printf("%s\n", pointer[j]);
}



void freedom(char**pointer, int i){
	for (int j = 0; j < i + 1; ++j)
		free(pointer[j]);
	free(pointer);
}	


int main(){
	int i = 0;
	printf("%s\n","Enter strings and ctrl+D after that:");
	char **pointer = malloc(sizeof(char**) * 1);
	while ((pointer[i] = get_str())){
		i++;
		pointer = realloc(pointer, sizeof(char**) * (i + 2) );
		if(!pointer) return 0;
	}

	sort(pointer, i);
	printf("\n%s\n","Sorted strings:");
	out_str(pointer, i);
	freedom(pointer, i);
	return 0;
}

