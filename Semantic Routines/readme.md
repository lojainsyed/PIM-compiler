# Semantic Analysis – Compiler Project

Build and Run:
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

Language Overview:
This compiler supports int and float variables, functions with parameters, arithmetic expressions (+, -, *), matrix access using M[i][j], assignment statements, print statements, and nested scopes.

AST Design:
The AST follows Chapter 6 design. It uses a tree structure where statements are connected using a "next" pointer and blocks use a "body" pointer. This represents the program structure without unnecessary syntax symbols.

Test Case 1: Large Expression
Input:
int a = 10;
int b = 5;
int c = 5 * 8 - 6 + (11 - 9 * 7) + 3;
print c;
Result:
Semantic analysis finished with 0 error(s).

Test Case 2: Type Handling
Input:
int a = 10;
float b = 2.5;
float d = a + b;
print d;
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
Output:
OUTPUT: 10
OUTPUT: 5
OUTPUT: 15
OUTPUT: 10
Explanation:
Inner variable x shadows global x, which is allowed.

Test Case 4: Matrix Operations
Input:
M[0][0] = 10;
M[0][1] = 5;
int a = M[0][0];
int b = M[0][1];
int sum = a + b;
M[0][2] = sum;
print sum;
Output:
OUTPUT: 15

Test Case 5: Undefined Variable
Input:
int a = 10;
int b = a + c;
print b;
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
Error:
semantic error: type mismatch in assignment 'a'
Note:
Execution may still run for debugging purposes, but the program is semantically invalid.

Design Decisions:
Nested scopes are supported using a symbol table stack. Variable shadowing is allowed across different scopes. The compiler performs type checking for int and float and supports matrix operations using M[i][j]. Multiple semantic errors are reported in one run.

Summary:
The compiler successfully implements AST construction using a linked structure with next pointers, symbol table management, function handling, type checking, matrix operations, and semantic error detection.

Final Statement:
The compiler follows standard compiler design principles and correctly handles AST construction, scope resolution, type checking, and semantic validation.
