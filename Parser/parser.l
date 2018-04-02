%{
#include"util.h"
#include"y.tab.h"
%}
%%
"\n" {return NEWLINE;};
[ \t] {};
">" {return GREAT;};
"<" {return LESS;};
">>" {return GREATGREAT;};
"|" {return PIPE;};
"&" {return AMPERSAND;};
. {return NOTOKEN;};
[^ \t\n][^ \t\n]* {yylval.string_val = strdup(yytext);return WORD;};
%%