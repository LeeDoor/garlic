%skeleton "lalr1.cc"
%language "c++"
%require "3.8.1"
%header
%define api.value.type variant
%define api.token.constructor
%define parse.assert

%code requires {
    #include "cell_type.hpp"
    #include "string_query_result.hpp"
    #include "expression_select_query.hpp"
    #include "condition_select_query.hpp"
    #include "compare_condition.hpp"
    #include "binary_logical_condition.hpp"
    #include "unary_logical_condition.hpp"
    #include "binary_math_expression.hpp"
    #include "unary_math_expression.hpp"
    #include "constant_expression.hpp"


    namespace garlic::sql_parser { class ParsingSession; }
    using namespace garlic;
    using namespace garlic::sql_parser;
}

%param { ParsingSession& session }
%locations

%define parse.trace
%define parse.error detailed
%define parse.lac full

%code {
    #include "parsing_session.hpp"

    template<typename T, typename... Args>
    std::unique_ptr<T> mk_v(ParsingSession& session, Args&&... args) {
	auto obj = std::make_unique<T>(std::forward<Args>(args)...);
	if constexpr (requires (const T& t) { t.validate(); }) {
	    if(auto error = obj->validate()) {
		session.invoke_error(ErrorStage::SemanticAnalysis, *error);
		return nullptr;
	    }
	}
	return obj;
    }

    #define ASSIGN_OR_ABORT(dst, expr) \
	do {                           \
	    auto _tmp = (expr);        \
	    if(!_tmp) YYERROR;         \
	    (dst) = std::move(_tmp);   \
	} while(0)
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
%token <FloatType> FLOAT "float"
%token <IntType> INTEGER "integer"
%token <StringType> STRING "string"
%nterm <uptr<Query>> query
%nterm <uptr<Condition>> cond
%nterm <uptr<Expression>> expr 
%nterm <uptr<Expression>> value 
%nterm <StringType> strings 
%printer { yyo << $$; } <*>;

%%

queries: /**/
    | queries query SEMICOLON { session.query_parsed(std::move($2)); }
    | queries error SEMICOLON { yyerrok; yyclearin; session.error_parsed(); }
    | queries SEMICOLON { session.blank_parsed(); }
    ;

query: SELECT cond { ASSIGN_OR_ABORT($$, mk_v<ConditionSelectQuery>(session, std::move($2))); }
     | SELECT expr { ASSIGN_OR_ABORT($$, mk_v<ExpressionSelectQuery>(session, std::move($2))); }
     ;

cond: cond LOGICAND cond { ASSIGN_OR_ABORT($$, mk_v<BinaryLogicalCondition>(session, std::move($1), std::move($3), And)); }
    | cond LOGICOR cond { ASSIGN_OR_ABORT($$, mk_v<BinaryLogicalCondition>(session, std::move($1), std::move($3), Or)); }
    | LPAREN cond RPAREN { ASSIGN_OR_ABORT($$, mk_v<UnaryLogicalCondition>(session, std::move($2), IsTrue)); }
    | NOT LPAREN cond RPAREN { ASSIGN_OR_ABORT($$, mk_v<UnaryLogicalCondition>(session, std::move($3), IsFalse)); }
    | expr MOREEQ expr { ASSIGN_OR_ABORT($$, mk_v<CompareCondition>(session, std::move($1), std::move($3), Ge)); }
    | expr LESSEQ expr { ASSIGN_OR_ABORT($$, mk_v<CompareCondition>(session, std::move($1), std::move($3), Le)); }
    | expr ISEQ expr { ASSIGN_OR_ABORT($$, mk_v<CompareCondition>(session, std::move($1), std::move($3), Eq)); }
    | expr NOTEQ expr { ASSIGN_OR_ABORT($$, mk_v<CompareCondition>(session, std::move($1), std::move($3), Ne)); }
    | expr MORE expr { ASSIGN_OR_ABORT($$, mk_v<CompareCondition>(session, std::move($1), std::move($3), Gt)); }
    | expr LESS expr { ASSIGN_OR_ABORT($$, mk_v<CompareCondition>(session, std::move($1), std::move($3), Lt)); }
   ;

expr: value { $$ = std::move($1); }
   | expr PLUS expr { ASSIGN_OR_ABORT($$, mk_v<BinaryMathExpression>(session, std::move($1), std::move($3), Add)); }
   | expr MINUS expr { ASSIGN_OR_ABORT($$, mk_v<BinaryMathExpression>(session, std::move($1), std::move($3), Sub)); }
   | expr MUL expr { ASSIGN_OR_ABORT($$, mk_v<BinaryMathExpression>(session, std::move($1), std::move($3), Mul)); }
   | expr DIV expr { ASSIGN_OR_ABORT($$, mk_v<BinaryMathExpression>(session, std::move($1), std::move($3), Div)); }
   | ABS expr ABS { ASSIGN_OR_ABORT($$, mk_v<UnaryMathExpression>(session, std::move($2), Abs)); }
   | LPAREN expr RPAREN { $$ = std::move($2); }
   | MINUS expr %prec UMINUS { ASSIGN_OR_ABORT($$, mk_v<UnaryMathExpression>(session, std::move($2), Neg)); }
   | expr REMDIV expr { ASSIGN_OR_ABORT($$, mk_v<BinaryMathExpression>(session, std::move($1), std::move($3), Remdiv)); }
   ;

value: INTEGER { ASSIGN_OR_ABORT($$, mk_v<IntConstExpr>(session, $1)); }
     | FLOAT   { ASSIGN_OR_ABORT($$, mk_v<FloatConstExpr>(session, $1)); }
     | strings { ASSIGN_OR_ABORT($$, mk_v<StringConstExpr>(session, $1)); }
     ;

strings: STRING { $$ = $1; }
       | strings STRING { $$ = $1 + $2; }
       ;

%left LOGICOR;
%left LOGICAND;
%left NOT;
%left PLUS MINUS;
%left MUL DIV REMDIV;
%left UMINUS;

%%
void yy::parser::error(const location_type&, const std::string& m) {
    session.invoke_error(ErrorStage::Parsing, m);
}
