%{ 
#include "driver.hpp"
#include "parser.tab.hpp"
%}

%option noyywrap nounput noinput batch debug
%{
    yy::parser::symbol_type make_FLOAT(std::string_view s, yy::parser::location_type& loc, driver& drv);
    yy::parser::symbol_type make_INTEGER(std::string_view s, yy::parser::location_type& loc, driver& drv);
    yy::parser::symbol_type make_STRING(std::string& s, yy::parser::location_type& loc);
%}

%x STRING_Q
%x STRING_D

EOL "\n"
blank [ \t]
token_separator {EOL}|{blank}|$
EXP ([Ee][-+]?[0-9]+)
float [0-9]+"."[0-9]*{EXP}?|"."?[0-9]+{EXP}?
int "0"|([1-9][0-9]*{EXP}?)
string_quote_q "'"
string_quote_d "\""
/* " */
string_content_q ([^\\'\n]*(\\.)*)*
string_content_d ([^\\"\n]*(\\.)*)*
/* " */


%{
    #define YY_USER_ACTION do { \
	loc.columns(1); \
	drv.memorize_token_begin_loc(); \
	--left_ok; \
	loc.columns (yyleng - 1); loc.step(); \
    } while(0);

    #define WHITESPACE_SEPARATED(TOKEN_NAME) \
    do { \
	if(left_ok < 0) { \
	    LEXING_ERROR("Token " TOKEN_NAME " should be whitespace-separated"); \
	} \
    } while(0);

    #define LEXING_ERROR(msg) do { \
        loc.columns(-yyleng); loc.step(); \
	drv.invoke_error(ErrorStage::Lexing, msg); \
	return yy::parser::make_YYerror(loc); \
    } while(0);
%}

%%

%{
    yy::location& loc = drv.location();
    loc.step();
    static std::string multiline_str;
    // 1 when seen a whitespace; 
    // decremented in YY_USER_ACTION; 
    // if == 0, it means the last character was a whitespace; 
    // if < 0, it wasn't
    static int left_ok = 1; 
    BEGIN INITIAL;
%}

"SELECT"/({token_separator}) { WHITESPACE_SEPARATED("SELECT"); return yy::parser::make_SELECT(loc); }

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

"AND"/({token_separator}) { WHITESPACE_SEPARATED("AND"); return yy::parser::make_LOGICAND(loc); }
"OR"/({token_separator})  { WHITESPACE_SEPARATED("OR"); return yy::parser::make_LOGICOR(loc); }
"!"			{ return yy::parser::make_NOT(loc); }

{int}    { return make_INTEGER(yytext, loc, drv); }
{float}  { return make_FLOAT(yytext, loc, drv); }

{string_quote_q} { multiline_str += yytext; BEGIN STRING_Q; }
<STRING_Q>{string_content_q} { multiline_str += yytext; }
<STRING_Q>{EOL} { loc.lines(); loc.step(); multiline_str += yytext; }
<STRING_Q><<EOF>> {
    drv.met_eof();
    LEXING_ERROR("Unterminated string");
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
    LEXING_ERROR("Unterminated string");
}
<STRING_D>{string_quote_d} { 
    multiline_str += yytext; 
    BEGIN INITIAL; 
    return make_STRING(multiline_str, loc);
}

{EOL} { left_ok = 1; loc.lines(); loc.step(); }
{blank}+ { left_ok = 1; loc.step(); }

.    {
	LEXING_ERROR("Invalid character \"" + std::string(yytext) + "\"");
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

yy::parser::symbol_type make_FLOAT(std::string_view s, yy::parser::location_type& loc, driver& drv) {
    if(auto num = make_number<FloatType>(s)) {
	return yy::parser::make_FLOAT(*num, loc);
    }
    LEXING_ERROR("Failed to convert \"" + std::string(s) + "\" to float; too big value");
}
yy::parser::symbol_type make_INTEGER(std::string_view s, yy::parser::location_type& loc, driver& drv) {
    if(auto num = make_number<IntType>(s)) {
	return yy::parser::make_INTEGER(*num, loc);
    }
    LEXING_ERROR("Failed to convert \"" + std::string(s) + "\" to int; too big value");
}
yy::parser::symbol_type make_STRING(std::string& s, yy::parser::location_type& loc) {
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
    auto [data, size] = query_io_.get_query();
    yy_scan_bytes(data, size);
}

void driver::scan_end() {
    yy_delete_buffer(YY_CURRENT_BUFFER);
}

