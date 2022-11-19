#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
#define MEM_SIZE 10

char *get_str(FILE *fp){
	int len, k = 0;
	char *str = malloc(sizeof(char) * MEM_SIZE);
	if(!str){
		free(str);
		return NULL;
	}
	while(fgets(str + k, MEM_SIZE, fp)){
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

void out_str(char **pointer){
	if(pointer){
		for (int j = 0; pointer[j]; ++j)
			if (pointer[j]) printf("%s\n", pointer[j]);
	}
	
}

void freedom(char **pointer){
	if(pointer){
		for (int j = 0; pointer[j]; ++j)
		if (pointer[j]) free(pointer[j]);
	free(pointer);
	}
}

char **split(char *str){
	char *buf_str;
	char **pointer = malloc(sizeof(char*));
	int count_pointer = 0, double_flag = 0, quotes_flag = 0;
	buf_str = calloc(strlen(str) + 1, sizeof(char));
	int j_buf = 0;
	for (int i = 0; i < strlen(str); ++i){
		if (str[i] == '"'){
			if (quotes_flag)
				quotes_flag = 0;
			else 
				quotes_flag = 1;
			j_buf++;
			double_flag = 0;	
		} else if (quotes_flag)
				buf_str[i - j_buf] = str[i];
			else {
				if (str[i] == ' '){
					double_flag = 0;
					if ((i > 0) && (str[i - 1] != ' ')){
						buf_str[i - j_buf] = '\0';
						if (buf_str[0]){
							pointer[count_pointer] = buf_str;
							buf_str = calloc(strlen(str) + 1, sizeof(char));
							count_pointer++;
							pointer = realloc(pointer, sizeof(char*) * (count_pointer + 1));
						}
						
					}
					j_buf = i + 1;
					
				} else if ((str[i] == '&') || (str[i] == '|') || (str[i] == '>')){
					if (double_flag){
						double_flag = 0;
						if (str[i] == str[i - 1]){
							buf_str[i - j_buf] = str[i];
							buf_str[i - j_buf + 1] = '\0';
							j_buf = i + 1;
							pointer[count_pointer] = buf_str;
							buf_str = calloc(strlen(str) + 1, sizeof(char));
							count_pointer++;
						} else {
							buf_str[i - j_buf] = '\0';
							j_buf = i;
							pointer[count_pointer] = buf_str;
							buf_str = calloc(strlen(str) + 1, sizeof(char));
							count_pointer++;
							buf_str[i - j_buf] = str[i];
							double_flag = 1;
						}
						pointer = realloc(pointer, sizeof(char*) * (count_pointer + 1));
					} else {
						if (buf_str[0] != '\0'){
							buf_str[i - j_buf] = '\0';
							j_buf = i;
							pointer[count_pointer] = buf_str;
							buf_str = calloc(strlen(str) + 1, sizeof(char));
							count_pointer++;
							pointer = realloc(pointer, sizeof(char*) * (count_pointer + 1));
						}
						buf_str[i - j_buf] = str[i];
						double_flag = 1;
					}

				} else if ((str[i] == ';') || (str[i] == '<') || (str[i] == '(') || (str[i] == ')')) {
					if (double_flag || (buf_str)) {
						double_flag = 0;
						buf_str[i - j_buf] = '\0';
						j_buf = i;
						pointer[count_pointer] = buf_str;
						buf_str = calloc(strlen(str) + 1, sizeof(char));
						count_pointer++;
						pointer = realloc(pointer, sizeof(char*) * (count_pointer + 1));
					}
					buf_str[i - j_buf] = str[i];
					buf_str[i - j_buf + 1] = '\0';
					j_buf = i + 1;
					pointer[count_pointer] = buf_str;
					buf_str = calloc(strlen(str) + 1, sizeof(char));
					count_pointer++;
					pointer = realloc(pointer, sizeof(char*) * (count_pointer + 1));
				} else {
					if (double_flag) {
						double_flag = 0;
						buf_str[i - j_buf] = '\0';
						j_buf = i;
						pointer[count_pointer] = buf_str;
						buf_str = calloc(strlen(str) + 1, sizeof(char));
						count_pointer++;
						pointer = realloc(pointer, sizeof(char*) * (count_pointer + 1));
					}
					buf_str[i - j_buf] = str[i];
				}
			}	
	}
	if (buf_str){
		if (buf_str[0] != '\0'){
			buf_str[strlen(str) - j_buf] = '\0';
			pointer[count_pointer] = buf_str;
			count_pointer++;
			pointer = realloc(pointer, sizeof(char*) * (count_pointer + 1));
		} else 
			free(buf_str);
	}
	if (quotes_flag){
		freedom(pointer);
		printf("%s\n", "ERROR: UNCORRECT USE OF QUOTATION MARKS");
		return NULL;
	}
	double_flag = 0;
	quotes_flag = 0;
	free(str);
	pointer[count_pointer] = NULL;
	return pointer;
}

void execute(char **pointer){
	int pid = fork(), status;
	if (pid == 0){
		if (execvp(pointer[0], pointer) == -1)
			printf("%s\n", "EXECUTION ERROR");
		exit(1);
	}
	else if (pid < 0) 
		printf("%s\n", "FORK ERROR");
	else 
		wait(&status);
}

void execute_cd(char **pointer){
	if (!pointer[1])
		chdir(getenv("HOME"));
	else {
		if (chdir(pointer[1]))
			printf("%s\n", "CD ERROR: -1");
	}
}

int main(int argc, char **argv){
	char **pointer;
	char *str;
	FILE *fp;
	if (argc == 1)
		fp = stdin;
	else if (argc == 2){
		fp = fopen(argv[1], "r");
		if (!fp)
			return 0;
	} else {
		printf("%s\n", "ERROR: UNCORRECT INPUT FILES");
		return 0;
	}
	printf("%s", "$ ");
	while((str = get_str(fp))){
		pointer = split(str);
		if (!pointer)
			printf("%s\n", "ERROR: NO COMMAND");
		if (!strcmp(pointer[0], "cd"))
			execute_cd(pointer);
		else
			execute(pointer);
		freedom(pointer);
		printf("%s", "$ ");
	}	
	if (argc == 2)
		fclose(fp);
	return 0;
}
