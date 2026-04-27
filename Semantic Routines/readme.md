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

The compiler supports a restricted C-like language with integer and float variables, functions with parameters, arithmetic expressions (+, -, *), matrix access using M[i][j], assignment statements, print statements, and nested scopes.

The Abstract Syntax Tree (AST) follows Chapter 6 design. It is a tree-based representation without unnecessary syntax symbols. Statements are linked using a structure where each statement points to the next using a next pointer, and blocks point to their first statement using a body pointer.

Test Case 1:
int a = 10;  
int b = 5;  
int c = 5 * 8 - 6 + (11 - 9 * 7) + 3;  
print c;  
Result: Semantic analysis finished with 0 error(s).

Test Case 2:
int a = 10;  
float b = 2.5;  
float d = a + b;  
print d;  
Result: OUTPUT: 12.500000  
Symbol Table: a → int, b → float, d → float

Test Case 3:
int x = 10;  
int f(int x) {  
    int y = 5;  
    int z = x + y;  
    print x;  
    print y;  
    print z;  
}  
print x;  
Result: OUTPUT: 10, OUTPUT: 5, OUTPUT: 15, OUTPUT: 10  
Symbol Table shows nested scopes with local, parameter, and global variables. Shadowing is allowed where inner scope variables override outer scope variables.

Test Case 4:
M[0][0] = 10;  
M[0][1] = 5;  
int a = M[0][0];  
int b = M[0][1];  
int sum = a + b;  
M[0][2] = sum;  
print sum;  
Result: OUTPUT: 15  
Symbol Table: a → int, b → int, sum → int

Test Case 5:
int a = 10;  
int b = a + c;  
print b;  
Result: semantic error: undeclared identifier 'c', arithmetic operands must be numeric, type mismatch in declaration 'b'

Test Case 6:
int a = 10;  
int a = 20;  
Result: semantic error: redeclaration of identifier 'a'

Test Case 7:
int a = 10;  
float b = 2.5;  
a = b;  
Result: semantic error: type mismatch in assignment 'a'  
Execution may still run for debugging, but the program is semantically invalid.

The compiler supports nested scopes using a symbol table stack, allows variable shadowing across different scopes, performs type checking for int and float, supports matrix operations with M[i][j], and detects semantic errors such as undeclared variables, redeclarations in the same scope, and type mismatches.

The compiler successfully implements AST construction using a linked structure, symbol table management, function and scope handling, type checking, matrix operations, and semantic error detection. It follows standard compiler design principles for AST representation, scope resolution, and semantic validation.
