/*If there are several strings that occur equally often? an early one is output*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MEM_SIZE 10

char *get_str(FILE *F){
	int len, k = 0;
	char *str = malloc(sizeof(char) * MEM_SIZE);
	if(!str){
		free(str);
		return NULL;
	}
	while(fgets(str + k, MEM_SIZE, F)){
		len = strlen(str);
		if(str[len - 1] != '\n'){
			k = k + MEM_SIZE - 1;
			str = realloc(str, sizeof(char) * (k + MEM_SIZE));
			if(!str){
				free(str);
				return NULL;
			}
		} else {
			str[len-1] = '\0';
			return str;
		}
	}
	free(str);
	return NULL;
}


int main(int argc,char **argv){
	FILE *f;
	f = fopen(argv[1], "r");
	if(f == NULL) return 0;
	char *str1, *str2, *res;
	if((str1 = get_str(f)) == NULL){
		printf("%s\n", "Empty file");
		fclose(f);
		return 0;
	}
	fseek (f, 0, SEEK_SET);
	int max = 0, curr = 0;
	long offset = 0;
	while ((str1 = get_str(f)) != NULL){
		offset = ftell(f);
		fseek (f, 0, SEEK_SET);
		while ((str2 = get_str(f)) != NULL){
			if (!strcmp(str1, str2))
				curr++;
			free(str2);
		}
		if(curr > max){
			max = curr;
			if(!res) 
				free(res);
			res = malloc(sizeof(char) * (strlen(str1) + 1));
			strcpy(res, str1);
		}
		fseek (f, offset, SEEK_SET);
		free(str1);
		curr = 0;
	}
	printf("The most common string is: %s\n", res);
	free(res);
	fclose(f);	
	return 0;
}
