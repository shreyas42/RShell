#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "command.h"

struct simplecmd* make_new_simplecmd(char *cmdname){
    struct simplecmd *smpcmd = (struct simplecmd *) malloc(sizeof(struct simplecmd));
    smpcmd->cmd_name = (char *) malloc(sizeof(char)*CMD_NAME);
    strncpy(smpcmd->cmd_name, cmdname, strlen(cmdname)+1);
    smpcmd->max_num_arg = 8;
    smpcmd->num_ava_arg = 0;
    smpcmd->args = (char **) malloc(sizeof(char *)*(smpcmd->max_num_arg));
    for(int i = 0;i < smpcmd->max_num_arg;i++){
        smpcmd->args[i] = NULL;
    }
    insert_args(smpcmd, cmdname);
    return smpcmd;    
}

void free_simple_cmd(struct simplecmd *scmd){
    free(scmd->cmd_name);
    for(int i = 0;i < scmd->num_ava_arg;i++){
        free(scmd->args[i]);
    }
    free(scmd->args);
}

void insert_args(struct simplecmd* cmd, char *arg){
    cmd->args[cmd->num_ava_arg] = (char *) malloc(sizeof(char)*strlen(arg)+1);
    strncpy(cmd->args[cmd->num_ava_arg], arg, strlen(arg)+1);
    cmd->num_ava_arg++;
}

void new_command_table(){
    if(command_table != NULL){
        return ;
    }
    command_table = (struct command *)malloc(sizeof(struct command));
    command_table->num_sim_commands = MAXSIZE;
    command_table->num_ava_simple_command = 0;
    command_table->simplecmds = (struct simplecmd **)malloc(sizeof(struct simplecmd *)*MAXSIZE);
    command_table->infile = (char *)malloc(sizeof(char)*ARGSIZE);
    strcpy(command_table->infile, "stdin\0");
    command_table->outfile = (char *)malloc(sizeof(char)*ARGSIZE);
    strcpy(command_table->outfile, "stdout\0");
    command_table->errfile = (char *)malloc(sizeof(char)*ARGSIZE);
    strcpy(command_table->errfile, "stderr\0");
    command_table->background = 0;
    command_table->mode = 0; // 0 is overwrite 1 is append;
}

void free_command_table(){
    if(command_table == NULL){
        return ;
    }
    free(command_table->errfile);
    free(command_table->infile);
    free(command_table->outfile);
    for(int i = 0;i < command_table->num_ava_simple_command;i++){
        free_simple_cmd(command_table->simplecmds[i]);
        free(command_table->simplecmds[i]);
    }
    free(command_table->simplecmds);
    free(command_table);
    command_table = NULL;
}

void insert_simplecmd(struct simplecmd *scmd){
    command_table->simplecmds[command_table->num_ava_simple_command] = scmd;
    command_table->num_ava_simple_command++;
}
void print_simple_cmd(struct simplecmd *cmd){
    fprintf(stderr, "AVA ARGS: %d\n", cmd->num_ava_arg);
    fprintf(stderr, "MAX ARGS: %d\n", cmd->max_num_arg);
    fprintf(stderr, "Command: %s\n", cmd->cmd_name);
    for(int i = 0;i < cmd->num_ava_arg;i++){
        fprintf(stderr, "ARGS: %s\n", cmd->args[i]);
    }
}

void print_table(){
    fprintf(stderr, "***************************DEBUG: COMMAND TABLE***************************\n");
    fprintf(stderr, "MAX CMDS: %d\n", command_table->num_sim_commands);
    fprintf(stderr, "AVA CMDS: %d\n", command_table->num_ava_simple_command);
    fprintf(stderr, "Infile: %s\n", command_table->infile);
    fprintf(stderr, "Outfile: %s\n", command_table->outfile);
    fprintf(stderr, "Errfile: %s\n", command_table->errfile);
    if(command_table->mode){
        fprintf(stderr, "Write Mode: Append Mode\n");
    }
    else{
        fprintf(stderr, "Write Mode: Overwrite Mode\n");
    }
    if(command_table->background){
        fprintf(stderr, "Background Process: True\n");
    }
    else{
        fprintf(stderr, "Background Process: False\n");
    }
    for(int i = 0;i < command_table->num_ava_simple_command;i++){
        print_simple_cmd(command_table->simplecmds[i]);
    }
    fprintf(stderr, "***************************DEBUG: END**********************************\n");
}

void clear_command_table(){
    memset(command_table->outfile, '\0', ARGSIZE);
    strncpy(command_table->outfile, "stdout\0",7);
    memset(command_table->infile, '\0', ARGSIZE);
    strncpy(command_table->infile, "stdin\0",6);
    memset(command_table->errfile, '\0', ARGSIZE);
    strncpy(command_table->errfile, "stderr\0",7);
    command_table->mode = 0;
    command_table->background = 0;
    for(int i = 0;i < command_table->num_ava_simple_command;i++){
        free_simple_cmd(command_table->simplecmds[i]);
    }
    command_table->num_ava_simple_command = 0;
}
