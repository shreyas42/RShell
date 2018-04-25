#include <stdio.h>
#include <string.h>
#include "joke.h"
#include <stdlib.h>
#include <time.h>

void display(){
	for(int i = 0;i < used;i++){
		printf("%s",joke[i]);	
	}
}

void getJoke(int index){
	printf("%s", joke[index]);
}

char * get_input(FILE *fp){
    char *inp = NULL;
    ssize_t buffsize = 0;
    getline(&inp, &buffsize, fp);
    int t;
    if((t = strlen(inp)) > 1){
        inp[t] = '\0';
    }
    return inp;
}


void readFile(char *path){
	FILE *fp = fopen(path, "r");
	char *temp = get_input(fp);
	while(temp[0] != '\0'){
		joke[used] = (char *)malloc(sizeof(char)*1024);
		strcpy(joke[used], temp);
		free(temp);
		used++;
		temp = get_input(fp);
	}
	fclose(fp);
}

void destroy(){
	for(int i = 0;i < used;i++){
		free(joke[i]);
	}
}

int main(int argc, char *argv[]){
	if(argc ==2){
		printf("This is command line utility to print a joke from the database.\nKEEP SMILING!\n");
		return 0;
	}
	readFile("/home/rishabh/USPproj/RShell/Command/jokey/joke.txt");
	srand(time(NULL));
	int t = rand()%used;
	getJoke(t);
	destroy();
	return 0;
}
