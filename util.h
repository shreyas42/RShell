#ifndef UTIL_H_
#define UTIL_H_
#define TRUE 1
#define FALSE 0
struct alias{
    char alias_name[32];
    char command_name[1024];
};

struct alias *list;
int used;
#define ALIASLIMIT 100 
#endif