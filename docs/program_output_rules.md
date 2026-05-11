# General
Program reads data from `std::cin` and prints output to `std::cout` and `std::cerr`.
All errors should be printed to `std::cerr`. Regular output should be printed to `std::cout`.
Program handles all input incoming from `std::cin` unless it faces `End Of File`. 

## Output Types
### Single String Type
Single String Type output made by actions that return single string as a result (errors, insertion report etc.).
That string is printed as it is with blank line below result.
Example:
```
[SYNTAX_ERROR] at [5.10] syntax error, unexpected identifier

```

### Table Type
Table output is made by `SELECT` action. 
Table is a matrix of strings, where first line is called a **header**, and all other lines form the **body**.
Whole table is wrapped with special ASCII characters that form a table.
Header is separated from the body.
For each column width is calculated as the widest cell in current column.
In each column all cells should fit considering two additional corner spaces at left and right.
Widths are individual for each column.
Example:
```
+--------+-----+
| String | Int |
+--------+-----+
| Alice  | 24  |
| Bob    | 31  |
| Eva    | 29  |
| Farid  | 41  |
| Greta  | 22  |
+--------+-----+
```

It may happen that some cells will show strings, that have multiple lines (`\n` characters inside).
In that case strings are being sliced by `\n` character and each line is printed on a new line within current cell.
All other line is aligned by height with the tallest cell in row. 
When program prints non-last sub line of some cell, it should print `+` sign instead of right corner space.
If some cell is lower than the tallest one, extra lines are filled with empty lines.
Column width is calculated by the widest sub line of cells.
Example:
```
+-----+---+-----+
| A  +| C+| Int |
| B   | D+|     |
|     | E |     |
+-----+---+-----+
| 123+| 1+| 5   |
| 456 | 2+|     |
|     | 3+|     |
|     | 4 |     |
| qwe | w | 6   |
+-----+---+-----+
```
> Here in second column the width is equal to 3: 1 for characters, 1 for corner space at left and 1 for corner space at right or a plus sign.

Each Tab `\t` character should be recognized as this string: ` -> `. 
Consider it when calculating column width.
Example:
```
{ { "Alice\t", 24 }, { "Greta", 22 } }

+-----------+-----+
| String    | Int |
+-----------+-----+
| Alice ->  | 24  |
| Greta     | 22  |
+-----------+-----+
```

## Output Rules
Output rules differ between CLI and File input/output. 
If user writes input manually in terminal and receives output in the same terminal, [CLI rules](#cli-rules) are applied. 
If user writes input from file or saves output to file, [File rules](#file-rules) are applied. 

### File Rules
File rules are common rules without any visual sugar.

### CLI Rules
If user uses the application manually, program generates slightly different output to make it easier to work with and read.

##### Prompt
After each line, Program prints prompt to tell user that it is waiting for some input.
Prompt looks like this: `#>`.
Example:
```
#> SELECT 1 + 1;
+-----+
| Int |
+-----+
| 2   |
+-----+
#>
```
> Here program prints #> prompt and starts waiting for user input. 
> When user prints he's request and press Enter, Program responses with the output and prints another prompt when it is ready to read again.

##### Zebra-Style Table
Sometimes when table has multiple lines with multiline cells it is complicated to distinguish rows.
In such cases every odd row from the table's body is highlighted with accent bash color and other rows are 
highlighted with blend bash color.
```
Accent color (AC): "\033[7m"
Blend color (BC): "\033[0m"
Reset code (RC): "\033[0m"
```

Every content line starts with vertical tab `|`, then goes the Accent color code 
or Blend color code, then whole string, then Reset code and closing vertical bar `|`.

Note that you can identify each row's end using plus sign at the end. 
That is why Zebra style is not applied in file mode.

Important: if every cell in table has 1 line height, this style shouldn't be applied.

Example:
```
+----------------+---------------------+
|<BC> String     | String          <RC>|
+----------------+---------------------+
|<BC> this is   +| this is another+<RC>|
|<BC> long multi+| long multi     +<RC>|
|<BC> line line  | lineline        <RC>|
|<AC> this is   +| this is another+<RC>|
|<AC> long multi+| long multi     +<RC>|
|<AC> line line  | lineline        <RC>|
|<BC> this is   +| this is another+<RC>|
|<BC> long multi+| long multi     +<RC>|
|<BC> line line  | lineline        <RC>|
+----------------+---------------------+
```
> ***Warning***: For visual convenience table width is expanded to fit <BC>, <AC> and <RC> codes.
> In actual program those codes are invisible and retrieved by terminal to change the color.

##### Error highlighting
Every error has a tag at the beginning with error type.

Example with **Syntax error**:
```
[SYNTAX_ERROR] at [5.10] syntax error, unexpected identifier

```

In CLI, this tag should be highlighted:
```
Error highlight color(EC): "\033[38;5;9m"
```

Highlighted example:
```
<EC>[SYNTAX_ERROR]<RC> at [5.10] syntax error, unexpected identifier

```
