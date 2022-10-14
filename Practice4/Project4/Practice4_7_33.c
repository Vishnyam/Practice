#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main(int argc, char **argv){
	FILE *f1, *f2;
	f1 = fopen(argv[1], "r");
	if(f1 == NULL) return 0;
	f2 = fopen("file123.txt", "w+");
	if(f2 == NULL) return 0;
	int len = atoi(argv[2]);
	char *str = malloc(sizeof(char) * (len + 1));
        if((str = get_str(f1)) == NULL){
		printf("%s\n", "Empty file");
		fclose(f1);
		return 0;
	}
        fseek (f1, 0, SEEK_SET);
	while(fgets(str, len, f1))
		fputs(str, f2);
	fclose(f1);
	f1 = fopen(argv[1], "w");
	for (int i = 0; i <= len; ++i){
		while(fgets(str, len, f2)){
			if(strlen(str) == i)
				fputs(str, f1);
		}
		fseek (f2, 0, SEEK_SET);
	}
	free(str);
	fclose(f1);
	fclose(f2);
	return 0;
}
