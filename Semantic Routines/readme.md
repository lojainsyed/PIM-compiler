# Semantic Analysis – Compiler Project

## Student Information

| Field | Value |
|------|------|
| Name | Lojain Syed |
| Course | Compiler Design |
| Project | Semantic Analysis |

## Overview

This project implements the semantic analysis phase for a restricted C-like language. The compiler performs AST construction, AST printing (Readable and Post-order), symbol table generation, name resolution, and semantic error detection.

## Compiler Pipeline

Source Code -> Scanner -> Parser -> AST -> Semantic Analysis -> Output

## Build and Run

### Build

bison --defines=token.h --output=parser.c parser.bison -v
flex -o scanner.c scanner.flex
gcc main.c semantic.c scanner.c parser.c -o compiler.exe -w

### Run

./compiler.exe tests/valid_all_ops.c
./compiler.exe tests/error_redeclaration.c
./compiler.exe tests/error_undeclared.c

## Executing the Tests

## Test 1: Valid Program

### Input Source

M[0][0] = 10;
M[0][1] = 5;

int a = M[0][0];
int b = M[0][1];

int add = a + b;
int sub = a - b;
int mul = a * b;

M[0][2] = add;
M[0][3] = sub;
M[0][4] = mul;

### Printed AST (Readable)

M[0][0] = 10;
M[0][1] = 5;
var [a] : int = expr M[0][0];
var [b] : int = expr M[0][1];
var [add] : int = expr (a + b);
var [sub] : int = expr (a - b);
var [mul] : int = expr (a * b);
M[0][2] = add;
M[0][3] = sub;
M[0][4] = mul;

### Printed AST (Post-Order)

M[0][0] 10 MATRIX_ASSIGN
M[0][1] 5 MATRIX_ASSIGN
M[0][0] DECL(a) STMT_DECL
M[0][1] DECL(b) STMT_DECL
a b + DECL(add) STMT_DECL
a b - DECL(sub) STMT_DECL
a b * DECL(mul) STMT_DECL
M[0][2] add MATRIX_ASSIGN
M[0][3] sub MATRIX_ASSIGN
M[0][4] mul MATRIX_ASSIGN

### Printed Symbol Table

Name            Kind       Type            Which
mul             GLOBAL     int             4
sub             GLOBAL     int             3
add             GLOBAL     int             2
b               GLOBAL     int             1
a               GLOBAL     int             0

Semantic analysis finished with 0 error(s).

## Test 2: Redeclaration Error

### Input Source

int a = 5;
int b = 10;
int a = b + 1;

M[0][0] = a;

### Printed AST (Readable)

var [a] : int = expr 5;
var [b] : int = expr 10;
var [a] : int = expr (b + 1);
M[0][0] = a;

### Printed AST (Post-Order)

5 DECL(a) STMT_DECL
10 DECL(b) STMT_DECL
b 1 + DECL(a) STMT_DECL
M[0][0] a MATRIX_ASSIGN

semantic error: redeclaration of identifier 'a'
Semantic analysis finished with 1 error(s).

### Printed Symbol Table

Name            Kind       Type            Which
b               GLOBAL     int             1
a               GLOBAL     int             0

## Test 3: Undeclared Variable

### Input Source

M[0][0] = 10;

int a = M[0][0];
int b = a + c;

M[0][1] = b;

### Printed AST (Readable)

M[0][0] = 10;
var [a] : int = expr M[0][0];
var [b] : int = expr (a + c);
M[0][1] = b;

### Printed AST (Post-Order)

M[0][0] 10 MATRIX_ASSIGN
M[0][0] DECL(a) STMT_DECL
a c + DECL(b) STMT_DECL
M[0][1] b MATRIX_ASSIGN

semantic error: undeclared identifier 'c'
semantic error: undeclared identifier 'c'
semantic error: arithmetic operands must be integers
semantic error: type mismatch in declaration 'b'
Semantic analysis finished with 4 error(s).

### Printed Symbol Table

Name            Kind       Type            Which
b               GLOBAL     int             1
a               GLOBAL     int             0
