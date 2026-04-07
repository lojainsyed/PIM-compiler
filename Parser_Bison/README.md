
# Parser Implementation using Bison and Flex

## Overview



The parser checks whether an input program follows the defined grammar rules.

### Output Behavior
- Valid input → Parse successful.
- Invalid input → Parse failed.

---

##  Grammar



### Variables (Non-terminals)
V = {P, L, S, X, Y, R}

### Terminals
Σ = {int, id, =, ;, +, -, *, M, [, ], integer}

### Start Symbol
S = P

### Production Rules

P → L  

L → L S  
L → S  

S → int id = X ;  
S → id = X ;  
S → R = X ;  

X → Y  
X → Y + Y  
X → Y - Y  
X → Y * Y  

Y → id  
Y → integer  
Y → R  

R → M [ integer ] [ integer ]

---

## Supported Language

### Declarations
int id = expr;

### Assignments
id = expr;

### Matrix Assignments
M[integer][integer] = expr;

---

## Expressions

Expressions can be:
- Identifier
- Integer
- Matrix reference
- Arithmetic operations:
  a + b  
  a - b  
  a * b  

---

## Matrix Access

M[integer][integer]

---

## Lexical Rules

### Identifiers
- Must start with a letter
- Can include letters, digits, and underscore

Examples:
- a  
- temp_1  
- sum_0  

Invalid example:
- _bad  

---

### Integers

Valid:
- 0  
- 5  
- 10  
- 123  

Invalid:
- 007  

---

### Comments

// this is a comment

---

### Whitespace

Whitespace is ignored:

int   a   =   M[ 0 ][ 1 ] ;

---

## Project Structure

Parser_Bison/
- parser.bison  
- scanner.flex  
- token.h  
- main.c  
- Makefile  
- tests/
  - valid_all_ops.c  
  - valid_ids.c  
  - valid_comments.c  
  - valid_whitespace.c  
  - valid_matrix_read_write.c  
  - invalid_chars.c  
  - invalid_ids.c  

---

## Build Instructions

Navigate to the project directory:

cd ~/ece506/Parser/assignment

---

### Using Makefile

make

---

### Manual Build Commands

bison --defines=token.h --output=parser.c parser.bison -v  
flex -o scanner.c scanner.flex  
gcc main.c scanner.c parser.c -o compiler.exe  

---

## Running the Parser

Run a single test file:

./compiler.exe < tests/valid_all_ops.c

---

## Expected Output

Valid input:
Parse successful.

Invalid input:
Parse failed.

---

## Test Cases

### Passing Tests
- valid_all_ops.c  
- valid_ids.c  
- valid_comments.c  
- valid_whitespace.c  
- valid_matrix_read_write.c  

### Failing Tests
- invalid_chars.c  
- invalid_ids.c  

---

## Clean Build

make clean


