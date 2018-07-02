#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "passgen.h"

char *passgen(int passlen){
	srand((unsigned int)(time(NULL)));
	int i;
	if(passlen > MAXLEN){
		passlen = MAXLEN;	
	}
	for(i = 0;i < passlen;i++){
		password[i] = password_symbols[rand()%61];	
	}
	password[i] = '\0';
	return password;
}

int main(int argc, char *argv[]){
	int passlen = 10; // Default length of password.
	if(argc == 1){
		goto gen;	
	}
	if(!strcmp(argv[1], "-help")){
		jump:	printf("passgen:\n\t Command Line Utility to generate a random password.\n\t Options: \n\t Length of password  -l option\n");
		return 0;	
	}
	if(!strcmp(argv[1], "-l")){
		if(argc == 3){
			passlen = atoi(argv[2]);
		}
		else{
			goto jump;		
		}	
	}
	gen: ;
		char *pass_word = passgen(passlen);
		printf("%s\n", pass_word);
	return 0;
}
