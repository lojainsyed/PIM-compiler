
# Scanner part 1 - Lexical Specification


## Input language: 
The input language of this compiler is a restricted C language considering simple scalar computations and matrix memory accesses for a Processing-in-Memory (PIM) architecture
The language supports:

Integer variable declarations

Reading values from a fixed 2-D matrix M

Simple arithmetic expressions with exactly one operator

Writing computed values back to the matrix

## Token Categories/Types:

1. Keywords
Reserved words with predefined meaning.

int

2. Identifiers

Names of scalar variables.
Identifiers begin with a letter or underscore and may contain letters, digits, or underscores.

Examples:

a

temp_1

sum0
3. Matrix Identifier

A reserved identifier representing the memory matrix.

M
4. Integer 

 Integers used for constants and matrix indices.

Examples:

0

12

34

5. Assignment Operator

Used to assign values.

=
6. Arithmetic Operators

Used in binary arithmetic expressions.

+

-

*
7. Delimiters / Punctuation

Symbols used for grouping and statement termination.

[

]

;
8. Comments

Used to annotate source code and ignored by the scanner.

Single-line comments beginning with //
9. Whitespace

Used to separate tokens and improve readability.
Whitespace is ignored by the scanner.

space

tab

newline
10. Invalid / Error Tokens

Any character sequence that does not match a valid token pattern.
This category is used for lexical error detection.
11. End-of-File (EOF)

A special token returned by the scanner to indicate that the input stream has been fully consumed.



## Formal Specification:
Keywords:(int)
Matrix Identifier:(M)
Identifiers :([A-Za-z_][A-Za-z0-9_]*)
Integer:(0|[1-9][0-9]*)
Assignment Operator:(=)
Arithmetic Operators:(\+|-|\*)
Delimiters :(\[|\]|;)
Comments:(\/\/[^\n]*)
Whitespace:([ \t\r\n]+)
Invalid Tokens:(.)
End-of-File:EOF

Complete Example Program

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








