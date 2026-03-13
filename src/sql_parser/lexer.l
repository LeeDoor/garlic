%option noyywrap noinput nounput

%{
#include "parser.tab.h"
%}

INTEGER [0-9]+

%%

"+" { return PLUS; }
"-" { return MINUS; }
"*" { return TIMES; }
"(" { return LPAREN; }
")" { return RPAREN; }

{INTEGER} {
    yylval = atoi(yytext);
    return NUMBER;
}
[ \t] {}
\n { return EOL; }

. { printf("unexpected token: %s\n", yytext); }
%%

