
# Scanner part 1 - Lexical Specification


## Input language: 
The input language of this compiler is a restricted C language considering simple scalar computations and matrix memory accesses for a Processing-in-Memory (PIM) architecture
The language supports:

- Integer  variable declarations
- Reading values from a fixed two-dimensional matrix `M`
- Simple arithmetic expressions containing exactly one operator
- Writing computed values back to the matrix
- Semicolon-terminated statements
- Single-line comments

## Token Categories/Types:

1. Keywords

| Token Name | Lexeme |
|-----------|--------|
| `Keyword`  | `int`  |
| `Keyword`  | `M`  |




2. Identifiers

Names of scalar variables.
Identifiers begin with a letter or underscore and may contain letters, digits, or underscores.
| Token Name | Pattern |
|-----------|---------|
| `ID` | `[A-Za-z_][A-Za-z0-9_]*` |

Examples:

a

temp_1

sum0

3. Matrix Identifier

A reserved identifier representing the memory matrix.

M
| Token Name | Lexeme |
|-----------|--------|
| `MATRIX_ID` | `M` |


4. Integer 

 Integers used for constants and matrix indices.

| Token Name | Pattern |
|-----------|---------|
| `INT_LITERAL` | `0 \| [1-9][0-9]*` |
Examples:

0

12

34


5. Assignment Operator


Used to assign values.

| Token Name | Lexeme |
|-----------|--------|
| `ASSIGN` | `=` |


6. Arithmetic Operators

Used in binary arithmetic expressions.
| Token Name | Lexeme |
|-----------|--------|
| `PLUS` | `+` |
| `MINUS` | `-` |
| `MUL` | `*` |


7. Delimiters / Punctuation

Symbols used for grouping and statement termination.

| Token Name | Lexeme |
|-----------|--------|
| `LBRACKET` | `[` |
| `RBRACKET` | `]` |
| `SEMICOLON` | `;` |


8. Comments

Used to annotate source code and ignored by the scanner.

Single-line comments beginning with //

| Token Name | Pattern |
|-----------|---------|
| `COMMENT` | `//[^\n]*` |


9. Whitespace

Used to separate tokens and improve readability.
Whitespace is ignored by the scanner.

| Token Name | Pattern |
|-----------|---------|
| `WHITESPACE` | `[ \t\r\n]+` |


space

tab

newline

10. Invalid / Error Tokens

Any character sequence that does not match a valid token pattern.
This category is used for lexical error detection.
| Token Name | Pattern |
|-----------|---------|
| `ERROR` | `.` |


11. End-of-File (EOF)

A special token returned by the scanner to indicate that the input stream has been fully consumed.

| Token Name |
|-----------|
| `EOF` |

---

## . Complete Example Program

```c
int a = M[0][1];
int b = M[0][2];

// Perform computations
int sum_0 = a + b;
int diff = sum_0 - b;
int prod = a * b;

// Store result back to matrix
M[0][3] = prod;


Keywords:
int x = M[1][2];
Identifiers:

Identifiers

int temp_1 = M[10][0];


Integer 

M[12][34] = a;


Arithmetic Operators

int c = a + b;
int d = c - b;
int e = a * b;


Delimiters

M[2][3] = e;


Whitespace

int   a   =   M[ 0 ][ 1 ] ;


Comments

int a = M[0][1]; // load value


End-of-File

<EOF>



















