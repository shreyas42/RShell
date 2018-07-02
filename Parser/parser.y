%{
    #include"util.h"
    #define SIZE 5
    int track = 0;
    struct command_entry *cur_entry;
    struct command_line *cmdtable;
%}

%union {
    char *string_val;
    } 
%token NEWLINE GREAT LESS GREATGREAT PIPE AMPERSAND NOTOKEN WORD
%start goal
%%
goal: command_loop {printf("Validated command\n");exit(0);}
;
command_loop: command_loop command_line 
|
;
command_line: pipe_list io_modifier_list background_opt NEWLINE /*create new command line, add entries from temp list, set params from global variables, add line to command table */
| NEWLINE
| error NEWLINE{yyerrok;}
;
pipe_list: pipe_list PIPE command_args
| command_args
;
command_args: WORD {cur_entry = create_cmd_entry(yylval.string_val);} arg_list {if(!add_cmd_entry(cmdtable,cur_entry)){
    delete_cmd_entry(cur_entry);
    delete_cmd_line(cmdtable);
    cur_entry = NULL;
    cmdtable = NULL;
    exit(-1);
}} /*TO DO:create a new command entry add it to the temp list. */
;
arg_list: arg_list WORD {add_argument(cur_entry,yylval.string_val);} /*TO DO: add args to current cmd entry in the temp list */
|
;
io_modifier_list: io_modifier_list io_modifier
|
;
io_modifier: GREATGREAT WORD /* set global variable outfile, also set global variable appendmode*/
| GREAT WORD /* set global variable outfile */
| LESS WORD /* set global variable infile */
;
background_opt: AMPERSAND /* set global variable isBackground to true */
|
;
%% 

int main(){
    cmdtable = create_cmd_line();
    return(yyparse()); 
}

int yyerror(char *s){
    fprintf(stderr,"%s\n",s);
    return 0;
}

int yywrap(){
    return 1;
}
