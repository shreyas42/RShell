#ifndef PARSER_H_
#define PARSER_H_
#include <stdlib.h>
#include <stdio.h>

#define CMD 25
#define PIPE "|"
#define SPACE " "


struct cmd{
	// Time stamp
	time_t tstamp;
    char* cmdname;
    int pid;
};

struct history_cmd{
    struct cmd *cmdhis;
    int num_cmds;
    int max_size;
};

FILE *fp;

struct history_cmd *hcmd;

void init_parser();
void destroy_parser();
void reset_parser();
void parse_cmd(char *cmd);

void add_cmd_history(char *cmd, int pid);
void display();

#endif