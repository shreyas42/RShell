#ifndef UTIL_H_
#define UTIL_H_

//header file that contains the common libraries that our
//parser and lexer require
//also defining our command table datastucture

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define DEFAULT_ARGS 2
#define DEFAULT_ENTRIES 2
#define DEFAULT_LINES 2

typedef int bool;

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

struct command_entry{
    char *command_name; //name of the command: passed to the exectutor
    int availableArgs; //used to keep track of current size of arglist
    int numArgs; //number of args currently in the arglist
    char **arglist; //array of arguments to the command
};

struct command_line{
    int available_command_entries; //size of the command entry list
    int num_command_entries; //number of entries in the list;
    struct command_entry **command_entry_list; //list of all commands in the line
    char *infile; //input redirection
    char *outfile; //output redirection
    bool appendMode; //append mode or write mode
    bool isBackground; //set for background 
};

struct command_table{
    struct command_line **cmd_line_list;
    int available_args;
    int num_args;
};

/*functions for the command table here */

//command entry functions
struct command_entry *create_cmd_entry(char *cmd_name);
int add_argument(struct command_entry *cur_entry,char *arg);
int delete_cmd_entry(struct command_entry *this);
void disp_cmd_entry(struct command_entry *this);

//command line functions
struct command_line *create_cmd_line();
int add_cmd_entry(struct command_line *cur_line,struct command_entry *cur_cmd);
int set_params(struct command_line *line,char *infile,char *outfile,bool appendMode,bool isBackground);
int delete_cmd_line(struct command_line *this);
void disp_cmd_line(struct command_line *this);

//command table functions
struct command_table *initialize_command_table();
int add_cmd_line(struct command_table *table,struct command_line *cur_line);
int delete_cmd_table(struct command_table *this);
void disp_cmd_table(struct command_table *this);

#endif
