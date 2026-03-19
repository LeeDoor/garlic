%skeleton "lalr1.cc"
%language "c++"
%require "3.8.1"
%header
%define api.value.type variant
%define api.token.constructor
%define parse.assert

%code requires {
    #include "value.hpp"

    namespace garlic::sql_parser { class driver; }
    using namespace garlic::sql_parser;
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
    NOTEQ    
    MOREEQ  
    LESSEQ  
    MORE    
    LESS    
    ;
%token
    LOGICAND
    LOGICOR
    NOT
    ;

%token EOL "EOL"
%token <float> FLOAT "float"
%token <int> INTEGER "integer"
%nterm <bool> comp
%nterm <Value> query
%nterm <Value> expr 
%nterm <Value> value 
%printer { yyo << $$; } <*>;

%%

queries: /**/
    | queries query SEMICOLON { std::cout << $2 << std::endl; }
    | queries SEMICOLON
    ;

query: SELECT comp { $$ = static_cast<int>($2); }
     | SELECT expr { $$ = $2; }
     ;

comp: comp LOGICAND comp { $$ = $1 && $3; }
    | comp LOGICOR comp { $$ = $1 || $3; }
    | LPAREN comp RPAREN { $$ = $2; }
    | NOT LPAREN comp RPAREN { $$ = !$3; }
    | expr MOREEQ expr { $$ = $1 >= $3; }
    | expr LESSEQ expr { $$ = $1 <= $3; }
    | expr ISEQ expr { $$ = $1 == $3; }
    | expr NOTEQ expr { $$ = $1 != $3; }
    | expr MORE expr { $$ = $1 > $3; }
    | expr LESS expr { $$ = $1 < $3; }
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
   | ABS expr ABS { $$ = $2.abs(); }
   | LPAREN expr RPAREN { $$ = $2; }
   | MINUS expr %prec UMINUS { $$ = $2 * -1; }
   | expr REMDIV expr { 
        $$ = $1 % $3; 
    }
   ;

value: INTEGER { $$ = $1; }
     | FLOAT   { $$ = $1; } 
     ;

%left LOGICOR;
%left LOGICAND;
%left NOT;
%left PLUS MINUS;
%left MUL DIV REMDIV;
%left UMINUS;

%%
void yy::parser::error (const location_type& l, const std::string& m) {
  std::cerr << "[SYNTAX_ERROR] " << l << ": " << m << '\n';
}
