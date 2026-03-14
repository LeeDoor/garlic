%skeleton "lalr1.cc"
%language "c++"
%require "3.8.1"
%header
%define api.value.type variant
%define api.token.constructor
%define parse.assert

%code requires {
    #include <iostream>
    class driver;
}

%param { driver& drv }
%locations

%define parse.trace
%define parse.error detailed
%define parse.lac full

%code {
    #include "driver.hpp"
}

%define api.token.prefix {TOK_}
%token
    MINUS   "-"
    PLUS    "+"
    MUL     "*"
    DIV     "/"
    LPAREN  "("
    RPAREN  ")"
    ISEQ    "=="
    MOREEQ  ">="
    LESSEQ  "<="
    MORE    ">"
    LESS    "<"
    ;
%token EOL "EOL"
%token <int> NUMBER "number"
%nterm <bool> comp
%nterm <int> expr 
%printer { yyo << $$; } <*>;

%%

comparisons: /**/
           | comparisons comp EOL { std::cout << $2 << std::endl; }
           | comparisons EOL
           ;

comp: expr { $$ = $1 != 0; }
    | expr ">=" expr { $$ = $1 >= $3; }
    | expr "<=" expr { $$ = $1 <= $3; }
    | expr "==" expr { $$ = $1 == $3; }
    | expr ">"  expr { $$ = $1 > $3; }
    | expr "<"  expr { $$ = $1 < $3; }
   ;

expr: NUMBER
   | expr "+" expr { $$ = $1 + $3; }
   | expr "-" expr { $$ = $1 - $3; }
   | expr "*" expr { $$ = $1 * $3; }
   | expr "/" expr { $$ = $1 / $3; }
   | "(" expr ")"  { $$ = $2; }
   ;

%left "+" "-";
%left "*" "/";

%%
void yy::parser::error (const location_type& l, const std::string& m) {
  std::cerr << l << ": " << m << '\n';
}
