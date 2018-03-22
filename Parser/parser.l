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
">&a" {return GREATAMPERSAND;};
">>&" {return GREATGREATAMPERSAND;};
"&" {return AMPERSAND;};
. {return NOTOKEN;};
[^ \t\n][^ \t\n]* {yylval.string_val = strdup(yytext);return WORD;};
%%