#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "remind.h"

void initial_text(){
	printf("***************************************************************\n");	
	printf("Welcome to CMD Remind.\n");
	printf("Press 1. To add a reminder.\n");
	printf("Press 2. To display a reminder for some date.\n");
	printf("Press 3. To delete a reminder date.\n");
	printf("Press 4. To exit.\n");
	printf("***************************************************************\n");
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

void initialize(){
	fp = fopen("/home/rishabh/USPproj/RShell/Command/remind/remind.txt", "r");
	char * temp = get_input(fp);
	int ptrlen;
	while(temp[0] != '\0'){
		char *ptr = strchr(temp, ':');
		ptrlen = strlen(ptr+1);
		if(temp[0] == 'D'){
			memobook[used].date = (char *)malloc(sizeof(char)*ptrlen);
			ptr[ptrlen]='\0';
			memobook[used].current = 0;
			strcpy(memobook[used].date, ptr+1);
			used++;
		}
		else if(temp[1] == 'V'){
			memobook[used-1].memo[memobook[used-1].current] = (char *)malloc(sizeof(char)*ptrlen);
			strcpy(memobook[used-1].memo[memobook[used-1].current], ptr+1);
			memobook[used-1].current++;
		}
		free(temp);
		temp = get_input(fp);
	}
	fclose(fp);
}

void writeFile(){
	fp = fopen("/home/rishabh/USPproj/RShell/Command/remind/remind.txt", "w");
	for(int i = 0;i < used;i++){
		fprintf(fp, "Date:%s\n", memobook[i].date);
		for(int j = 0;j < memobook[i].current;j++){
			fprintf(fp, "\tValue:%s", memobook[i].memo[j]);
		}
	}
}

int search(char *date){
	for(int i = 0;i < used;i++){
		if(!strcmp(memobook[i].date, date)){
			return i;
		}
	}
	return -1;
}

void addentry(char *date, char *rem){
	int t = search(date);
	if(t == -1){
		memobook[used].date = (char *)malloc(sizeof(char)*strlen(date));
		strcpy(memobook[used].date, date);
		memobook[used].current = 0;
		memobook[used].memo[memobook[used].current] = (char *)malloc(sizeof(char)*strlen(rem));
		strcpy(memobook[used].memo[memobook[used].current], rem);
		memobook[used].current++;
		used++;
	}
	else{
		if(memobook[t].current == 10){
			printf("Cannot add more reminders\n");
			return;
		}
		else{
			memobook[t].memo[memobook[t].current] = (char *)malloc(sizeof(char)*strlen(rem));
			strcpy(memobook[t].memo[memobook[t].current], rem);
			memobook[t].current++;
		}
	}
}

void deleteentry(char *date){
	int t = search(date);
	if(t == -1){
		printf("Date Not Found\n");
		return ;
	}
	else{
		int i = used-1;
		struct reminder temp = memobook[t];
		memobook[t] = memobook[i];
		memobook[i] = temp;
		used--;
	}
}

void displayAll(char *date){
	int t = search(date);
	if(t == -1){
		printf("Date Not Found\n");
		return ;
	}
	else{
		printf("Date: %s\n", memobook[t].date);
		for(int i = 0;i < memobook[t].current;i++){
			printf("\tReminder: %s", memobook[t].memo[i]);
		}
	}
}
void cleanup(){
	for(int i = 0;i < used;i++){
		free(memobook[i].date);
		for(int j = 0;j < memobook[i].current;j++){
			free(memobook[i].memo[j]);
		}
	}
}

int main(int argc, char *argv[]){
	if(argc == 2){
		printf("remind:\n\tA command line tool to store the reminders for some particular date.\n\tRun the program as \n\t\t<remind>\n");
		return 0;
	}
	initial_text();
	initialize();
	int inp;
	char date[32];
	char memo[1024];
	char c;
	while(1){
		printf("Enter your choice\n");
		scanf("%d", &inp);
		switch(inp){
			case 1: printf("Enter today's Date:\n");
					scanf("%s", date);
					scanf("%c", &c);
					printf("Enter the reminder:\n");
					fgets(memo, 1024, stdin);
					addentry(date, memo);
					fflush(stdout);
					break;
			case 2: printf("Enter the date to display\n");
					scanf("%s", date);
					displayAll(date);
					fflush(stdout);
					break;
			case 3: printf("Enter the date to delete the reminders\n");
					scanf("%s", date);
					deleteentry(date);
					fflush(stdout);
					break;
			case 4: writeFile();
					cleanup();
					fflush(stdout);
					return 0;
			default: printf("Please enter from 1 to 4\n");
					 fflush(stdout);
					 break;
		}
		memset(date, '\0', 32);
		memset(memo, '\0', 1024);
		fflush(stdout);
	}
	return -1;
}	