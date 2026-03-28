%{ 
#include "driver.hpp"
#include "parser.tab.hpp"
%}

%option noyywrap nounput noinput batch debug
%{
    yy::parser::symbol_type make_FLOAT(std::string_view s, const yy::parser::location_type& loc, const driver& drv);
    yy::parser::symbol_type make_INTEGER(std::string_view s, const yy::parser::location_type& loc, const driver& drv);
    yy::parser::symbol_type make_STRING(std::string_view s, const yy::parser::location_type& loc);
%}

EXP ([Ee][-+]?[0-9]+)
float [0-9]+"."[0-9]*{EXP}?|"."?[0-9]+{EXP}?
int "0"|([1-9][0-9]*{EXP}?)
blank [ \t\n]
single_string ("'"([^\\']*(\\.)*)*"'")|("\""([^\\"]*(\\.)*)*"\"")
incomplete_string ("'"([^\\']*(\\.)*)*)|("\""([^\\"]*(\\.)*)*)
/* " */
string ({single_string}{blank}*)*

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

"AND"{blank} { return yy::parser::make_LOGICAND(loc); }
"OR"{blank}  { return yy::parser::make_LOGICOR(loc); }
"!"	     { return yy::parser::make_NOT(loc); }

{int}    { return make_INTEGER(yytext, loc, drv); }
{float}  { return make_FLOAT(yytext, loc, drv); }
{string}  { return make_STRING(yytext, loc); }
{incomplete_string} { 
    if(drv.more_context_required())
	return yy::parser::make_YYEOF(loc);
    drv.log_error(driver::ErrorStage::Lexing, "Unterminated string");
    return yy::parser::make_YYerror(loc);
}

{blank}+ { }

.    {
        drv.log_error(driver::ErrorStage::Lexing, "Invalid character");
	return yy::parser::make_YYerror(loc);
     }
<<EOF>> { drv.met_eof(); return yy::parser::make_YYEOF (loc); }

%%

template<typename T> requires std::is_arithmetic_v<T>
std::optional<T> make_number(std::string_view s) {
    T result;
    auto [ptr, ec] = std::from_chars(s.data(), s.data() + s.size(), result);
    if (ec == std::errc::result_out_of_range) {
	return std::nullopt;
    }
    return result;
}

yy::parser::symbol_type make_FLOAT(std::string_view s, const yy::parser::location_type& loc, const driver& drv) {
    if(auto num = make_number<FloatType>(s)) {
	return yy::parser::make_FLOAT(*num, loc);
    }
    drv.log_error(driver::ErrorStage::Lexing, "Failed to convert \"" + std::string(s) + "\" to float; too big value");
    return yy::parser::make_YYerror(loc);
}
yy::parser::symbol_type make_INTEGER(std::string_view s, const yy::parser::location_type& loc, const driver& drv) {
    if(auto num = make_number<IntType>(s)) {
	return yy::parser::make_INTEGER(*num, loc);
    }
    drv.log_error(driver::ErrorStage::Lexing, "Failed to convert \"" + std::string(s) + "\" to int; too big value");
    return yy::parser::make_YYerror(loc);
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
    is_eof_ = false;
    more_context_required_ = false;
    yy_scan_bytes(query_.data(), static_cast<int>(query_.size()));
}

void driver::scan_end() {
    yy_delete_buffer(YY_CURRENT_BUFFER);
}

