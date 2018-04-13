#ifndef COMMAND_H_
#define COMMAND_H_

#define CMD_NAME 32
#define ARGSIZE 1024
#define MAXSIZE 64

struct simplecmd{
    int num_ava_arg;
    int max_num_arg;
    char **args;
    char *cmd_name;
};


struct command{
    int num_ava_simple_command;
    int num_sim_commands;
    struct simplecmd ** simplecmds;
    char *outfile;
    char *infile;
    char *errfile;
    int mode;
    int background;
};

struct command *command_table;

struct simplecmd *make_new_simplecmd(char *cmd);
void insert_args(struct simplecmd *cmd, char *arg);
void free_simple_cmd(struct simplecmd *scmd);
void print_table();
void new_command_table();
void free_command_table();
void clear_command_table();
void insert_simplecmd(struct simplecmd *simplecmd);
#endif