%{ 
#include "driver.hpp"
#include "parser.tab.hpp"
%}

%option noyywrap nounput noinput batch debug
%{
    yy::parser::symbol_type make_FLOAT(std::string_view s, const yy::parser::location_type& loc);
    yy::parser::symbol_type make_INTEGER(std::string_view s, const yy::parser::location_type& loc);
    yy::parser::symbol_type make_STRING(std::string_view s, const yy::parser::location_type& loc);
%}

EXP ([Ee][-+]?[0-9]+)
float [0-9]+"."[0-9]*{EXP}?|"."?[0-9]+{EXP}?
int "0"|([1-9][0-9]*{EXP}?)
blank [ \t\n]
single_string ("'"([^\\']*(\\.)*)*"'")|("\""([^\\"]*(\\.)*)*"\"")
string ({single_string}{blank}*)*

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
"!=" { return yy::parser::make_NOTEQ(loc); }
">=" { return yy::parser::make_MOREEQ(loc); }
"<=" { return yy::parser::make_LESSEQ(loc); }
">"  { return yy::parser::make_MORE(loc); }
"<"  { return yy::parser::make_LESS(loc); }

"AND"{blank}  { return yy::parser::make_LOGICAND(loc); }
"OR"{blank}  { return yy::parser::make_LOGICOR(loc); }
"!"  { return	 yy::parser::make_NOT(loc); }

{int}    { return make_INTEGER(yytext, loc); }
{float}  { return make_FLOAT(yytext, loc); }
{string}  { return make_STRING(yytext, loc); }

{blank}+ { }

.    {
        throw ll::lexing_error(loc, "invalid character: " + std::string(yytext));
     }
<<EOF>>    return yy::parser::make_YYEOF (loc);

%%

yy::parser::symbol_type make_FLOAT(std::string_view s, const yy::parser::location_type& loc) {
    errno = 0;
    char* end;
    float n = strtof(s.data(), &end);
    if (errno == ERANGE)
        throw ll::lexing_error(loc, "failed to convert string to float");
    return yy::parser::make_FLOAT(n, loc);
}
yy::parser::symbol_type make_INTEGER(std::string_view s, const yy::parser::location_type& loc) {
    errno = 0;
    int n = std::stoi(s.data());
    if (errno == ERANGE)
        throw ll::lexing_error(loc, "failed to convert string to int");
    return yy::parser::make_INTEGER(n, loc);
}
yy::parser::symbol_type make_STRING(std::string_view s, const yy::parser::location_type& loc) {
    std::string result; result.reserve(s.size() - 2);
    char in_string = '\0';
    for(size_t i = 0; i < s.size(); ++i) {
	if(s[i] == '\"' || s[i] == '\'') { // "
	    if(!in_string) {
		in_string = s[i];
		continue;
	    } else if (in_string == s[i]) {
		in_string = '\0';
		continue;
	    }
	}
	if(!in_string) continue;
	if(s[i] == '\\') {
	    switch(s[++i]) {
		case 'n':
		    result.push_back('\n');
		    break;
		case 't':
		    result.push_back('\t');
		    break;
		case '\\':
		    result.push_back('\\');
		    break;
		case '\'':
		    result.push_back('\'');
		    break;
		case '\"': // " -- character to fix broken highlight in .ll file
		    result.push_back('\"');// "
		    break;
		default:
		    result.push_back(s[--i]);
		    break;
	    }
	} else { 
	    result.push_back(s[i]);
	}
    }
    return yy::parser::make_STRING(std::move(result), loc);
}
void driver::scan_begin() {
    yy_flex_debug = debug_mode_;
    yyin = stdin;
}

void driver::scan_end() {
}

