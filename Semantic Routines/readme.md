# Semantic Analysis – Compiler Project

Build and Run:
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

This compiler supports a restricted C-like language including integer and float variables, functions with parameters, arithmetic expressions (+, -, *), matrix access using M[i][j], assignment statements, print statements, and nested scopes.

The Abstract Syntax Tree (AST) follows Chapter 6 design. It represents program structure without unnecessary syntax and uses a linked representation where statements are connected using a "next" pointer and blocks reference their first statement using a "body" pointer.

Test Case 1: Large Expression
Input:
int a = 10;
int b = 5;
int c = 5 * 8 - 6 + (11 - 9 * 7) + 3;
print c;

AST Tree:
EXPR_ADD
  left:
    EXPR_ADD
      left:
        EXPR_SUB
          left:
            EXPR_MUL
              5
              8
          right:
            6
      right:
        EXPR_SUB
          11
          EXPR_MUL
            9
            7
  right:
    3

Result:
Semantic analysis finished with 0 error(s).

Test Case 2: Type Handling (int + float)
Input:
int a = 10;
float b = 2.5;
float d = a + b;
print d;

AST Tree:
STMT_DECL (d)
  EXPR_ADD
    a
    b

Output:
OUTPUT: 12.500000

Test Case 3: Function Scope (Shadowing)
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
  STMT_DECL (x)
  next:
    STMT_DECL (function f)
      TYPE_FUNCTION
      STMT_BLOCK
        body:
          STMT_DECL (y)
          next:
            STMT_DECL (z)
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

Note:
Shadowing is allowed where inner scope variables override outer scope variables.

Test Case 4: Matrix Operations
Input:
M[0][0] = 10;
M[0][1] = 5;

int a = M[0][0];
int b = M[0][1];

int sum = a + b;

M[0][2] = sum;

print sum;

AST Tree:
STMT_MATRIX_ASSIGN
next:
  STMT_MATRIX_ASSIGN
  next:
    STMT_DECL (a)
    next:
      STMT_DECL (b)
      next:
        STMT_DECL (sum)
          EXPR_ADD (a + b)
        next:
          STMT_MATRIX_ASSIGN
          next:
            STMT_PRINT

Output:
OUTPUT: 15

Test Case 5: Undefined Variable
Input:
int a = 10;
int b = a + c;
print b;

AST Tree:
EXPR_ADD
  a
  c

Errors:
semantic error: undeclared identifier 'c'
semantic error: arithmetic operands must be numeric
semantic error: type mismatch in declaration 'b'

Test Case 6: Redeclaration (Same Scope)
Input:
int a = 10;
int a = 20;

Error:
semantic error: redeclaration of identifier 'a'

Test Case 7: Assignment Type Mismatch
Input:
int a = 10;
float b = 2.5;
a = b;

AST Tree:
STMT_ASSIGN
  a = b

Error:
semantic error: type mismatch in assignment 'a'

Note:
Execution may still run for debugging purposes, but the program is semantically invalid.

The compiler supports nested scopes using a symbol table stack, allows variable shadowing across different scopes, performs type checking for int and float, supports matrix operations using M[i][j], and detects semantic errors including undeclared variables, redeclarations in the same scope, and type mismatches.

The compiler successfully implements AST construction using linked structures, symbol table management, function handling, matrix operations, and semantic validation following standard compiler design principles.
