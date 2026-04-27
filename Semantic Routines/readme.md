# Semantic Analysis – Compiler Project

## Build and Run

bison --defines=token.h --output=parser.c parser.bison -v  
flex -o scanner.c scanner.flex  
gcc main.c semantic.c execute.c scanner.c parser.c -o compiler.exe -w  

./compiler.exe tests/pass_large_expr.c  
./compiler.exe tests/pass_types.c  
./compiler.exe tests/pass_scope_shadowing.c  
./compiler.exe tests/pass_3_matrix.c  
./compiler.exe tests/error_undeclared.c  
./compiler.exe tests/error_redeclaration_same_scope.c  
./compiler.exe tests/error_assignment_type_mismatch.c  

---

## Language Overview

This compiler supports a restricted C-like language with:
- Integer and float variables  
- Functions with parameters  
- Arithmetic expressions (+, -, *)  
- Matrix access using M[i][j]  
- Assignment statements  
- Print statements  
- Nested scopes  

---

## AST Design

The AST follows Chapter 6 design:
- Tree-based representation (no unnecessary syntax)
- Statements linked using:
