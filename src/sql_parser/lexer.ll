%{ 
# include <cerrno>
# include <climits>
# include <cstdlib>
# include <cstring> 
# include <string>
# include "driver.hpp"
# include "parser.tab.hpp"
%}

%option noyywrap nounput noinput batch debug
%{
    yy::parser::symbol_type make_NUMBER (const std::string &s, const yy::parser::location_type& loc);
%}

int "0"|(("+"|"-")?[1-9][0-9]*)
blank [ \t\n]

%{
  #define YY_USER_ACTION  loc.columns (yyleng);
%}

%%

%{
  yy::location& loc = drv.location();
  loc.step ();
%}

"SELECT"{blank} { return yy::parser::make_SELECT(loc); }

";"  { return yy::parser::make_SEMICOLON(loc); }
"-"  { return yy::parser::make_MINUS(loc); }
"+"  { return yy::parser::make_PLUS(loc); }
"*"  { return yy::parser::make_MUL(loc); }
"/"  { return yy::parser::make_DIV(loc); }
"("  { return yy::parser::make_LPAREN(loc); }
")"  { return yy::parser::make_RPAREN(loc); }
"=" { return yy::parser::make_ISEQ(loc); }
">=" { return yy::parser::make_MOREEQ(loc); }
"<=" { return yy::parser::make_LESSEQ(loc); }
">"  { return yy::parser::make_MORE(loc); }
"<"  { return yy::parser::make_LESS(loc); }

{int} { return make_NUMBER (yytext, loc); }

{blank} { }

.    {
         throw yy::parser::syntax_error
           (loc, "invalid character: " + std::string(yytext));
     }
<<EOF>>    return yy::parser::make_YYEOF (loc);

%%

yy::parser::symbol_type make_NUMBER(const std::string &s, const yy::parser::location_type& loc) {
    errno = 0;
    long n = strtol (s.c_str(), NULL, 10);
    if (! (INT_MIN <= n && n <= INT_MAX && errno != ERANGE))
        throw yy::parser::syntax_error (loc, "integer is out of range: " + s);
    return yy::parser::make_NUMBER ((int) n, loc);
}
void driver::scan_begin() {
    yy_flex_debug = debug_mode_;
    yyin = stdin;
}

void driver::scan_end() {
}

