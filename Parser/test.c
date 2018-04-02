#include"util.h"

int main(){
    struct command_entry *cur = create_cmd_entry("ls");
    add_argument(cur,"-l");
    add_argument(cur,"-i");
    add_argument(cur,"-a");
    struct command_entry *next = create_cmd_entry("./a.out");
    add_argument(next,"1");
    add_argument(next,"2");
    printf("%s\n",cur->command_name);
    printf("%s\n",next->command_name);
    for(int i=0;i<cur->numArgs;i++){
        printf("%s\n",cur->arglist[i]);
    }
    for(int i=0;i<next->numArgs;i++){
        printf("%s\n",next->arglist[i]);
    }
    struct command_line *line = create_cmd_line();
    add_cmd_entry(line,cur);
    add_cmd_entry(line,next);
    for(int i=0;i<line->num_command_entries;i++){
        for(int j=0;j<line->command_entry_list[i]->numArgs;j++){
           printf("%s\n",line->command_entry_list[i]->arglist[j]); 
        }
    }
    free(cur);
    free(next);
    free(line);
    return 0;
}