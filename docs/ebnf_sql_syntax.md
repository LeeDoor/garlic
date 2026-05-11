> All keywords that will appear in rules below can be written in any case with different cases for each letter.
> These are correct keywords: `SELECT`, `select`, `Select`, `SELEct`, `SeLeCt` and so on.

# Errors
### Lexical Error
If any character doesn't match any non-terminal, this character is called *invalid* and query terminates with **Lexical Error**.
Example: `SELICT 12;`. Here the `SELICT` word can't be recognized as a `SELECT`, so this word is invalid and we consider the query contains lexical error.

### Syntax Error
If all characters are recognized as non-terminals but they can't form the resulting non-terminal, the query terminates with **Syntax Error**.
Example: `SELECT 12 * / 5;`. Here all tokens are valid and recognized, but `12 * / 5` can't match any expression rule, so 
we consider the query contains syntax error.

### Semantic Error
If parsing input corresponds the grammar but still ill-formed by some rules, the query terminates with **Semantic Error**. 
The error is recognized as semantic **only if it violates the rule under #REQUIREMENTS section**. Other text aims to explain the grammar rules.
1. Example: `SELECT "hello" + 5;`. Here the query corresponds to the grammar, but the addition of the string and the number is forbidden,
so we consider the query contains semantic error.
2. Another example: `SELECT user.name FROM users, users;`. Keeping same table names in one `FROM` clause is forbidden - Semantic Error.

### Runtime Error
If parsing input is grammatically valid and doesn't violate any rules but under some circumstances the query execution will lead to an error,
then we consider the query contains **Runtime Error**.
Example: `SELECT users.salary / users.debt FROM users;`. This query looks valid, but in some cases if we got unlucky some user may not have any debts,
so we face the division by zero and the query fails with runtime error.
Another example: `SELECT users.salary FROM users;`. This is completely valid query, but if we don't have a table with name `users` this 
query fill fail with runtime error.

### Panic Error Handling Rules
If parser meets any error while parsing, it panics and skips all input until next query delimiter (AKA semicolon ";").
When such delimiter is found, parsing process continues as usual.

# Primitives
## Defining a Value

The `value` can be either a positive number, string, or a column reference. 

The value can be a reference to table's column.

The number is always positive, but can be negated later using [unary minus](#defining-an-expression). 
The number can be either an integer or a floating point number.
After number you can have an exponent.

The string is a set of characters inside single or double quotes.
There is no difference between single and double quotes.
Inside quotes there can be ANY character. 
If you want to put a quote as a part of the string content, use `\"` or `\'` escape sequence. 
Any escape sequence inside string such as \n or \t is recognized as a single escape character.
Example: `"Love,\nMike\t19.02.2007"` is recognized as:
```
Love,
Mike    19.02.2007
```
Multiple strings `s1 s2 ... sn` are recognized as a single string resulted by concatenating `s[1-n]` strings.
Example: "Hello" ", " 'world!' is recognized as "Hello, world!". Different quotes can be combined.

Identifier is a non-term both for table name and column name.
Identifier is a nonempty set of digits, letters, and underscores with leading letter.

```EBNF
value                  = uns_number | string{string} | table_column_reference ;
uns_number             = ( uns_integer | uns_float ) [exponent] ;
uns_integer            = "0" | positive_digit {digit} ;
uns_float              = digit {digit} "." {digit} | ["."] digit {digit} ;
exponent               = ( "E" | "e" ) [ "+" | "-" ] digit{digit} ;
table_column_reference = identifier "." identifier ;
identifier             = letter { [digit] [letter] "_" } ;
digit                  = "0" | positive_digit ;
positive_digit         = "1" | ... | "9" ;
letter                 = "a" | "A" | "b" | "B" | ... | "z" | "Z" ;
string                 = ( "'" { !"'" | "\'" } "'" ) | ( '"' { !'"' | '\"' } '"' ) ;
```

## Defining an Expression

Expression is a value or an operation performed to other expressions.
Allowed operations: addition, subtraction, multiplication, division, remainder of division, modulus, precedence brackets and unary minus.
Note that unary minus should have **higher priority** than the subtraction operation.

```EBNF
expression = value
           | expression "+" expression
           | expression "-" expression
           | expression "*" expression
           | expression "/" expression
           | expression "%" expression
           | "|" expression "|"
           | "(" expression ")"
           | "-" expression
           ;
```

### REQUIREMENTS
1. Despite an expression can be a string, these operations on strings is semantically invalid.
String concatenation with addition operation is not allowed.

## Defining a Condition

Condition is a boolean value resulting by:
1. The comparison of two other conditions:
   conjunction, disjunction, 
   equivalence, implication, addition modulo two, 
   precedence brackets, negation over the expression in parentheses;
2. The comparison of two expressions. 
   Can be applied even to strings using [lexicographical comparison](https://en.wikipedia.org/wiki/Lexicographic_order).
3. The constant (`true` or `false`);

```EBNF
condition = condition "AND" condition
          | condition "OR" condition
          | condition "<=>" condition
          | condition "->" condition
          | condition "^" condition
          | "(" condition ")"
          | "!(" condition ")"
          | expression ">=" expression
          | expression "<=" expression
          | expression "=" expression
          | expression "!=" expression
          | expression ">" expression
          | expression "<" expression
          | "true"
          | "false"
          ;
```

## Defining an Evaluateable

Evaluateable defines anything that can be resolved, evaluated.
It can be a condition with boolean result, an expression with numeric or string result.

```EBNF
evaluateable = condition | expression ;
```

# Defining Queries

A valid parsing input is defined as set of queries.
Note: empty input is also a valid input.

```EBNF
queries = { [query] ";" } ;
query   = select_query ;
```

## SELECT Query

`SELECT` query grabs data from database tables and represents it as a table. 

`SELECT` keyword always comes with a selector or a combination of selectors.
Each selector defines a column in the resulting table, so there should be at least 1 selector.
Any selector may be followed by `AS` keyword with a string. 
This string defines its column name. 
If no `AS` keyword specified, the column name is set with default value.

The `SELECT` keyword optionally may be followed by `FROM` keyword.
If no `FROM` keyword specified, the query will not refer to any table and **the
resulting table will have only one line and a header**.

`FROM` keyword comes with set of table names. 
Each table_name specifies the table that will be iterated through within Select Query.

Each row of table[`i`] will be iterated with every row in table[`i + 1`].
If From clause has 1 table_name of a table with N rows, the resulting table will have N rows too.
If From clause has 2 tables with sizes N and M respectively, the resulting table will have N * M rows.
If From clause has 3 tables with sizes N, M, and P respectively, the resulting table will have N * M * P rows.

```EBNF
select_query = "SELECT" selector{selector} [ "FROM" table_name{table_name} ] ;
selector = evaluateable [ "AS" string ] ;
table_name = identifier ;
```

### REQUIREMENTS
1. Any table used in Select Query should be specified here, but specified tables don't have to be used.
> Note that if we don't specify the table name we used in selector, the query will fail with Semantic Error.
> If we specify the table name but we don't have such table yet, this is Runtime Error.
> See [[#Errors]]

2. Every table_name should be **unique**.
