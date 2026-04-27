# Semantic Analysis – Compiler Project

## Build and Run

bison --defines=token.h --output=parser.c parser.bison -v  
flex -o scanner.c scanner.flex  
gcc main.c semantic.c execute.c scanner.c parser.c -o compiler.exe -w  

Run:
./compiler.exe tests/pass_large_expr.c  
./compiler.exe tests/pass_types.c  
./compiler.exe tests/pass_scope_shadowing.c  
./compiler.exe tests/pass_3_matrix.c  
./compiler.exe tests/error_undeclared.c  
./compiler.exe tests/error_redeclaration_same_scope.c  
./compiler.exe tests/error_assignment_type_mismatch.c  

---

## Language Overview

This compiler supports:
- int and float variables  
- functions with parameters  
- arithmetic expressions (+, -, *)  
- matrix access M[i][j]  
- assignment statements  
- print statements  
- nested scopes  

---

## AST Design

The AST follows Chapter 6 design:
- Tree-based representation  
- Statements connected using next pointer  
- Blocks use body pointer  

---

## Test Results

### Test 1: Large Expression

Input:
int a = 10;  
int b = 5;  
int c = 5 * 8 - 6 + (11 - 9 * 7) + 3;  
print c;  

AST Tree:
PROGRAM  
  stmt_list:  
    STMT_DECL  
    next:  
      STMT_DECL  
      next:  
        STMT_DECL  
          EXPR_ADD  
            left:  
              EXPR_ADD  
                left:  
                  EXPR_SUB  
                    left:  
                      EXPR_MUL  
                        left: 5  
                        right: 8  
                    right: 6  
                right:  
                  EXPR_SUB  
                    left: 11  
                    right:  
                      EXPR_MUL  
                        left: 9  
                        right: 7  
            right: 3  

Result:
Semantic analysis finished with 0 error(s).  

---

### Test 2: Type Handling

Input:
int a = 10;  
float b = 2.5;  
float d = a + b;  
print d;  

AST Tree:
PROGRAM  
  stmt_list:  
    STMT_DECL  
    next:  
      STMT_DECL  
      next:  
        STMT_DECL  
          EXPR_ADD  
            left: EXPR_NAME a  
            right: EXPR_NAME b  
        next:  
          STMT_PRINT  

Output:
OUTPUT: 12.500000  

---

### Test 3: Function Scope (Shadowing)

Input:
int x = 10;  

int f(int x) {  
    int y = 5;  
    int z = x + y;  
    print x;  
    print y;  
    print z;  
}  

print x;  

AST Tree:
PROGRAM  
  stmt_list:  
    STMT_DECL  
    next:  
      STMT_DECL  
        TYPE_FUNCTION  
        STMT_BLOCK  
          body:  
            STMT_DECL  
            next:  
              STMT_DECL  
              next:  
                STMT_PRINT  
                next:  
                  STMT_PRINT  
                  next:  
                    STMT_PRINT  
      next:  
        STMT_PRINT  

Output:
OUTPUT: 10  
OUTPUT: 5  
OUTPUT: 15  
OUTPUT: 10  

Note: Shadowing is allowed  

---

### Test 4: Matrix Operations

Input:
M[0][0] = 10;  
M[0][1] = 5;  

int a = M[0][0];  
int b = M[0][1];  

int sum = a + b;  

M[0][2] = sum;  

print sum;  

AST Tree:
PROGRAM  
  stmt_list:  
    STMT_MATRIX_ASSIGN  
    next:  
      STMT_MATRIX_ASSIGN  
      next:  
        STMT_DECL  
        next:  
          STMT_DECL  
          next:  
            STMT_DECL  
              EXPR_ADD  
                left: EXPR_NAME a  
                right: EXPR_NAME b  
            next:  
              STMT_MATRIX_ASSIGN  
              next:  
                STMT_PRINT  

Output:
OUTPUT: 15  

---

### Test 5: Undefined Variable

Input:
int a = 10;  
int b = a + c;  
print b;  

AST Tree:
EXPR_ADD  
  left: a  
  right: c  

Errors:
semantic error: undeclared identifier 'c'  
semantic error: arithmetic operands must be numeric  
semantic error: type mismatch in declaration 'b'  

---

### Test 6: Redeclaration (Same Scope)

Input:
int a = 10;  
int a = 20;  

AST Tree:
PROGRAM  
  stmt_list:  
    STMT_DECL  
    next:  
      STMT_DECL  

Error:
semantic error: redeclaration of identifier 'a'  

---

### Test 7: Assignment Type Mismatch

Input:
int a = 10;  
float b = 2.5;  
a = b;  

AST Tree:
STMT_ASSIGN  
  name: a  
  value: EXPR_NAME b  

Error:
semantic error: type mismatch in assignment 'a'  

Note: Execution may still run for debugging  

---

## Design Decisions

- Nested scopes supported  
- Symbol table stack used  
- Shadowing allowed  
- Multiple errors reported  
- Type checking for int and float  
- Matrix operations supported  

---

## Summary

- AST construction using next pointer  
- Symbol table with nested scopes  
- Function handling  
- Type checking  
- Matrix operations  
- Semantic error detection  

---

## Final Statement

The compiler follows standard compiler design principles and correctly handles AST construction, scope resolution, type checking, and semantic validation.
