%skeleton "lalr1.cc"
%language "c++"
%require "3.8.1"
%header
%define api.value.type variant
%define api.token.constructor
%define parse.assert

%code requires {
    #include "value.hpp"
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

    using namespace garlic;

    namespace garlic::sql_parser { class driver; }
    using namespace garlic::sql_parser;
}

%param { driver& drv }
%locations

%define parse.trace
%define parse.error detailed
%define parse.lac full

%code {
    #include "driver.hpp"

    class DumbTableValueGatherer : public TableValueGatherer {
    public:
	sptr<CellValue> get_table_value(const std::string& ) override {
	    throw std::logic_error("Not supposed to be invoked; stub class");
	}
    };

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
%nterm <uptr<Query>> query
%nterm <uptr<Condition>> comp
%nterm <uptr<Expression>> expr 
%nterm <uptr<Expression>> value 
%printer { yyo << $$; } <*>;

%%

queries: /**/
    | queries query SEMICOLON { 
	auto gatherer = std::make_shared<DumbTableValueGatherer>(); 
	auto result = $2->resolve(gatherer); 
	std::cout << result->format() << std::endl;
    }
    | queries SEMICOLON
    ;

query: SELECT comp { $$ = std::make_unique<ConditionSelectQuery>(std::move($2)); }
     | SELECT expr { $$ = std::make_unique<ExpressionSelectQuery>(std::move($2)); }
     ;

comp: comp LOGICAND comp { $$ = std::make_unique<BinaryLogicalCondition>(std::move($1), std::move($3), And); }
    | comp LOGICOR comp { $$ = std::make_unique<BinaryLogicalCondition>(std::move($1), std::move($3), Or); }
    | LPAREN comp RPAREN { $$ = std::make_unique<UnaryLogicalCondition>(std::move($2), IsFalse); }
    | NOT LPAREN comp RPAREN { $$ = std::make_unique<UnaryLogicalCondition>(std::move($3), IsFalse); }
    | expr MOREEQ expr { $$ = std::make_unique<CompareCondition>(std::move($1), std::move($3), Ge); }
    | expr LESSEQ expr { $$ = std::make_unique<CompareCondition>(std::move($1), std::move($3), Le); }
    | expr ISEQ expr { $$ = std::make_unique<CompareCondition>(std::move($1), std::move($3), Equals); }
    | expr NOTEQ expr { $$ = std::make_unique<CompareCondition>(std::move($1), std::move($3), Ne); }
    | expr MORE expr { $$ = std::make_unique<CompareCondition>(std::move($1), std::move($3), Gt); }
    | expr LESS expr { $$ = std::make_unique<CompareCondition>(std::move($1), std::move($3), Lt); }
   ;

expr: value { $$ = std::move($1); }
   | expr PLUS expr { $$ = std::make_unique<BinaryMathExpression>(std::move($1), std::move($3), ADD); }
   | expr MINUS expr { $$ = std::make_unique<BinaryMathExpression>(std::move($1), std::move($3), SUB); }
   | expr MUL expr { $$ = std::make_unique<BinaryMathExpression>(std::move($1), std::move($3), MUL); }
   | expr DIV expr { $$ = std::make_unique<BinaryMathExpression>(std::move($1), std::move($3), DIV); }
   | ABS expr ABS { $$ = std::make_unique<UnaryMathExpression>(std::move($2), ABS); }
   | LPAREN expr RPAREN { $$ = std::move($2); }
   | MINUS expr %prec UMINUS { $$ = std::make_unique<UnaryMathExpression>(std::move($2), NEG);  }
   | expr REMDIV expr { $$ = std::make_unique<BinaryMathExpression>(std::move($1), std::move($3), REMDIV); }
   ;

value: INTEGER { $$ = std::make_unique<IntConstExpr>($1); }
     | FLOAT   { $$ = std::make_unique<FloatConstExpr>($1); }
     ;

%left LOGICOR;
%left LOGICAND;
%left NOT;
%left PLUS MINUS;
%left MUL DIV REMDIV;
%left UMINUS;

%%
void yy::parser::error (const location_type& l, const std::string& m) {
  std::cerr << "[SYNTAX_ERROR] " << l << ": " << m << '\n';
}
