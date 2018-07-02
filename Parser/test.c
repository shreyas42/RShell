#include "command.h"
#include "parser.h"
#include <stdio.h>


int main(){
    char cmd[64] = "ls -al|grep hello < infile > outfile";
    init_parser();
    parse_cmd(cmd); 
    add_cmd_history(cmd);
    destroy_parser();   
}
