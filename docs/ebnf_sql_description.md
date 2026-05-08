# EBNF SQL description
## Simple Expressions
### Defining a Value

The `value`'s can be either a positive number, string or a column reference. 

The number is always positive, but can be negated later using unary minus (`#TODO` make a reference). 
The number can be either an integer or a floating point number.
After number you can see an exponent.

The string is a set of characters inside single or double quotes.
There is no difference between single and double quotes.
Inside quotes there can be ANY character. 
If you want to put a quote as a part of the string content, use \" or \' escape sequence. 
Any escape sequence inside string such as \n or \t is recognized as a single escape character.
Example: `"Love,\nMike\t19.02.2007"` is recognized as:
```
Love,
Mike    19.02.2007
```
Multiple strings `s1 s2 ... sn` are recognized as a single string resulted by concatenating `s[1-n]` strings.
Example: "Hello" ", " 'world!' is recognized as "Hello, world!". Different quotes can be combined.

Identifier is a term both for table name and column name.
Identifier is a non-empty set of digits, letters and underscores with leading letter.

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
