
# Parser Implementation using Bison and Flex

## Overview

This project implements a parser for a simple programming language using **Bison** (parser generator) and **Flex** (lexer).

The parser checks whether an input program follows the defined grammar rules.

### Output Behavior
- Valid input → Parse successful.
- Invalid input → Parse failed.

---

## Context-Free Grammar

This grammar defines a simple language with declarations, assignments, matrix references, and arithmetic expressions.

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

assignment/
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

## Running All Tests

for f in tests/*.c; do  
  echo "Running $f"  
  ./compiler.exe < $f  
  echo ""  
done  

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

---

## Notes

- Matrix access must be exactly M[integer][integer]  
- Identifiers must begin with a letter  
- Leading zeros in integers are not allowed  
- Supported operators: +, -, *  
- Comments are ignored  
- Whitespace is ignored  

---

## Conclusion

This project demonstrates the use of Bison and Flex to implement a parser that validates programs based on a defined context-free grammar.
