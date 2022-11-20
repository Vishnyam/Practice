/*
	I/O redirection : conveir < file > file &
	< input
	> output
	
	1.	execute_background &
	2.	execute_redir < > >>
	3. 	execute (background) 
	4. 	execute_conv | |
*/


#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/stat.h>
#include<sys/fcntl.h>
#include<sys/types.h>
#include<sys/wait.h>
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
					if ((double_flag) || (buf_str[0] != '\0')) {
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

int len_arr(char**pointer){
	int j = 0;
	for (j = 0; pointer[j]; ++j);
	return j;
}

int execute_conv(char **pointer, int background_flag){
	char **buf_pointer = calloc(sizeof(char*), len_arr(pointer));
	int j_buf = 0;
	int fd[2];
	for (int i = 0; pointer[i]; ++i){
		if (strcmp(pointer[i], "|")){
			buf_pointer[j_buf] = pointer[i];
			j_buf++;
		} else {
			pipe(fd);
			switch (fork()){
				case 0:
					dup2(fd[1], 1);
					close(fd[1]);
					close(fd[0]);
					if (execvp(buf_pointer[0], buf_pointer) == -1)
						printf("%s\n", "EXECUTION ERROR");
					exit(1);
				case -1:
					printf("%s\n", "CONV FORK ERROR");
					break;
				default:
					dup2(fd[0], 0);
					close(fd[1]);
					close(fd[0]);
			}
			j_buf = 0;
			free(buf_pointer);
			buf_pointer = calloc(sizeof(char*), len_arr(pointer));
		}	
	}
	switch (fork()){
		case 0:
			if (execvp(buf_pointer[0], buf_pointer) == -1)
				printf("%s\n", "EXECUTION ERROR");
			exit(1);
		case -1:
			printf("%s\n", "CONV LAST FORK ERROR");
			break;
	}
	free(buf_pointer);
	close(fd[0]);
	close(fd[1]);
	int status;
	if (background_flag)
		exit(0);
	while(wait(&status) != -1);
	return 0;
}

int execute(char **pointer, int background_flag, char *input, char *output, int output_flag){
	int status, pid;
	int fp[2], redir_fp[2]; 
	switch (pid = fork()){
		case 0:
			if (input){
				redir_fp[0] = open(input, O_RDONLY);
				dup2(redir_fp[0], 0);
				close(redir_fp[0]);
			} 
			if (output){
				if (output_flag)
					redir_fp[1] = open(output, O_WRONLY | O_CREAT | O_APPEND, 0666);
				else 
					redir_fp[1] = open(output, O_WRONLY | O_CREAT | O_TRUNC, 0666);
				dup2(redir_fp[1], 1);
				close(redir_fp[1]);
			}
			if (background_flag){
				fp[0] = open("/dev/null", O_RDONLY);
				dup2(fp[0], 0);
				close(fp[0]);
				close(fp[1]);
			}
			execute_conv(pointer, background_flag);
			exit(1);
		case -1:
			printf("%s\n", "FORK ERROR");
			break;
		default:
			wait(&status);
	}
	close(fp[0]);
	close(fp[1]);
	return 0;
}

int execute_redir(char **pointer, int background_flag){
	char **buf_pointer = calloc(sizeof(char*), len_arr(pointer) + 1);
	int j_buf = 0, output_flag = 0, jump_flag = 0;
	char *input = NULL, *output = NULL;
	for (int i = 0; pointer[i]; ++i){
		if (!strcmp(pointer[i], "<") || !strcmp(pointer[i], ">") || !strcmp(pointer[i], ">>")){
			if (!strcmp(pointer[i], "<")){
				if (pointer[i + 1]){
					input = pointer[i + 1];
				} else {
					printf("INCORRECT INPUT\n");
					return 1;
				}	
			} else {
				if (pointer[i + 1]){
					output = pointer[i + 1];
				} else {
					printf("%s\n", "INCORRECT INPUT");
					return 1;
				}
				if (!strcmp(pointer[i], ">>"))
					output_flag = 1;
			}
		jump_flag = 1;
		} else {
			if (!jump_flag){
				buf_pointer[j_buf] = pointer[i];
				j_buf++;
			}
			jump_flag = 0;
		}
	}
	if (buf_pointer[0])
		execute(buf_pointer, background_flag, input, output, output_flag);
	free(buf_pointer);
	return 0;
}

void execute_background(char **pointer){
	char **buf_pointer = calloc(sizeof(char*), len_arr(pointer) + 1);
	int j_buf = 0;
	for (int i = 0; pointer[i]; ++i){
		if (strcmp(pointer[i], "&")){
			buf_pointer[j_buf] = pointer[i];
			j_buf++;
		} else {
			execute_redir(buf_pointer, 1);
			j_buf = 0;
			free(buf_pointer);
			buf_pointer = calloc(sizeof(char*), len_arr(pointer) + 1);
		}
	}
	execute_redir(buf_pointer, 0);
	free(buf_pointer);
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
	
	while((printf("%s", "\n$ ")) && (str = get_str(fp))){
		pointer = split(str);
		if (!pointer[0]){
			printf("%s\n", "ERROR: NO COMMAND");
			continue;
		}
			
		if (!strcmp(pointer[0], "cd"))
			execute_cd(pointer);
		else
			execute_background(pointer);
		freedom(pointer);
	}
	printf("\n");
	if (argc == 2)
		fclose(fp);
	return 0;
}
