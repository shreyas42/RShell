%{
    #include"util.h"
%}

%union {char *string_val;} 
%token NEWLINE GREAT LESS GREATGREAT PIPE GREATAMPERSAND GREATGREATAMPERSAND AMPERSAND NOTOKEN WORD
%start goal
%%
goal: command_loop {printf("Valid input\n");}
;
command_loop: command_loop command_line 
|
;
command_line: pipe_list io_modifier_list background_opt NEWLINE
| NEWLINE
| error NEWLINE{yyerrok;}
;
pipe_list: pipe_list PIPE command_args
| command_args
;
command_args: WORD arg_list 
;
arg_list: arg_list WORD 
|
;
io_modifier_list: io_modifier_list io_modifier
|
;
io_modifier: GREATGREAT WORD
| GREAT WORD
| LESS WORD
| GREATAMPERSAND WORD
| GREATGREATAMPERSAND WORD
;
background_opt: AMPERSAND
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