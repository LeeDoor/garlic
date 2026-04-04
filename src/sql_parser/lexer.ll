%{ 
#include "driver.hpp"
#include "parser.tab.hpp"
%}

%option noyywrap nounput noinput batch debug
%{
    yy::parser::symbol_type make_FLOAT(std::string_view s, yy::parser::location_type& curloc, driver& drv);
    yy::parser::symbol_type make_INTEGER(std::string_view s, yy::parser::location_type& curloc, driver& drv);
    yy::parser::symbol_type make_STRING(std::string& s, yy::parser::location_type& curloc);
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
	curloc.columns(1); \
	drv.memorize_token_begin_loc(); \
	--left_ok; \
	curloc.columns (yyleng - 1); curloc.step(); \
    } while(0);

    #define WHITESPACE_SEPARATED(TOKEN_NAME) \
    do { \
	if(left_ok < 0) { \
	    LEXING_ERROR("Token " TOKEN_NAME " should be whitespace-separated"); \
	} \
    } while(0);

    #define LEXING_ERROR(msg) do { \
        curloc.columns(-yyleng); curloc.step(); \
	drv.invoke_error(ErrorStage::Lexing, msg); \
	return yy::parser::make_YYerror(curloc); \
    } while(0);
%}

%%

%{
    auto& location = drv.location();
    auto& curloc = location.cur();
    curloc.step();
    static std::string multiline_str;
    // 1 when seen a whitespace; 
    // decremented in YY_USER_ACTION; 
    // if == 0, it means the last character was a whitespace; 
    // if < 0, it wasn't
    static int left_ok = 1; 
    BEGIN INITIAL;
%}

"SELECT"/({token_separator}) { WHITESPACE_SEPARATED("SELECT"); return yy::parser::make_SELECT(curloc); }

";"  { return yy::parser::make_SEMICOLON(curloc); }
"-"  { return yy::parser::make_MINUS(curloc); }
"+"  { return yy::parser::make_PLUS(curloc); }
"*"  { return yy::parser::make_MUL(curloc); }
"/"  { return yy::parser::make_DIV(curloc); }
"%"  { return yy::parser::make_REMDIV(curloc); }
"("  { return yy::parser::make_LPAREN(curloc); }
")"  { return yy::parser::make_RPAREN(curloc); }
"|"  { return yy::parser::make_ABS(curloc); }

"="  { return yy::parser::make_ISEQ(curloc); }
"!=" { return yy::parser::make_NOTEQ(curloc); }
">=" { return yy::parser::make_MOREEQ(curloc); }
"<=" { return yy::parser::make_LESSEQ(curloc); }
">"  { return yy::parser::make_MORE(curloc); }
"<"  { return yy::parser::make_LESS(curloc); }

"AND"/({token_separator}) { WHITESPACE_SEPARATED("AND"); return yy::parser::make_LOGICAND(curloc); }
"OR"/({token_separator})  { WHITESPACE_SEPARATED("OR"); return yy::parser::make_LOGICOR(curloc); }
"!"			{ return yy::parser::make_NOT(curloc); }

{int}    { return make_INTEGER(yytext, curloc, drv); }
{float}  { return make_FLOAT(yytext, curloc, drv); }

{string_quote_q} { multiline_str += yytext; BEGIN STRING_Q; }
<STRING_Q>{string_content_q} { multiline_str += yytext; }
<STRING_Q>{EOL} { curloc.lines(); curloc.step(); multiline_str += yytext; }
<STRING_Q><<EOF>> {
    drv.met_eof();
    LEXING_ERROR("Unterminated string");
}
<STRING_Q>{string_quote_q} { 
    multiline_str += yytext; 
    BEGIN INITIAL; 
    return make_STRING(multiline_str, curloc); 
}

{string_quote_d} { multiline_str += yytext; BEGIN STRING_D; }
<STRING_D>{string_content_d} { multiline_str += yytext; }
<STRING_D>{EOL} { curloc.lines(); curloc.step(); multiline_str += yytext; }
<STRING_D><<EOF>> {
    drv.met_eof();
    LEXING_ERROR("Unterminated string");
}
<STRING_D>{string_quote_d} { 
    multiline_str += yytext; 
    BEGIN INITIAL; 
    return make_STRING(multiline_str, curloc);
}

{EOL} { left_ok = 1; curloc.lines(); curloc.step(); }
{blank}+ { left_ok = 1; curloc.step(); }

.    {
	LEXING_ERROR("Invalid character \"" + std::string(yytext) + "\"");
     }
<<EOF>> {
    drv.met_eof(); 
    return yy::parser::make_YYEOF (curloc); 
}

%%

template<typename T> requires std::is_arithmetic_v<T>
std::optional<T> make_number(std::string_view s) {
    T result;
    auto [ptr, ec] = std::from_chars(s.data(), s.data() + s.size(), result);
    if (ec == std::errc::result_out_of_range) {
	return std::nullopt;
    } else if (ec == std::errc::invalid_argument) {
	throw std::logic_error("Regular expression matched wrong number");
    }
    return result;
}

yy::parser::symbol_type make_FLOAT(std::string_view s, yy::parser::location_type& curloc, driver& drv) {
    if(auto num = make_number<FloatType>(s)) {
	return yy::parser::make_FLOAT(*num, curloc);
    }
    LEXING_ERROR("Failed to convert \"" + std::string(s) + "\" to float; too big value");
}
yy::parser::symbol_type make_INTEGER(std::string_view s, yy::parser::location_type& curloc, driver& drv) {
    if(auto num = make_number<IntType>(s)) {
	return yy::parser::make_INTEGER(*num, curloc);
    }
    LEXING_ERROR("Failed to convert \"" + std::string(s) + "\" to int; too big value");
}
yy::parser::symbol_type make_STRING(std::string& s, yy::parser::location_type& curloc) {
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
    return yy::parser::make_STRING(std::move(result), curloc);
}

void driver::scan_begin() {
    yy_flex_debug = debug_mode_;
    auto string_view = query_io_.get_query();
    yy_scan_bytes(string_view.data(), string_view.size());
}

void driver::scan_end() {
    yy_delete_buffer(YY_CURRENT_BUFFER);
}

