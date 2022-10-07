/*Enter strings and "Enter" after that*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MEMORY 10

typedef struct list{
		char *s;
		struct list *next;
	} List;

char *get_str(void){
	int len, k = 0;
	char *str = malloc(sizeof(char*) * MEMORY);
	if(!str){
		free(str);
		return NULL;
	}
	while(fgets(str + k, MEMORY, stdin)){
		len = strlen(str);
		if(str[len - 1] != '\n'){
			k = k + MEMORY - 1;
			str = (char*)realloc(str, sizeof(char*) * (k + MEMORY));
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

void out_str(List *pointer){
	while (pointer->next != NULL){
		if (pointer->s) 
			printf("%s\n", pointer->s);
		pointer = pointer->next;	
	}
	printf("%s\n", pointer->s);
}

void freedom(List *pointer){
	if (pointer->next != NULL){
		List *follow = pointer->next;
		while (follow->next != NULL){
			free(pointer->s);
			free(pointer);
			pointer = follow;
			follow = follow->next;
		}
		free(pointer->s);
		free(pointer);
		free(follow->s);
		free(follow);
	}
	else{
		free(pointer->s);
		free(pointer);
	}
	
}

int main(){
	int flag = 1;
	while(flag){
		printf("%s\n", "Enter strings:");
		char *str = get_str();
		List *head = malloc(sizeof(List) * 1);
		if (!head){
			free(str);
			return 0;
		}
		head->s = str;
		head->next = NULL;
		str = get_str();
		while (str[0] != '\0'){
			List *p = malloc(sizeof(List) * 1);
			if (!p){
				freedom(head);
				free(str);
				return 0;
			}
			p->s = str;
			if (head->next == NULL){
				if(strcmp(head->s,str) > 0){
					p->next = head;
					head = p;
				} else {
					head->next = p;
					p->next = NULL;
				}
			} else {
				List *q = head;
				while ((q->next != NULL)){
					if (strcmp(q->next->s,str) < 0)
						q = q->next;
					else 
						break;
				}
				if (q == head){
					if (strcmp(head->s,str) > 0){
						p->next = head;
						head = p;
					} else {
						p->next = head->next;
						head->next = p;
					}
				} else {
					if (q->next == NULL){
						q->next = p;
						p->next = NULL;
					} else {
						p->next = q->next;
						q->next = p;
					}
				}		
			}
			str = get_str();
		}
		printf("%s\n", "Sorted strings:");
		List *headBuf = head;
		out_str(headBuf);
		printf("\n%s\n", "Do you want to sort again?\n1-yes, 0-no\n");
		scanf("%d", &flag);
		freedom(head);
		free(str);
	}
	

	return 0;
}
