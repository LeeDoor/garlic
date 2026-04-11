%{ 
#include "parsing_context.hpp"
#include "parser.tab.hpp"
%}

%option noyywrap nounput noinput batch debug
%{
    yy::parser::symbol_type make_FLOAT(std::string_view s, Position& curloc, ParsingContext& ctx);
    yy::parser::symbol_type make_INTEGER(std::string_view s, Position& curloc, ParsingContext& ctx);
    yy::parser::symbol_type make_STRING(std::string& s, Position& curloc);
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
string_content_q ([^\\'\n]*(\\.)*)*
string_content_d ([^\\"\n]*(\\.)*)*

%{
/// Executes before each rule
    #define YY_USER_ACTION do { \
	location.on_token_start(); \
	--left_ok; \
    } while(0);

/// should be pasted in every newline 
/// character; used for location accounting
    #define MET_NEWLINE() do { \
	curloc.lines(yyleng); \
	location.on_line_start(); \
    } while(0);
    
/// should be pasted in every meaningful token;
/// used to track content query start location.
    #define MET_CONTENT() do { \
	if(waiting_query_content) { \
	    waiting_query_content = false; \
	    location.on_content_query_start(); \
	} \
	curloc.columns(yyleng); \
    } while(0); 

/// should be pasted in every space, \t or similar;
/// used to modify location.
    #define MET_SPACE() do { \
	curloc.columns(yyleng); \
    } while(0); 

/// should be pasted in every word-separating whitespace;
/// used to distinguish words like SELECT<>AND<>OR
    #define MET_WHITESPACE() \
    { left_ok = 1; }

/// should be pasted if token should be whitespace separated;
/// i.e. to avoid "true ANDOR false".
    #define WHITESPACE_SEPARATED(TOKEN_NAME) \
    do { \
	if(left_ok < 0) { \
	    LEXING_ERROR("Token " TOKEN_NAME " should be whitespace-separated"); \
	} \
    } while(0);

/// triggers lexical error with given message
    #define LEXING_ERROR(msg) do { \
	ctx.invoke_error(ErrorStage::Lexing, msg); \
	return yy::parser::make_YYerror(curloc); \
    } while(0);
%}

%%

%{
    auto& context = ctx.context();
    auto& location = context.location;
    auto& curloc = location.cur();
    auto& multiline_str = context.multiline_string_buffer;
    // set to 1 when seen a whitespace; 
    // decremented in YY_USER_ACTION; 
    // if == 0, it means the last character was a whitespace; 
    // if < 0, it wasn't
    auto& left_ok = context.left_ok; 
    auto& waiting_query_content = context.waiting_query_content;
%}

"SELECT"/({token_separator}) { MET_CONTENT(); WHITESPACE_SEPARATED("SELECT"); return yy::parser::make_SELECT(curloc); }

";"  { MET_CONTENT(); return yy::parser::make_SEMICOLON(curloc); }
"-"  { MET_CONTENT(); return yy::parser::make_MINUS(curloc); }
"+"  { MET_CONTENT(); return yy::parser::make_PLUS(curloc); }
"*"  { MET_CONTENT(); return yy::parser::make_MUL(curloc); }
"/"  { MET_CONTENT(); return yy::parser::make_DIV(curloc); }
"%"  { MET_CONTENT(); return yy::parser::make_REMDIV(curloc); }
"("  { MET_CONTENT(); return yy::parser::make_LPAREN(curloc); }
")"  { MET_CONTENT(); return yy::parser::make_RPAREN(curloc); }
"|"  { MET_CONTENT(); return yy::parser::make_ABS(curloc); }

"="  { MET_CONTENT(); return yy::parser::make_ISEQ(curloc); }
"!=" { MET_CONTENT(); return yy::parser::make_NOTEQ(curloc); }
">=" { MET_CONTENT(); return yy::parser::make_MOREEQ(curloc); }
"<=" { MET_CONTENT(); return yy::parser::make_LESSEQ(curloc); }
">"  { MET_CONTENT(); return yy::parser::make_MORE(curloc); }
"<"  { MET_CONTENT(); return yy::parser::make_LESS(curloc); }

"AND"/({token_separator}) { MET_CONTENT(); WHITESPACE_SEPARATED("AND"); return yy::parser::make_LOGICAND(curloc); }
"OR"/({token_separator})  { MET_CONTENT(); WHITESPACE_SEPARATED("OR"); return yy::parser::make_LOGICOR(curloc); }
"!"			  { MET_CONTENT(); return yy::parser::make_NOT(curloc); }

{int}    { MET_CONTENT(); return make_INTEGER(yytext, curloc, ctx); }
{float}  { MET_CONTENT(); return make_FLOAT(yytext, curloc, ctx); }

{string_quote_q} { MET_CONTENT(); multiline_str += yytext; BEGIN STRING_Q; }
<STRING_Q>{string_content_q} { multiline_str += yytext; }
<STRING_Q>{EOL} { MET_NEWLINE(); multiline_str += yytext; }
<STRING_Q><<EOF>> {
    BEGIN INITIAL;
    ctx.met_eof();
    LEXING_ERROR("Unterminated string");
}
<STRING_Q>{string_quote_q} { 
    multiline_str += yytext; 
    BEGIN INITIAL; 
    return make_STRING(multiline_str, curloc); 
}

{string_quote_d} { MET_CONTENT(); multiline_str += yytext; BEGIN STRING_D; }
<STRING_D>{string_content_d} { multiline_str += yytext; }
<STRING_D>{EOL} { MET_NEWLINE(); multiline_str += yytext; }
<STRING_D><<EOF>> {
    BEGIN INITIAL;
    ctx.met_eof();
    LEXING_ERROR("Unterminated string");
}
<STRING_D>{string_quote_d} { 
    multiline_str += yytext; 
    BEGIN INITIAL; 
    return make_STRING(multiline_str, curloc);
}

{EOL} { MET_NEWLINE(); MET_WHITESPACE(); }
{blank}+ { MET_SPACE(); MET_WHITESPACE(); }

.    {
	MET_CONTENT(); LEXING_ERROR("Invalid character \"" + std::string(yytext) + "\"");
     }
<<EOF>> {
    ctx.met_eof(); 
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

yy::parser::symbol_type make_FLOAT(std::string_view s, Position& curloc, ParsingContext& ctx) {
    if(auto num = make_number<FloatType>(s)) {
	return yy::parser::make_FLOAT(*num, curloc);
    }
    LEXING_ERROR("Failed to convert \"" + std::string(s) + "\" to float; too big value");
}
yy::parser::symbol_type make_INTEGER(std::string_view s, Position& curloc, ParsingContext& ctx) {
    if(auto num = make_number<IntType>(s)) {
	return yy::parser::make_INTEGER(*num, curloc);
    }
    LEXING_ERROR("Failed to convert \"" + std::string(s) + "\" to int; too big value");
}
yy::parser::symbol_type make_STRING(std::string& s, Position& curloc) {
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

void ParsingContext::scan_begin(StringViewType query_string) {
    yy_flex_debug = debug_mode_;
    yy_scan_bytes(query_string.data(), query_string.size());
}

void ParsingContext::scan_end() {
    yy_delete_buffer(YY_CURRENT_BUFFER);
}

