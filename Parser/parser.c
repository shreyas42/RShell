#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "command.h"
#include "../util.h"
#include <regex.h>
#include <dirent.h>

extern struct history_cmd *hcmd;
extern FILE *fp;

void init_parser(){
    // Initializes the parser
    // Initializes the history command table
    fp = fopen("cmdhistory.txt", "a+");
    hcmd = (struct history_cmd *)malloc(sizeof(struct history_cmd));
    hcmd->cmdhis = (struct cmd *) malloc(sizeof(struct cmd)*CMD);
    hcmd->num_cmds = 0;
    hcmd->max_size = CMD;
    // Initializes the global command table
    new_command_table();
}

void destroy_parser(){
    // Writes all the command to a file
    free_command_table();
    for(int i = 0;i < hcmd->num_cmds;i++){
        free(hcmd->cmdhis[i].cmdname);
    }
    free(hcmd->cmdhis);
    free(hcmd);
    hcmd = NULL;
    fclose(fp);
}

void reset_parser(){
    clear_command_table();
}

void add_cmd_history(char *cmd, int pid){
    if(hcmd->max_size == hcmd->num_cmds){
        hcmd->num_cmds = 0;
    }
    struct cmd temp;
    temp.cmdname = (char *)malloc(sizeof(char)*strlen(cmd)+1);
    temp.pid = pid;
    strncpy(temp.cmdname, cmd, strlen(cmd));
    temp.cmdname[strlen(cmd)+1] = '\0';
    hcmd->cmdhis[hcmd->num_cmds] = temp;
    hcmd->num_cmds++;
    fprintf(fp, "CMD: %s, PID: %d\n", hcmd->cmdhis[hcmd->num_cmds-1].cmdname, hcmd->cmdhis[hcmd->num_cmds-1].pid);
}

void display(){
    for(int i = 0;i < hcmd->num_cmds;i++){
        printf("%s\t%d\n", hcmd->cmdhis[i].cmdname, hcmd->cmdhis[i].pid);
    }
}

char *search(char *alias){
    for(int i = 0;i < used;i++){
        if(!strcmp(list[i].alias_name, alias)){
            char *cmd = (char *)malloc(sizeof(char)*strlen(list[i].command_name));
            strcpy(cmd, list[i].command_name);
            cmd[strlen(list[i].command_name)+1] = '\0';
            return cmd;
        }
    }
    return NULL;
}

void process_args_insert(struct simplecmd *cmd, char* arg){
	regex_t reg;	
	char *p = strchr(arg, '*');
	char *q = strchr(arg, '?');
	size_t matches = 100;
	regmatch_t matched[matches];
	if(p == NULL && q == NULL){
		insert_args(cmd, arg);
		return ;	
	}
	char *regex = (char *)malloc(2*strlen(arg)+10);
	char *a = arg;
	char *r = regex;
	*r = '^'; 
	r++;
	while(*a){
		if(*a == '*'){
			*r='.';
			r++;
			*r='*';
			r++;		
		}	
		else if(*a == '?'){
			*r='.';
			r++;		
		}
		else if(*a == '.'){
			*r = '\\';
			r++;
			*r='.';
			r++;		
		}
		else{
			*r = *a;
			r++;
		}
		a++;
	}
	*r='$';
	r++;
	*r=0;
	int expbuf = regcomp(&reg, regex, 0);
	if(expbuf > 0){
		perror("regcomp");
		free(regex);		
		return;
	}
	DIR *dir = opendir(".");
	if(dir == NULL){
		perror("opendir");
		free(regex);	
		return;
	}
	struct dirent *ent;
	while ((ent = readdir(dir))!=NULL){
		if(regexec(&reg, ent->d_name, matches, matched, 0) == 0){
			insert_args(cmd, strdup(ent->d_name));
		}
	}
	closedir(dir);
	free(regex);
}

void process_token(char *token){
    char *tok = strtok(token, SPACE); //SPACE " "
    struct simplecmd *cmd;
    char *cm;
    if((cm=search(tok))== NULL){
        cmd = make_new_simplecmd(tok);
    }
    else{
        cmd = make_new_simplecmd(cm);
        free(cm);
    }
    char c = '\0';
    while(tok != NULL){
        tok = strtok(NULL, SPACE);
        if(tok != NULL){
            switch(tok[0]){
                case '>': c = '>';
                        if(tok[1] == '>'){
                            command_table->mode = 1;
                        }
                        break;
                case '<':   c = '<';
                            break;
                case '&':   c = '\n';
                            command_table->background = 1;
                        break;
                default: if(c == '\0'){
                            process_args_insert(cmd, tok);
                        }
                        else if(c == '<'){
                            strcpy(command_table->infile, tok);
                            c = '\0';
                        }
                        else if(c == '>'){
                            strcpy(command_table->outfile, tok);
                            c = '\0';
                        }
                        break;
            }
        }
    }
    insert_simplecmd(cmd);
}

void parse_cmd(char *cmd){
    // Tokenize the command using strtok
    // Understand and code for working of every symbol
    // Use add commands function 
    // Print command table generated
    char* command = (char *)malloc(sizeof(char)*strlen(cmd)+1);
    strncpy(command, cmd, strlen(cmd));
    command[strlen(cmd)] = '\0';
    char *tokens[64];
    int i = 0;
    tokens[i] = strtok(command, PIPE); //PIPE "|"
    char *temp = tokens[0];
    while(temp != NULL){
        temp  = strtok(NULL, PIPE);
        if(temp != NULL){
            i++;
            tokens[i] = temp;
        }
    }
    for(int k = 0;k <=i ;k++){
        process_token(tokens[k]);
    }
    // DEBUG: print_table();
    free(command);
}
