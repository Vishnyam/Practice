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
int len_arr(char**pointer){
	int j = 0;
	for (j = 0; pointer[j]; ++j);
	return j;
}

void freedom(char **pointer){
	if(pointer){
		for (int j = 0; pointer[j]; ++j)
		if (pointer[j]) free(pointer[j]);
	free(pointer);
	}
}

char **split(char *str){
//	printf("HELLO WORLD!!!!!!!!!!!!!!!!!!!!!!");
	char *buf_str;
	char **pointer = malloc(sizeof(char*));
	int count_pointer = 0, double_flag = 0, quotes_flag = 0;
	buf_str = calloc(strlen(str) + 1, sizeof(char));
//	printf("HELLO WORLD 1");
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
					if ((i > 0) && (str[i - 1] != ' ') && (buf_str[0])){
					//	buf_str[i - j_buf] = '\0';
					//	if (buf_str[0]){
							pointer[count_pointer] = buf_str;
							buf_str = calloc(strlen(str) + 1, sizeof(char));
//							printf("HELLO WORLD 2");
							count_pointer++;
							pointer = realloc(pointer, sizeof(char*) * (count_pointer + 1));
					//	}
						
					}
					j_buf = i + 1;
					
				} else if ((str[i] == '&') || (str[i] == '|') || (str[i] == '>')){
					if (double_flag){
						double_flag = 0;				
						if (str[i] == str[i - 1]){
							if((i < strlen(str) - 1) && (str[i] == '|') && (str[i + 1] == '|')){
							} else {
								buf_str[i - j_buf] = str[i];
							//	buf_str[i - j_buf + 1] = '\0';
								j_buf = i + 1;
								pointer[count_pointer] = buf_str;
								buf_str = calloc(strlen(str) + 1, sizeof(char));
						//		count_pointer++;
							}
						} else {
						//	buf_str[i - j_buf] = '\0';
							j_buf = i;
							pointer[count_pointer] = buf_str;
							buf_str = calloc(strlen(str) + 1, sizeof(char));
		
							buf_str[i - j_buf] = str[i];
							double_flag = 1;
						}
						count_pointer++;
						pointer = realloc(pointer, sizeof(char*) * (count_pointer + 1));
					} else {
						if (buf_str[0] != '\0'){
					//		buf_str[i - j_buf] = '\0';
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
					//	buf_str[i - j_buf] = '\0';
						j_buf = i;
						pointer[count_pointer] = buf_str;
						buf_str = calloc(strlen(str) + 1, sizeof(char));
						count_pointer++;
						pointer = realloc(pointer, sizeof(char*) * (count_pointer + 1));
					}
					buf_str[i - j_buf] = str[i];
					//buf_str[i - j_buf + 1] = '\0';
					j_buf = i + 1;
					pointer[count_pointer] = buf_str;
					buf_str = calloc(strlen(str) + 1, sizeof(char));
//					printf("HELLO WORLD 3");
					count_pointer++;
					pointer = realloc(pointer, sizeof(char*) * (count_pointer + 1));
				} else {
					if (double_flag) {
						double_flag = 0;
					//	buf_str[i - j_buf] = '\0';
						j_buf = i;
						pointer[count_pointer] = buf_str;
						buf_str = calloc(strlen(str) + 1, sizeof(char));
						count_pointer++;
						pointer = realloc(pointer, sizeof(char*) * (count_pointer + 1));
//						printf("HELLO WORLD 3");
					}
					buf_str[i - j_buf] = str[i];
				}
			}	
	}
	if (buf_str){
		if (buf_str[0] != '\0'){
		//	buf_str[strlen(str) - j_buf] = '\0';
			pointer[count_pointer] = buf_str;
			count_pointer++;
			pointer = realloc(pointer, sizeof(char*) * (count_pointer + 1));
//			printf("HELLO WORLD 4");
		} else 
			free(buf_str);
	}
	if (quotes_flag){
		freedom(pointer);
		printf("%s\n", "ERROR: UNCORRECT USE OF QUOTATION MARKS");
//		printf("HELLO WORLD 5");
		return NULL;
	}
	double_flag = 0;
	quotes_flag = 0;
	pointer[count_pointer] = NULL;
//	printf("HELLO WORLD");
	return pointer;
}


char **split_bracket(char *str){
	char *buf_str = calloc(strlen(str) + 1, sizeof(char));
	char **pointer = malloc(sizeof(char*));
	int j = 0, j_buf = 0, bracket_count = 0, bracket_flag = 0;
	
	for (int i = 0; i < strlen(str); ++i) {
    	if (str[i] == '(') {                        
			if ((!bracket_flag) && buf_str[0]) {
		        j_buf = i;
		        pointer[j] = buf_str;
		        buf_str = calloc(strlen(str) + 1, sizeof(char));
		        j++;
		        pointer = realloc(pointer, sizeof(char*) * (j + 1));
      		}
		      bracket_count ++;
		      bracket_flag = 1;
		      buf_str[i - j_buf] = str[i];
    	} else if (str[i] == ')') {          

		      bracket_count --;
		      if (!bracket_flag) {
		        free(buf_str);
		        freedom(pointer);
		        return NULL;
		      }
		      buf_str[i - j_buf] = str[i];
		      if (!bracket_count) {
		        bracket_flag = 0;
		        j_buf = i + 1;
		        pointer[j] = buf_str;
		        buf_str = calloc(strlen(str) + 1, sizeof(char));
		        j++;
		        pointer = realloc(pointer, sizeof(char*) * (j + 1));
		      }
   		 } else 
      		buf_str[i - j_buf] = str[i];
	}

	if (buf_str)
		if (buf_str[0]) {
			pointer[j] = buf_str;
			j++;
			pointer = realloc(pointer, sizeof(char*) * (j + 1));
			if (!buf_str) {
				free(pointer);
				return NULL;
			}
		} else 
			free(buf_str);
	if (bracket_count) {
		freedom(pointer);
		return NULL;
	}
	pointer[j] = NULL;
	int count = 0;
	char **final_pointer;
	for (int i = 0; pointer[i]; i ++){
		if (pointer[i][0] != '(') {
			char **new_pointer = split(pointer[i]);
		    if (!count) 
				final_pointer = malloc(sizeof(char*) * (count + len_arr(new_pointer) + 1));
		    else 
				final_pointer = realloc(final_pointer, sizeof(char*) * (count + len_arr(new_pointer) + 1));
		    	memcpy(final_pointer + count, new_pointer, (len_arr(new_pointer) + 1) * sizeof(char*));
		    	count += len_arr(new_pointer);
		} else {
	
			if (!count) 
				final_pointer = malloc(sizeof(char *) * (count + 2));
			else 
				final_pointer = realloc(final_pointer, sizeof(char *) * (count + 2));
			final_pointer[count] = pointer[i];
			count ++;
			final_pointer[count] = NULL;
		}
	}
	return final_pointer;
}

int execute_conv(char **pointer, int background_flag){
//	out_str(pointer);
//	printf("---");
	char **buf_pointer = calloc(sizeof(char*), len_arr(pointer) + 1);
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
				//	out_str(buf_pointer);
					if (execvp(buf_pointer[0], buf_pointer) == -1)
						printf("%s\n", "EXECUTION ERROR CONV 1");
					exit(1);
				case -1:
					printf("%s\n", "CONV FORK ERROR");
					free(buf_pointer);
					close(fd[0]);
					close(fd[1]);
					return 1;
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
				printf("%s\n", "EXECUTION ERROR CONV 2");
			exit(1);
		case -1:
			free(buf_pointer);
			close(fd[0]);
			close(fd[1]);
			printf("%s\n", "CONV LAST FORK ERROR");
			return 1;
	}
	free(buf_pointer);
	close(fd[0]);
	close(fd[1]);
	int status;
	if (background_flag)
		exit(0);
	while(wait(&status) != -1);
	return status;
}

int execute(char **pointer, int background_flag, char *input, char *output, int output_flag){
	int status, pid, stat;
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
			stat = execute_conv(pointer, background_flag);
			if (stat)
				exit(1);
			exit(0);
		case -1:
			printf("%s\n", "FORK ERROR");
			break;
		default:
			wait(&status);
	}
	close(fp[0]);
	close(fp[1]);
	return status;
}

int execute_redir(char **pointer, int background_flag){
	char **buf_pointer = calloc(sizeof(char*), len_arr(pointer) + 1);
	int j_buf = 0, output_flag = 0, jump_flag = 0, stat;
	char *input = NULL, *output = NULL;
	for (int i = 0; pointer[i]; ++i){
		if (!strcmp(pointer[i], "<") || !strcmp(pointer[i], ">") || !strcmp(pointer[i], ">>")){
			if (!strcmp(pointer[i], "<")){
				if (pointer[i + 1]){
					input = pointer[i + 1];
				} else {
					printf("INCORRECT INPUT\n");
					free(buf_pointer);
					return 1;
				}	
			} else {
				if (pointer[i + 1]){
					output = pointer[i + 1];
				} else {
					printf("%s\n", "INCORRECT INPUT");
					free(buf_pointer);
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
		stat = execute(buf_pointer, background_flag, input, output, output_flag);
	free(buf_pointer);
	return stat;
}

int execute_bracket(char **pointer, int background_flag){
	char **buf_pointer;
	char *buf_str;
	if (pointer && pointer[0] && pointer[0][0] == '(') {
    	if (pointer[1]) 
      		return 1;
	    buf_str = pointer[0] + 1;
	    buf_str[strlen(pointer[0]) - 2] = '\0';
	    buf_pointer = calloc(sizeof(char*), 4);
	    buf_pointer[0] = "./a.out";
	    buf_pointer[1] = "stdin";
	    buf_pointer[2] = buf_str;
//	    printf("__________");
//	    out_str(buf_pointer);
//	    printf("__________");
	    return execute(buf_pointer, background_flag, NULL, NULL, 0);
	} else
		return execute_redir(pointer, background_flag);
}

int execute_cond(char **pointer, int background_flag){
	char **buf_pointer = calloc(sizeof(char*), len_arr(pointer) + 1);
	int j_buf = 0, stat = 0, symbol_flag = -1, i;
	for (i = 0; pointer[i]; ++i){
		if (!strcmp(pointer[i],"&&") || !strcmp(pointer[i],"||")){
			if (!strcmp(pointer[i],"&&")) 
				symbol_flag = 0;
			else 
				symbol_flag = 1;
			stat = execute_bracket(buf_pointer, background_flag);
		//	printf("%d\n", stat);
			free(buf_pointer);
			buf_pointer = NULL;
			break;	
		} else {
			buf_pointer[j_buf] = pointer[i];
			j_buf++;
		}
	}
	if (buf_pointer && buf_pointer[0])
		stat = execute_bracket(buf_pointer, background_flag);
	if (pointer[i] && pointer[i + 1])
		if (!symbol_flag)
			stat = stat || execute_cond(pointer + i + 1, background_flag);
		else
			stat = stat && execute_cond(pointer + i + 1, background_flag);
	if(buf_pointer)
		free(buf_pointer);
	return stat;
}

int execute_background(char **pointer){
	char **buf_pointer = calloc(sizeof(char*), len_arr(pointer) + 1);
	int j_buf = 0, stat = 0, status = 0;
	for (int i = 0; pointer[i]; ++i){
		if (strcmp(pointer[i], "&")){
			buf_pointer[j_buf] = pointer[i];
			j_buf++;
		} else {
			status = execute_cond(buf_pointer, 1);
			j_buf = 0;
			free(buf_pointer);
			buf_pointer = calloc(sizeof(char*), len_arr(pointer) + 1);
		}
	}
	stat = execute_cond(buf_pointer, 0);
	free(buf_pointer);
	return stat || status;
}

int execute_cd(char **pointer){
	int stat = 0;
	if (!pointer[1])
		chdir(getenv("HOME"));
	else {
		if (chdir(pointer[1])){
			printf("%s\n", "CD ERROR: -1");
			stat = 1;
		}	
	}
	return stat;
}

void sigHndlr(int s){
	if (s == SIGINT)
		exit(0);
}


int main(int argc, char **argv){
	char **pointer;
	char *str;
	int stat = 0;
	FILE *fp;
	if ((argc == 1) || (!strcmp(argv[1], "stdin"))){
	//	printf("HERE\n");
		fp = stdin;
	}
		
	else if (argc >= 2){
	//	printf("OH NO\n");
		fp = fopen(argv[1], "r");
		if (!fp){
			printf("ERROR: FILE OPEN\n");
			return 1;
		}	
	}
	if (argc == 3) {
	//	printf("WHERE AM I\n");
	//	out_str(&argv[2]);
	//	printf("_______");
	    pointer = split_bracket(argv[2]);
	//   out_str(pointer);
	    stat = execute_background(pointer);
	 //   printf("%d\n", stat);
		freedom(pointer);
	  	return stat;
   }
	signal(SIGINT, sigHndlr);
	while((printf("%s", "\n$ ")) && (str = get_str(fp))){
		pointer = split_bracket(str);
		if (!pointer[0]){
			printf("%s\n", "ERROR: NO COMMAND\n");
			continue;
		}
		if (!strcmp(pointer[0], "cd"))
			stat = execute_cd(pointer);
		else
			stat = execute_background(pointer);
		freedom(pointer);
		free(str);
	}
	printf("\n");
	if (argc == 2)
		fclose(fp);
	return stat;
}
