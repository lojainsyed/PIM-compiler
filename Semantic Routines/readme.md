# Semantic Analysis – Compiler Project

## Build and Run

```bash
bison --defines=token.h --output=parser.c parser.bison -v
flex -o scanner.c scanner.flex
gcc main.c semantic.c execute.c scanner.c parser.c -o compiler.exe -w
```

## Test Commands

```bash
./compiler.exe tests/pass_large_expr.c
./compiler.exe tests/pass_4_types.c
./compiler.exe tests/pass_scope_function.c
./compiler.exe tests/pass_3_matrix.c
./compiler.exe tests/error_undeclared.c
./compiler.exe tests/error_redeclaration_same_scope.c
./compiler.exe tests/error_assignment_type_mismatch.c
```

## Language Overview

This compiler supports integer variables, float variables, arithmetic expressions, assignment statements, print statements, matrix reads and writes using `M[i][j]`, function declarations with parameters, and nested scopes.

## AST Design

The AST follows the Chapter 6 style. The compiler uses structured nodes for declarations, statements, expressions, types, functions, and blocks. Statements are connected using the `next` pointer, and block statements use the `body` pointer to refer to the first statement inside the block.

---

## Test 1: Large Arithmetic Expression

### Input

```c
int a = 10;
int b = 5;
int c = 5 * 8 - 6 + (11 - 9 * 7) + 3;
print c;
```

### AST TREE

```text
PROGRAM
  stmt_list:
    STMT_DECL
      decl:
        DECL name=a
          type:
            TYPE int
          value:
            EXPR_INT value=10
    next:
      STMT_DECL
        decl:
          DECL name=b
            type:
              TYPE int
            value:
              EXPR_INT value=5
      next:
        STMT_DECL
          decl:
            DECL name=c
              type:
                TYPE int
              value:
                EXPR_ADD
                  left:
                    EXPR_ADD
                      left:
                        EXPR_SUB
                          left:
                            EXPR_MUL
                              left:
                                EXPR_INT value=5
                              right:
                                EXPR_INT value=8
                          right:
                            EXPR_INT value=6
                      right:
                        EXPR_SUB
                          left:
                            EXPR_INT value=11
                          right:
                            EXPR_MUL
                              left:
                                EXPR_INT value=9
                              right:
                                EXPR_INT value=7
                  right:
                    EXPR_INT value=3
        next:
          STMT_PRINT
            expr:
              EXPR_NAME name=c
```

### Semantic Analysis

```text
Semantic analysis finished with 0 error(s).
```

### Symbol Table

```text
Nested Symbol Table:
====================

Scope Level 0:
-------------------------------------
Name            Kind       Type                 Which
a               GLOBAL     int                  0
b               GLOBAL     int                  1
c               GLOBAL     int                  2
```

---

## Test 2: Type Handling with int and float

### Input

```c
int a = 10;
float b = 2.5;
int c = 5 + 3;
float d = a + b;
print d;
```

### AST TREE

```text
PROGRAM
  stmt_list:
    STMT_DECL
      decl:
        DECL name=a
          type:
            TYPE int
          value:
            EXPR_INT value=10
    next:
      STMT_DECL
        decl:
          DECL name=b
            type:
              TYPE float
            value:
              EXPR_FLOAT value=2.500000
      next:
        STMT_DECL
          decl:
            DECL name=c
              type:
                TYPE int
              value:
                EXPR_ADD
                  left:
                    EXPR_INT value=5
                  right:
                    EXPR_INT value=3
        next:
          STMT_DECL
            decl:
              DECL name=d
                type:
                  TYPE float
                value:
                  EXPR_ADD
                    left:
                      EXPR_NAME name=a
                    right:
                      EXPR_NAME name=b
          next:
            STMT_PRINT
              expr:
                EXPR_NAME name=d
```

### Semantic Analysis

```text
Semantic analysis finished with 0 error(s).
```

### Symbol Table

```text
Nested Symbol Table:
====================

Scope Level 0:
-------------------------------------
Name            Kind       Type                 Which
a               GLOBAL     int                  0
b               GLOBAL     float                1
c               GLOBAL     int                  2
d               GLOBAL     float                3
```

### Execution

```text
OUTPUT: 12.500000
```

---

## Test 3: Function Scope and Shadowing

### Input

```c
int x = 10;

int f(int x) {
    int y = 5;
    int z = x + y;
    print x;
    print y;
    print z;
}

print x;
```

### AST READABLE FORM

```text
var [x] : int = expr 10;
var [f] : function returning int params(x: int) code {
var [y] : int = expr 5;
var [z] : int = expr (x + y);
print x;
print y;
print z;
}
print x;
```

### AST TREE

```text
PROGRAM
  stmt_list:
    STMT_DECL
      decl:
        DECL name=x
          type:
            TYPE int
          value:
            EXPR_INT value=10
    next:
      STMT_DECL
        decl:
          DECL name=f
            type:
              TYPE_FUNCTION
                return_type:
                  TYPE int
                params:
                  PARAM name=x
                    type:
                      TYPE int
            code:
              STMT_BLOCK
                body:
                  STMT_DECL
                    decl:
                      DECL name=y
                        type:
                          TYPE int
                        value:
                          EXPR_INT value=5
                  next:
                    STMT_DECL
                      decl:
                        DECL name=z
                          type:
                            TYPE int
                          value:
                            EXPR_ADD
                              left:
                                EXPR_NAME name=x
                              right:
                                EXPR_NAME name=y
                    next:
                      STMT_PRINT
                        expr:
                          EXPR_NAME name=x
                      next:
                        STMT_PRINT
                          expr:
                            EXPR_NAME name=y
                        next:
                          STMT_PRINT
                            expr:
                              EXPR_NAME name=z
      next:
        STMT_PRINT
          expr:
            EXPR_NAME name=x
```

### Semantic Analysis

```text
Semantic analysis finished with 0 error(s).
```

### Symbol Table

```text
Nested Symbol Table:
====================

Scope Level 2:
-------------------------------------
Name            Kind       Type                 Which
y               LOCAL      int                  0
z               LOCAL      int                  1

Scope Level 1:
-------------------------------------
Name            Kind       Type                 Which
x               PARAM      int                  0

Scope Level 0:
-------------------------------------
Name            Kind       Type                 Which
x               GLOBAL     int                  0
f               GLOBAL     function returning int params(x: int) 1
```

### Execution

```text
OUTPUT: 10
OUTPUT: 5
OUTPUT: 15
OUTPUT: 10
```

### Explanation

This test shows nested scopes. The global `x` is stored in scope level 0. The function parameter `x` is stored in scope level 1. The local variables `y` and `z` are stored in scope level 2. The compiler allows shadowing across different scopes.

---

## Test 4: Matrix Read, Matrix Write, and Expression

### Input

```c
M[0][0] = 10;
M[0][1] = 5;

int a = M[0][0];
int b = M[0][1];

int sum = a + b;

M[0][2] = sum;

print sum;
```

### AST READABLE FORM

```text
M[0][0] = 10;
M[0][1] = 5;
var [a] : int = expr M[0][0];
var [b] : int = expr M[0][1];
var [sum] : int = expr (a + b);
M[0][2] = sum;
print sum;
```

### AST TREE

```text
PROGRAM
  stmt_list:
    STMT_MATRIX_ASSIGN
      target:
        EXPR_MATRIX_REF
          row:
            EXPR_INT value=0
          col:
            EXPR_INT value=0
      value:
        EXPR_INT value=10
    next:
      STMT_MATRIX_ASSIGN
        target:
          EXPR_MATRIX_REF
            row:
              EXPR_INT value=0
            col:
              EXPR_INT value=1
        value:
          EXPR_INT value=5
      next:
        STMT_DECL
          decl:
            DECL name=a
              type:
                TYPE int
              value:
                EXPR_MATRIX_REF
                  row:
                    EXPR_INT value=0
                  col:
                    EXPR_INT value=0
        next:
          STMT_DECL
            decl:
              DECL name=b
                type:
                  TYPE int
                value:
                  EXPR_MATRIX_REF
                    row:
                      EXPR_INT value=0
                    col:
                      EXPR_INT value=1
          next:
            STMT_DECL
              decl:
                DECL name=sum
                  type:
                    TYPE int
                  value:
                    EXPR_ADD
                      left:
                        EXPR_NAME name=a
                      right:
                        EXPR_NAME name=b
            next:
              STMT_MATRIX_ASSIGN
                target:
                  EXPR_MATRIX_REF
                    row:
                      EXPR_INT value=0
                    col:
                      EXPR_INT value=2
                value:
                  EXPR_NAME name=sum
              next:
                STMT_PRINT
                  expr:
                    EXPR_NAME name=sum
```

### Semantic Analysis

```text
Semantic analysis finished with 0 error(s).
```

### Symbol Table

```text
Nested Symbol Table:
====================

Scope Level 0:
-------------------------------------
Name            Kind       Type                 Which
a               GLOBAL     int                  0
b               GLOBAL     int                  1
sum             GLOBAL     int                  2
```

### Execution

```text
OUTPUT: 15
```

---

## Test 5: Undefined Variable Error

### Input

```c
int a = 10;
int b = a + c;
print b;
```

### AST READABLE FORM

```text
var [a] : int = expr 10;
var [b] : int = expr (a + c);
print b;
```

### AST TREE

```text
PROGRAM
  stmt_list:
    STMT_DECL
      decl:
        DECL name=a
          type:
            TYPE int
          value:
            EXPR_INT value=10
    next:
      STMT_DECL
        decl:
          DECL name=b
            type:
              TYPE int
            value:
              EXPR_ADD
                left:
                  EXPR_NAME name=a
                right:
                  EXPR_NAME name=c
      next:
        STMT_PRINT
          expr:
            EXPR_NAME name=b
```

### Semantic Analysis

```text
semantic error: undeclared identifier 'c'
semantic error: arithmetic operands must be numeric
semantic error: type mismatch in declaration 'b'

Semantic analysis finished with 3 error(s).
```

### Symbol Table

```text
Nested Symbol Table:
====================

Scope Level 0:
-------------------------------------
Name            Kind       Type                 Which
a               GLOBAL     int                  0
b               GLOBAL     int                  1
```

### Execution

```text
Runtime error: variable c not found
OUTPUT: 10
```

---

## Test 6: Redeclaration in Same Scope

### Input

```c
int a = 10;
int a = 20;
print a;
```

### AST READABLE FORM

```text
var [a] : int = expr 10;
var [a] : int = expr 20;
print a;
```

### AST TREE

```text
PROGRAM
  stmt_list:
    STMT_DECL
      decl:
        DECL name=a
          type:
            TYPE int
          value:
            EXPR_INT value=10
    next:
      STMT_DECL
        decl:
          DECL name=a
            type:
              TYPE int
            value:
              EXPR_INT value=20
      next:
        STMT_PRINT
          expr:
            EXPR_NAME name=a
```

### Semantic Analysis

```text
semantic error: redeclaration of identifier 'a'

Semantic analysis finished with 1 error(s).
```

### Symbol Table

```text
Nested Symbol Table:
====================

Scope Level 0:
-------------------------------------
Name            Kind       Type                 Which
a               GLOBAL     int                  0
```

---

## Test 7: Assignment Type Mismatch

### Input

```c
int a = 10;
float b = 2.5;

a = b;
print a;
```

### AST READABLE FORM

```text
var [a] : int = expr 10;
var [b] : float = expr 2.500000;
a = b;
print a;
```

### AST TREE

```text
PROGRAM
  stmt_list:
    STMT_DECL
      decl:
        DECL name=a
          type:
            TYPE int
          value:
            EXPR_INT value=10
    next:
      STMT_DECL
        decl:
          DECL name=b
            type:
              TYPE float
            value:
              EXPR_FLOAT value=2.500000
      next:
        STMT_ASSIGN name=a
          value:
            EXPR_NAME name=b
        next:
          STMT_PRINT
            expr:
              EXPR_NAME name=a
```

### Semantic Analysis

```text
semantic error: type mismatch in assignment 'a'

Semantic analysis finished with 1 error(s).
```

### Symbol Table

```text
Nested Symbol Table:
====================

Scope Level 0:
-------------------------------------
Name            Kind       Type                 Which
a               GLOBAL     int                  0
b               GLOBAL     float                1
```

### Execution

```text
OUTPUT: 2.500000
```


