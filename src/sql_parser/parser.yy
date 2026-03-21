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

    template<typename T, typename... Args>
    std::unique_ptr<T> mk(Args&&... args) {
	return std::make_unique<T>(std::forward<Args>(args)...);
    }

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
%nterm <uptr<Condition>> cond
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

query: SELECT cond { $$ = mk<ConditionSelectQuery>(std::move($2)); }
     | SELECT expr { $$ = mk<ExpressionSelectQuery>(std::move($2)); }
     ;

cond: cond LOGICAND cond { $$ = mk<BinaryLogicalCondition>(std::move($1), std::move($3), And); }
    | cond LOGICOR cond { $$ = mk<BinaryLogicalCondition>(std::move($1), std::move($3), Or); }
    | LPAREN cond RPAREN { $$ = mk<UnaryLogicalCondition>(std::move($2), IsTrue); }
    | NOT LPAREN cond RPAREN { $$ = mk<UnaryLogicalCondition>(std::move($3), IsFalse); }
    | expr MOREEQ expr { $$ = mk<CompareCondition>(std::move($1), std::move($3), Ge); }
    | expr LESSEQ expr { $$ = mk<CompareCondition>(std::move($1), std::move($3), Le); }
    | expr ISEQ expr { $$ = mk<CompareCondition>(std::move($1), std::move($3), Eq); }
    | expr NOTEQ expr { $$ = mk<CompareCondition>(std::move($1), std::move($3), Ne); }
    | expr MORE expr { $$ = mk<CompareCondition>(std::move($1), std::move($3), Gt); }
    | expr LESS expr { $$ = mk<CompareCondition>(std::move($1), std::move($3), Lt); }
   ;

expr: value { $$ = std::move($1); }
   | expr PLUS expr { $$ = mk<BinaryMathExpression>(std::move($1), std::move($3), Add); }
   | expr MINUS expr { $$ = mk<BinaryMathExpression>(std::move($1), std::move($3), Sub); }
   | expr MUL expr { $$ = mk<BinaryMathExpression>(std::move($1), std::move($3), Mul); }
   | expr DIV expr { $$ = mk<BinaryMathExpression>(std::move($1), std::move($3), Div); }
   | ABS expr ABS { $$ = mk<UnaryMathExpression>(std::move($2), Abs); }
   | LPAREN expr RPAREN { $$ = std::move($2); }
   | MINUS expr %prec UMINUS { $$ = mk<UnaryMathExpression>(std::move($2), Neg);  }
   | expr REMDIV expr { $$ = mk<BinaryMathExpression>(std::move($1), std::move($3), Remdiv); }
   ;

value: INTEGER { $$ = mk<IntConstExpr>($1); }
     | FLOAT   { $$ = mk<FloatConstExpr>($1); }
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
