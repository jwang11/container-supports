#include<stdio.h>
#include <string.h>
int main(int argc, char *argv[])
{
	FILE *f;
	int nn, i;
	char ch;
	if (3 == argc){
		f = fopen(argv[1], "w");
		if(f == NULL){
			printf("Open file %s with write ERROR\n", argv[1]);
			return 2;
		}
		nn = strlen(argv[2]);
		i = 0;
		while(i < nn){
			fputc(argv[2][i], f);
			++i;
		}
		fclose(f);
	} else if (argc == 2){
		f = fopen(argv[1], "r");
		if (NULL == f){
			printf("Open file %s with read ERROR\n", argv[1]);
			return 2;
		}
		while((ch=fgetc(f)) != EOF){
			printf("%c", ch);
		}
		printf("\n");
		fclose(f);
	}else{
		printf("Usage: test file **\n");
		return 3;
	}
	return 0;
}
