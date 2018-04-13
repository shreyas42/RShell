#ifndef SHELL_H_
#define SHELL_H_

#define SIZE 1024

char *config_file = ".rshrc";
int ind = 0;
struct conf{
    char key[SIZE];
    char val[SIZE];
}config[5];




void init_shell(char *config_file);
int get_index(char *key);
#endif