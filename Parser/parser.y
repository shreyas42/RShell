%{
    #include"util.h"
    #define SIZE 5
    struct command_entry **entries;
    int track = 0;
%}

%union {
    char *string_val;
    } 
%token NEWLINE GREAT LESS GREATGREAT PIPE AMPERSAND NOTOKEN WORD
%start goal
%%
goal: command_loop {printf("Valid input\n");}
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
command_args: WORD arg_list {create_cmd_entry(yylval.string_val);} /*TO DO:create a new command entry add it to the temp list. */
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
   printf("Enter the input program\n");
   return(yyparse()); 
}

int yyerror(char *s){
    fprintf(stderr,"%s\n",s);
    return 0;
}

int yywrap(){
    return 1;
}
