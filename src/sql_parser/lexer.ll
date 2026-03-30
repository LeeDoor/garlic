%{ 
#include "driver.hpp"
#include "parser.tab.hpp"
%}

%option noyywrap nounput noinput batch debug
%{
    yy::parser::symbol_type make_FLOAT(std::string_view s, const yy::parser::location_type& loc, driver& drv);
    yy::parser::symbol_type make_INTEGER(std::string_view s, const yy::parser::location_type& loc, driver& drv);
    yy::parser::symbol_type make_STRING(std::string& s, const yy::parser::location_type& loc);
%}

%x STRING_Q
%x STRING_D

EOL "\n"
EXP ([Ee][-+]?[0-9]+)
float [0-9]+"."[0-9]*{EXP}?|"."?[0-9]+{EXP}?
int "0"|([1-9][0-9]*{EXP}?)
blank [ \t]
string_quote_q "'"
string_quote_d "\""
/* " */
string_content_q ([^\\'\n]*(\\.)*)*
string_content_d ([^\\"\n]*(\\.)*)*
/* " */


%{
    #define YY_USER_ACTION do { loc.columns (yyleng); loc.step(); } while(0);
%}

%%

%{
  yy::location& loc = drv.location();
  loc.step();
  std::string multiline_str;
  BEGIN INITIAL;
%}

"SELECT"({blank}|{EOL}) { return yy::parser::make_SELECT(loc); }

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

{string_quote_q} { multiline_str += yytext; BEGIN STRING_Q; }
<STRING_Q>{string_content_q} { multiline_str += yytext; }
<STRING_Q>{EOL} { loc.lines(); loc.step(); multiline_str += yytext; }
<STRING_Q><<EOF>> {
    drv.met_eof();
    drv.invoke_error(driver::ErrorStage::Lexing, "Unterminated string");
    return yy::parser::make_YYerror(loc);
}
<STRING_Q>{string_quote_q} { 
    multiline_str += yytext; 
    BEGIN INITIAL; 
    return make_STRING(multiline_str, loc); 
}

{string_quote_d} { multiline_str += yytext; BEGIN STRING_D; }
<STRING_D>{string_content_d} { multiline_str += yytext; }
<STRING_D>{EOL} { loc.lines(); loc.step(); multiline_str += yytext; }
<STRING_D><<EOF>> {
    drv.met_eof();
    drv.invoke_error(driver::ErrorStage::Lexing, "Unterminated string");
    return yy::parser::make_YYerror(loc);
}
<STRING_D>{string_quote_d} { 
    multiline_str += yytext; 
    BEGIN INITIAL; 
    return make_STRING(multiline_str, loc);
}

{EOL} { loc.lines(); loc.step(); }
{blank}+ { loc.step(); }

.    {
        drv.invoke_error(driver::ErrorStage::Lexing, "Invalid character \"" + std::string(yytext) + "\"");
	return yy::parser::make_YYerror(loc);
     }
<<EOF>> {
    drv.met_eof(); 
    return yy::parser::make_YYEOF (loc); 
}

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

yy::parser::symbol_type make_FLOAT(std::string_view s, const yy::parser::location_type& loc, driver& drv) {
    if(auto num = make_number<FloatType>(s)) {
	return yy::parser::make_FLOAT(*num, loc);
    }
    drv.invoke_error(driver::ErrorStage::Lexing, "Failed to convert \"" + std::string(s) + "\" to float; too big value");
    return yy::parser::make_YYerror(loc);
}
yy::parser::symbol_type make_INTEGER(std::string_view s, const yy::parser::location_type& loc, driver& drv) {
    if(auto num = make_number<IntType>(s)) {
	return yy::parser::make_INTEGER(*num, loc);
    }
    drv.invoke_error(driver::ErrorStage::Lexing, "Failed to convert \"" + std::string(s) + "\" to int; too big value");
    return yy::parser::make_YYerror(loc);
}
yy::parser::symbol_type make_STRING(std::string& s, const yy::parser::location_type& loc) {
    std::string result; result.reserve(s.size() - 2);
    for(size_t i = 1; i < s.size() - 1; ++i) {
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
    s.clear();
    return yy::parser::make_STRING(std::move(result), loc);
}

void driver::scan_begin() {
    yy_flex_debug = debug_mode_;
    yy_scan_bytes(query_.data(), static_cast<int>(query_.size()));
}

void driver::scan_end() {
    yy_delete_buffer(YY_CURRENT_BUFFER);
}

