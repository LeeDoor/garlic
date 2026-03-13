%define api.value.type {int}
%parse-param {int *ret}

%{
    #include <stdio.h>

    int yylex();
    static void yyerror(int *ret, const char* s) { (void)ret; fprintf(stderr, "%s\n", s); }
%}

// Terminals

%token NUMBER PLUS MINUS TIMES UMINUS LPAREN RPAREN EOL

// Precedence and associativity

%left PLUS MINUS
%left TIMES
%left UMINUS

%%
start
    : expr EOL { *ret = $1; }
    | EOL { *ret = 0; }
;

expr
    : expr PLUS expr { $$ = $1 + $3; }
    | expr MINUS expr { $$ = $1 - $3; }
    | expr TIMES expr { $$ = $1 * $3; }
    | MINUS expr %prec UMINUS { $$ = -$2; }
    | LPAREN expr RPAREN { $$ = $2; }
    | NUMBER { $$ = $1; }
;

