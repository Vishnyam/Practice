#include<stdio.h>
#include<stdlib.h>
#include<string.h>
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

char **split(FILE *fp){
	char *str, *buf_str;
	char **pointer = malloc(sizeof(char*));
	int count_pointer = 0, flag = 0, special_flag = 0;
	while((str = get_str(fp))){
		buf_str = calloc(strlen(str) + 1, sizeof(char));
		int j_buf = 0;
		for (int i = 0; i < strlen(str); ++i){
			if (str[i] == '"'){
				if (special_flag)
					special_flag = 0;
				else 
					special_flag = 1;
				j_buf++;
				flag = 0;	
			} else if (special_flag)
					buf_str[i - j_buf] = str[i];
				else {
					if (str[i] == ' '){
						flag = 0;
						if ((i > 0) && (str[i - 1] != ' ')){
							buf_str[i - j_buf] = '\0';
							pointer[count_pointer] = buf_str;
							buf_str = calloc(strlen(str) + 1, sizeof(char));
							count_pointer++;
							pointer = realloc(pointer, sizeof(char*) * (count_pointer + 1));
						}
						j_buf = i + 1;
						
					} else if ((str[i] == '&') || (str[i] == '|') || (str[i] == '>')){
						if (flag){
							flag = 0;
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
								flag = 1;
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
							flag = 1;
						}
	
					} else if ((str[i] == ';') || (str[i] == '<') || (str[i] == '(') || (str[i] == ')')) {
						if (flag || (buf_str)) {
							flag = 0;
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
						if (flag) {
							flag = 0;
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
		if (special_flag){
			freedom(pointer);
			printf("%s\n", "ERROR: UNCORRECT USE OF QUOTATION MARKS");
			return NULL;
		}
		flag = 0;
		special_flag = 0;
		free(str);
	}
	pointer[count_pointer] = NULL;
	
	return pointer;
}



int main(int argc, char **argv){
	char **pointer;
	FILE *fp;
	if (argc == 1)
		pointer = split(stdin);
	else if (argc == 2){
		fp = fopen(argv[1], "r");
		if (!fp)
			return 0;
		pointer = split(fp);
	} else {
		printf("%s\n", "ERROR: UNCORRECT INPUT FILES");
		return 0;
	}
		
	printf("\n%s\n", "Split strings: ");
	out_str(pointer);
	freedom(pointer);
	if (argc == 2)
		fclose(fp);
	return 0;
}
