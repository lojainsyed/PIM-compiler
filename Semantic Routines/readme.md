
# Semantic Analysis



## Implemented Features
### AST
- Supports expressions, declarations, and statements
- Readable AST printing


### Symbol Table
Stores:
- Name
- Kind (GLOBAL / LOCAL)
- Type
- Which (index)

### Semantic Analysis
- Undeclared identifier detection
- Redeclaration detection
- Basic type checking
- Arithmetic validation

## Build and Run
### Build
bison --defines=token.h --output=parser.c parser.bison -v
flex -o scanner.c scanner.flex
gcc main.c semantic.c scanner.c parser.c -o compiler.exe -w

### Run
./compiler.exe tests/valid_all_ops.c
./compiler.exe tests/error_redeclaration.c
./compiler.exe tests/error_undeclared.c

# Executing the Tests

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


### Printed Symbol Table
Name            Kind       Type            Which
mul             GLOBAL     int             4
sub             GLOBAL     int             3
add             GLOBAL     int             2
b               GLOBAL     int             1
a               GLOBAL     int             0

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



### Semantic Errors
semantic error: redeclaration of identifier 'a'

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



### Semantic Errors
semantic error: undeclared identifier 'c'
semantic error: undeclared identifier 'c'
semantic error: arithmetic operands must be integers
semantic error: type mismatch in declaration 'b'

### Printed Symbol Table
Name            Kind       Type            Which
b               GLOBAL     int             1
a               GLOBAL     int             0

## Notes
- Multiple errors may be reported for a single issue due to both name resolution and type checking phases.
- The symbol table uses a stack-based scope system.
- The `which` field represents the position of the variable in the scope.

## Conclusion
This project successfully implements AST construction, symbol table generation, semantic analysis, and error detection for a simple programming language. The compiler correctly handles valid programs and identifies semantic errors.
