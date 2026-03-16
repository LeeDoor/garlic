%{ 
# include <cerrno>
# include <climits>
# include <cstdlib>
# include <cstring> 
# include <string>
# include <format>
# include "driver.hpp"
# include "parser.tab.hpp"
%}

%option noyywrap nounput noinput batch debug
%{
    #include <sstream>
    yy::parser::symbol_type make_FLOAT(const std::string &s, const yy::parser::location_type& loc);
    yy::parser::symbol_type make_INTEGER(const std::string &s, const yy::parser::location_type& loc);
%}

EXP ([Ee][-+]?[0-9]+)
float [0-9]+"."[0-9]*{EXP}?|"."?[0-9]+{EXP}?
int "0"|([1-9][0-9]*{EXP}?)
blank [ \t\n]

%{
    #define YY_USER_ACTION  loc.columns (yyleng);

    namespace ll {
    class lexing_error : public std::runtime_error {
    public:
        lexing_error(const yy::location& loc, std::string msg)
        : std::runtime_error{ format_string(loc, msg) }
        {}

    private:
        static std::string format_string(const yy::location& l, std::string msg) {
            std::stringstream ss;
            ss << "[LEXING_ERROR] " << l << ":" << msg << std::endl;
            return ss.str();
        }
    };
    }
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
"%"  { return yy::parser::make_REMDIV(loc); }
"("  { return yy::parser::make_LPAREN(loc); }
")"  { return yy::parser::make_RPAREN(loc); }
"|"  { return yy::parser::make_ABS(loc); }

"="  { return yy::parser::make_ISEQ(loc); }
"!="  { return yy::parser::make_NOTEQ(loc); }
">=" { return yy::parser::make_MOREEQ(loc); }
"<=" { return yy::parser::make_LESSEQ(loc); }
">"  { return yy::parser::make_MORE(loc); }
"<"  { return yy::parser::make_LESS(loc); }

"AND"{blank}  { return yy::parser::make_LOGICAND(loc); }
"OR"{blank}  { return yy::parser::make_LOGICOR(loc); }
"!"  { return yy::parser::make_NOT(loc); }

{int} { return make_INTEGER(yytext, loc); }
{float} { return make_FLOAT(yytext, loc); }

{blank}+ { }

.    {
        throw ll::lexing_error(loc, "invalid character: " + std::string(yytext));
     }
<<EOF>>    return yy::parser::make_YYEOF (loc);

%%

yy::parser::symbol_type make_FLOAT(const std::string &s, const yy::parser::location_type& loc) {
    errno = 0;
    char* end;
    float n = strtof(s.c_str(), &end);
    if (errno == ERANGE)
        throw ll::lexing_error(loc, "failed to convert " + s + " to float");
    return yy::parser::make_FLOAT(n, loc);
}
yy::parser::symbol_type make_INTEGER(const std::string &s, const yy::parser::location_type& loc) {
    errno = 0;
    int n = std::stoi(s.c_str());
    if (errno == ERANGE)
        throw ll::lexing_error(loc, "failed to convert " + s + " to int");
    return yy::parser::make_INTEGER(n, loc);
}
void driver::scan_begin() {
    yy_flex_debug = debug_mode_;
    yyin = stdin;
}

void driver::scan_end() {
}

