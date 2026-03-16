%skeleton "lalr1.cc"
%language "c++"
%require "3.8.1"
%header
%define api.value.type variant
%define api.token.constructor
%define parse.assert

%code requires {
    #include <iostream>
    #include "value.hpp"

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
    SELECT
    ;
%token SEMICOLON;
%token
    MINUS   
    PLUS    
    MUL     
    DIV     
    REMDIV
    LPAREN  
    RPAREN  
    ABS
    ;
%token
    ISEQ    
    MOREEQ  
    LESSEQ  
    MORE    
    LESS    
    NOT
    ;
%token EOL "EOL"
%token <float> FLOAT "float"
%token <int> INTEGER "integer"
%nterm <bool> comp
%nterm <bool> query /* For now its bool; later will be more complex */
%nterm <Value> expr 
%nterm <Value> value 
%printer { yyo << $$; } <*>;

%%

queries: /**/
       | queries query SEMICOLON { std::cout << $2 << std::endl; }
       | queries SEMICOLON
    ;

query: SELECT comp { $$ = $2; }
     ;

comp: expr { $$ = $1 != 0; }
    | expr MOREEQ expr { $$ = $1 >= $3; }
    | expr LESSEQ expr { $$ = $1 <= $3; }
    | expr ISEQ expr { $$ = $1 == $3; }
    | expr MORE  expr { $$ = $1 > $3; }
    | expr LESS  expr { $$ = $1 < $3; }
   ;

expr: value { $$ = $1; }
   | expr PLUS expr { $$ = $1 + $3; }
   | expr MINUS expr { $$ = $1 - $3; }
   | expr MUL expr { $$ = $1 * $3; }
   | expr DIV expr { 
        if($3 == 0) {
            error(drv.location(), "Division by zero");
            return 1;
        }
        else $$ = $1 / $3; 
    }
   | LPAREN expr RPAREN  { $$ = $2; }
   | ABS expr ABS { $$ = $2.abs(); }
   | MINUS expr %prec UMINUS { $$ = $2 * -1; }
   | expr REMDIV expr { 
        $$ = $1 % $3; 
    }
   ;

value: INTEGER { $$ = $1; }
     | FLOAT   { $$ = $1; } 
     ;

%left PLUS MINUS;
%left MUL DIV REMDIV;
%left UMINUS;
%%
void yy::parser::error (const location_type& l, const std::string& m) {
  std::cerr << l << ": " << m << '\n';
}
