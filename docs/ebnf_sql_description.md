# EBNF SQL Description
## Syntax Rules
### Defining a Value

The `value` can be either a positive number, string, or a column reference. 

The number is always positive, but can be negated later using unary minus (#TODO make a reference). 
The number can be either an integer or a floating point number.
After number you can see an exponent.

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
string                 = ( "'" { !"'" | "\'" } "'" ) | ( '"' { !'"' | '\"' } '"' ) ;
letter                 = "a" | "A" | "b" | "B" | ... | "z" | "Z" ;
positive_digit         = "1" | ... | "9" ;
digit                  = "0" | positive_digit ;
identifier             = letter { [digit] [letter] "_" } ;
table_column_reference = identifier "." identifier ;
exponent               = ( "E" | "e" ) [ "+" | "-" ] digit{digit} ;
uns_integer            = "0" | positive_digit {digit} ;
uns_float              = digit {digit} "." {digit} | ["."] digit {digit} ;
uns_number             = ( uns_integer | uns_float ) [exponent] ;
value                  = uns_number | string{string} | table_column_reference ;
```

### Defining an Expression

Expression is a value or an operation performed to other expressions.
Allowed operations: addition, subtraction, multiplication, division, remainder of division, modulus, precedence brackets and unary minus.
Despite an expression can be a string, these operations on strings is semantically invalid.
Grammar rules allow applying these operations to strings, so the compiler needs to do a semantic analysis (#TODO reference to semantic analysis block).
Note that unary minus should have **higher priority** than the subtraction operation.
String concatenation with addition operation is not allowed.

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

### Defining a Condition

Condition is a boolean value resulting by:
1. The comparison of two other conditions:
   conjunction, disjunction, 
   equivalence, implication, addition modulo two, 
   precedence brackets, negation over the expression in parentheses;
2. The comparison of two expressions. 
   Can be applied even to strings using lexicographical comparison (#TODO make reference to wiki).
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
