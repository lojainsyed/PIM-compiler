# Semantic Analysis – Compiler Project

## Build and Run

    bison --defines=token.h --output=parser.c parser.bison -v
    flex -o scanner.c scanner.flex
    gcc main.c semantic.c execute.c scanner.c parser.c -o compiler.exe -w

    ./compiler.exe tests/pass_1_basic.c
    ./compiler.exe tests/error_2_redeclaration.c
    ./compiler.exe tests/error_1_undeclared.c
    ./compiler.exe tests/pass_function_scope.c
    ./compiler.exe tests/pass_4_types.c

---

## Grammar Rules

### Variables (Non-terminals)

    V = {Program, StmtList, Stmt, Decl, Block, ParamListOpt, ParamList, Param, Expr, Term, Factor, Type}

### Terminals

    Σ = {int, float, id, =, ;, +, -, *, /, (, ), {, }, ,, print, M, [, ], number, float_number}

### Start Symbol

    S = Program

### Production Rules

    Program → StmtList

    StmtList → StmtList Stmt
    StmtList → Stmt

    Stmt → Decl
    Stmt → id = Expr ;
    Stmt → M [ Expr ] [ Expr ] = Expr ;
    Stmt → print Expr ;
    Stmt → Block

    Decl → Type id = Expr ;
    Decl → Type id ( ParamListOpt ) Block

    ParamListOpt → ParamList
    ParamListOpt → ε

    ParamList → Param
    ParamList → Param , ParamList

    Param → Type id

    Block → { StmtList }

    Expr → Expr + Term
    Expr → Expr - Term
    Expr → Term

    Term → Term * Factor
    Term → Term / Factor
    Term → Factor

    Factor → id
    Factor → number
    Factor → float_number
    Factor → M [ Expr ] [ Expr ]
    Factor → ( Expr )

    Type → int
    Type → float

---

## Test Results

### Test 1: Matrix + Arithmetic

Input:

    M[0][0] = 10;
    M[0][1] = 5;

    int a = M[0][0];
    int b = M[0][1];

    int add = a + b;
    print add;

Output:

    OUTPUT: 15

---

### Test 2: Redeclaration Error

Input:

    int a = 5;
    int a = 10;

Output:

    semantic error: redeclaration of identifier 'a'

---

### Test 3: Undeclared Variable

Input:

    int a = 5;
    int b = a + c;

Output:

    semantic error: undeclared identifier 'c'

---

### Test 4: Function Scope

Input:

    int GlobalValue = 10;

    int main() {
        int X = 10;
        int LocalValue = 5;
        int Result = X + LocalValue;

        print Result;
    }

    print GlobalValue;

Output:

    OUTPUT: 15
    OUTPUT: 10

---

### Test 5: Type Handling (int + float)

Input:

    int a = 10;
    float b = 2.5;

    float result = a + b;

    print result;

Output:

    OUTPUT: 12.500000

---

### Test 6: Complex Arithmetic Expression

Input:

    int B = 8;
    int C = 6;

    int RESULT = B + B * C / 2 - (3 + 4);
    print RESULT;

Output:

    OUTPUT: 17

---

## AST (Abstract Syntax Tree)

The parser builds an AST that represents the structure of the program.

Example:

    int a = 10;
    int b = 5;
    int c = a + b;
    print c;

AST:

    var [a] : int = expr 10;
    var [b] : int = expr 5;
    var [c] : int = expr (a + b);
    print c;

---

## Symbol Table

The compiler uses nested scopes:

    Level 2 → LOCAL
    Level 1 → PARAM
    Level 0 → GLOBAL

Example:

    int GlobalValue = 10;

    int main(int X) {
        int LocalValue = 5;
        int Result = X + LocalValue;
    }

Symbol Table:

    Scope Level 2:
        LocalValue LOCAL
        Result LOCAL

    Scope Level 1:
        X PARAM

    Scope Level 0:
        GlobalValue GLOBAL
        main GLOBAL function

---

## Semantic Analysis

Performs:

    - Name resolution (undeclared variables)
    - Redeclaration checking
    - Type checking
    - Scope handling

---

## Execution

The AST is interpreted:

    - Expressions are evaluated
    - Values are stored in runtime memory
    - Statements are executed
    - Results are printed

---

## Important Notes

    - print is a built-in statement (not a function)
    - Functions are defined but not called
    - Parameter passing is not implemented in execution
    - Matrix supports dynamic indexing using expressions
    - Symbol table uses nested scopes

---

## Conclusion

This compiler successfully implements lexical analysis, parsing, AST construction, semantic analysis, symbol tables, and execution using an interpreter-based approach.

---

## One-line Summary

    A compiler that parses, analyzes, and executes a small language using AST and symbol tables.
