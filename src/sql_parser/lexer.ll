%{ 
#include "parsing_session.hpp"
#include "parser_engine.hpp"
#include "parser.tab.hpp"
%}

%option noyywrap nounput noinput batch debug
%{
    #include "literals_converter.hpp"
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
    #define YY_USER_ACTION { \
	session.on_each_token(); \
    }
/// should be pasted in every newline 
/// character; used for location accounting
    #define MET_NEWLINE() { \
	session.met_newline(yyleng); \
    }    
/// should be pasted in every meaningful token;
/// used to track content query start location.
    #define MET_CONTENT() { \
	session.met_content(yyleng); \
    }

/// should be pasted in every space, \t or similar;
/// used to modify location.
    #define MET_SPACE() { \
	session.met_space(yyleng); \
    } 
/// should be pasted in every word-separating whitespace;
/// used to distinguish words like SELECT<>AND<>OR
    #define MET_WHITESPACE() \
	{ session.met_word_delimeter(); }

/// should be pasted if token should be whitespace separated;
/// i.e. to avoid "true ANDOR false".
    #define WHITESPACE_SEPARATED(TOKEN_NAME) { \
	if(auto res = session.whitespace_separated(TOKEN_NAME)) { \
	    return std::move(*res); \
	} \
    }
/// triggers lexical error with given message
    #define LEXING_ERROR(msg) { \
	return session.lexing_error(msg); \
    } 
%}

%%

%{
    auto& curloc = session.location().cur();
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

{int}    { MET_CONTENT(); return make_INTEGER(yytext, curloc, session); }
{float}  { MET_CONTENT(); return make_FLOAT(yytext, curloc, session); }

{string_quote_q} { MET_CONTENT(); session.append_line(yytext); BEGIN STRING_Q; }
<STRING_Q>{string_content_q} { session.append_line(yytext); }
<STRING_Q>{EOL} { MET_NEWLINE(); session.append_line(yytext); }
<STRING_Q><<EOF>> {
    BEGIN INITIAL;
    session.met_eof();
    LEXING_ERROR("Unterminated string");
}
<STRING_Q>{string_quote_q} { 
    session.append_line(yytext); 
    BEGIN INITIAL; 
    return make_STRING(session.get_multiline_string(), curloc); 
}

{string_quote_d} { MET_CONTENT(); session.append_line(yytext); BEGIN STRING_D; }
<STRING_D>{string_content_d} { session.append_line(yytext); }
<STRING_D>{EOL} { MET_NEWLINE(); session.append_line(yytext); }
<STRING_D><<EOF>> {
    BEGIN INITIAL;
    session.met_eof();
    LEXING_ERROR("Unterminated string");
}
<STRING_D>{string_quote_d} { 
    session.append_line(yytext); 
    BEGIN INITIAL; 
    return make_STRING(session.get_multiline_string(), curloc);
}

{EOL} { MET_NEWLINE(); MET_WHITESPACE(); }
{blank}+ { MET_SPACE(); MET_WHITESPACE(); }

.    {
	MET_CONTENT(); LEXING_ERROR("Invalid character \"" + std::string(yytext) + "\"");
     }
<<EOF>> {
    session.met_eof(); 
    return yy::parser::make_YYEOF (curloc); 
}

%%

void ParserEngine::scan_begin(StringViewType query_string) {
    yy_flex_debug = debug_mode_;
    yy_scan_bytes(query_string.data(), query_string.size());
}

void ParserEngine::scan_end() {
    yy_delete_buffer(YY_CURRENT_BUFFER);
}

