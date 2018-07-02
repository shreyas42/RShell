#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell.h"
#include "../Parser/parser.h"
#include "../Executor/executor.h"
#include "../Parser/command.h"
#include "../util.h"
#include <signal.h>
#include <unistd.h>
#define KCYN "\x1B[36m"
#define KMAG "\x1B[35m"
#define KWHT "\x1B[37m"

extern int ind;

void interrupt_handler(int signo){
	return ;
} 

void get_key_val(char *buf, char *key, char *val){
    char * ret = strrchr(buf, '=');
    int len = strlen(ret);
    int inlen = strlen(buf);
    strncpy(key,buf,inlen-len);
    key[strlen(key)]='\0';
    strncpy(val,ret+1,len);
    val[strlen(val)]='\0';
}

void alias(){
    char alias[32], cmd[32];
    printf("Enter the alias name:\t");
    scanf("%s", alias);
    printf("Enter the command name:\t");
    scanf("%s", cmd);
    struct alias temp;
    strcpy(temp.alias_name, alias);
    strcpy(temp.command_name, cmd);
    list[used++] = temp;
    printf("Alias Created for %s!\n", cmd);
}

int get_index(char *key){
    for(int i = 0;i < ind;i++){
        if(!strcmp(key, config[i].key)){
            return i;
        }
    }
    return -1;
}


void init_shell(char *conf_file){
    printf("Welcome to RShell! This is custom build shell for learning shell concepts.\n");
    FILE * fptr;
    char buf[SIZE];
    list = (struct alias *)malloc(sizeof(struct alias)*ALIASLIMIT);

    if ((fptr = fopen(conf_file, "r")) == NULL)
    {
        fprintf(stderr, "Error! opening %s file\n", conf_file);
        // Program exits if file pointer returns NULL.
        exit(-1);         
    }
    while(fscanf(fptr, "%[^\n]\n", buf) != EOF){
        // printf("DEBUG: INFO %s\n", buf);
        if(buf[0] == '%' && buf[1] == '%'){
            memset(buf,'\0',sizeof(buf));
            continue;
        }
        if(buf[0] == '@'){
            char * ret = strchr(buf, ':');
            char conf[SIZE];
            strncpy(conf,ret+1, strlen(ret));
            conf[strlen(conf)] = '\0';
            // printf("%s\n", conf);
            char key[SIZE], val[SIZE];
            memset(key,'\0',sizeof(key));
            memset(val,'\0',sizeof(val));
            get_key_val(conf,key,val);
            // printf("%s\n", key);
            // printf("%s\n", val);
            strncpy(config[ind].key, key, strlen(key));
            strncat(config[ind].key, "\0", strlen(key));
            strncpy(config[ind].val, val, strlen(val));
            strncat(config[ind].val, "\0", strlen(val));
            ind++;
            memset(conf,'\0',sizeof(conf));
            memset(key,'\0',sizeof(key));
            memset(val,'\0',sizeof(val));
        }
        else if(buf[0] == 'a'){
            char *ret = strchr(buf, ' ');
            char *p = strchr(ret, '\r');
            // *p='\0';
            struct alias temp;
            int i = 1;
            while(ret[i] != '='){
                temp.alias_name[i-1] = ret[i];
                i++;
            }
            temp.alias_name[i-1] = '\0';
            char *ret1 = strchr(ret, '=');
            strcpy(temp.command_name, ret1+1);
            temp.command_name[strlen(ret+1)+1] = '\0';
            list[used] = temp;
            used++; 
        }
        memset(buf,'\0',sizeof(buf));
    }

    fclose(fptr);
}

char * get_command(){
    char *cmd = NULL;
    ssize_t buffsize = 0;
    getline(&cmd, &buffsize, stdin);
    int t;
    if((t = strlen(cmd)) > 1){
        cmd[t-1] = '\0';
    }
    return cmd;
}

int main(){
	struct sigaction interruptaction;
	interruptaction.sa_sigaction = &interrupt_handler;
	interruptaction.sa_flags = SA_SIGINFO | SA_RESTART;
	if(sigaction(SIGINT, &interruptaction , NULL) < 0)
		perror("Sigaction");
    init_shell(config_file);
    char cwd[1024];
    char buffer[1024];
    int index1,index2,index3,index4;
    if((index1 = get_index("USERNAME")) != -1){
        strncpy(buffer, config[index1].val, strlen(config[index1].val));
        index2 = get_index("HOST");
        strncat(buffer,"@",1);
        strncat(buffer, config[index2].val, strlen(config[index2].val));
        strncat(buffer,":",1); 
    }
    index4 = get_index("PROMPT");
    //printf("%s\n",config[get_index("PATH")].val);
    char *command;
    init_parser();
    int pid;
    while(1){
        getcwd(cwd, sizeof(cwd));
        printf("%s%s%s%s%s%s ", KCYN, buffer,KMAG, cwd,KWHT, config[index4].val);
        fflush(stdout);
        // Read Input
        command = get_command();
        if(!strcmp(command, "exit")){
            free(command);
            command = NULL;
            break;
        }
        if(!strcmp(command, "\n")){
            free(command);
            command = NULL;
            fflush(stdout);
            fflush(stdin);
            continue;
        }
        if(!strcmp(command, "alias")){  
            free(command);
            command = NULL;
            alias();
            fflush(stdout);
            fflush(stdin);
            continue;
        }
        if(!strcmp(command, "history")){
            free(command);
            command = NULL;
            display();
            fflush(stdout);
            fflush(stdin);
            continue;
        }
        // Call to parser
        parse_cmd(command);
        // Call to executor
        pid = execute();
        add_cmd_history(command, pid);
        free(command);
        command = NULL;
        reset_parser();
        fflush(stdout);
        fflush(stdin);
    }
    destroy_parser();
    free(list);
    return 0;
}
