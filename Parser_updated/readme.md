# Semantic Analysis – Compiler Project

## Build and Run

    bison --defines=token.h --output=parser.c parser.bison -v
    flex -o scanner.c scanner.flex
    gcc main.c semantic.c execute.c scanner.c parser.c -o compiler.exe -w

    ./compiler.exe tests/pass_1_basic.c
    ./compiler.exe tests/pass_3_matrix.c
    ./compiler.exe tests/pass_4_types.c
    ./compiler.exe tests/pass_function_scope.c
    ./compiler.exe tests/error_2_redeclaration.c
    ./compiler.exe tests/error_1_undeclared.c

---

## Grammar Rules

Variables (Non-terminals)

    V = {Program, StmtList, Stmt, Decl, Block, ParamListOpt, ParamList, Param, Expr, Term, Factor, Type}

Terminals

    Σ = {int, float, id, =, ;, +, -, *, /, (, ), {, }, ,, print, M, [, ], number, float_number}

Start Symbol

    S = Program

Production Rules

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

---

### Test 1: Basic Program

Input:

    int a = 10;
    int b = 5;
    int c = a + b;
    print c;

Output:

    Parse successful!

    AST:
    ----------------
    var [a] : int = expr 10;
    var [b] : int = expr 5;
    var [c] : int = expr (a + b);
    print c;

    Semantic Analysis:
    ----------------

    Semantic analysis finished with 0 error(s).

    Symbol Table:
    ----------------

    Nested Symbol Table:
    ====================

    Scope Level 0:
    -------------------------------------
    Name            Kind       Type                 Which
    a               GLOBAL     int                  0
    b               GLOBAL     int                  1
    c               GLOBAL     int                  2

    Execution:
    ----------------
    OUTPUT: 15

---

### Test 2: Matrix + Arithmetic

Input:

    M[0][0] = 10;
    M[0][1] = 5;

    int a = M[0][0];
    int b = M[0][1];

    int add = a + b;
    print add;

Output:

    Parse successful!

    AST:
    ----------------
    M[0][0] = 10;
    M[0][1] = 5;
    var [a] : int = expr M[0][0];
    var [b] : int = expr M[0][1];
    var [add] : int = expr (a + b);
    print add;

    Semantic Analysis:
    ----------------

    Semantic analysis finished with 0 error(s).

    Symbol Table:
    ----------------

    Nested Symbol Table:
    ====================

    Scope Level 0:
    -------------------------------------
    Name            Kind       Type                 Which
    a               GLOBAL     int                  0
    b               GLOBAL     int                  1
    add             GLOBAL     int                  2

    Execution:
    ----------------
    OUTPUT: 15

---

### Test 3: Type Handling (int + float)

Input:

    int a = 10;
    float b = 2.5;

    float result = a + b;
    print result;

Output:

    Parse successful!

    AST:
    ----------------
    var [a] : int = expr 10;
    var [b] : float = expr 2.500000;
    var [result] : float = expr (a + b);
    print result;

    Semantic Analysis:
    ----------------

    Semantic analysis finished with 0 error(s).

    Symbol Table:
    ----------------

    Nested Symbol Table:
    ====================

    Scope Level 0:
    -------------------------------------
    Name            Kind       Type                 Which
    a               GLOBAL     int                  0
    b               GLOBAL     float                1
    result          GLOBAL     float                2

    Execution:
    ----------------
    OUTPUT: 12.500000

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

    Parse successful!

    AST:
    ----------------
    var [GlobalValue] : int = expr 10;
    var [main] : function int code {
    var [X] : int = expr 10;
    var [LocalValue] : int = expr 5;
    var [Result] : int = expr (X + LocalValue);
    print Result;
    }
    print GlobalValue;

    Semantic Analysis:
    ----------------

    Semantic analysis finished with 0 error(s).

    Symbol Table:
    ----------------

    Nested Symbol Table:
    ====================

    Scope Level 2:
    -------------------------------------
    Name            Kind       Type                 Which
    X               LOCAL      int                  0
    LocalValue      LOCAL      int                  1
    Result          LOCAL      int                  2

    Scope Level 1:
    -------------------------------------
    Name            Kind       Type                 Which

    Scope Level 0:
    -------------------------------------
    Name            Kind       Type                 Which
    GlobalValue     GLOBAL     int                  0
    main            GLOBAL     function int         1

    Execution:
    ----------------
    OUTPUT: 15
    OUTPUT: 10

---

### Test 5: Redeclaration Error

Input:

    int a = 5;
    int a = 10;

Output:

    Parse successful!

    AST:
    ----------------
    var [a] : int = expr 5;
    var [a] : int = expr 10;

    Semantic Analysis:
    ----------------
    semantic error: redeclaration of identifier 'a'

    Semantic analysis finished with 1 error(s).

---

### Test 6: Undeclared Variable

Input:

    int a = 5;
    int b = a + c;

Output:

    Parse successful!

    AST:
    ----------------
    var [a] : int = expr 5;
    var [b] : int = expr (a + c);

    Semantic Analysis:
    ----------------
    semantic error: undeclared identifier 'c'
    semantic error: undeclared identifier 'c'
    semantic error: arithmetic operands must be numeric
    semantic error: type mismatch in declaration 'b'

    Semantic analysis finished with 4 error(s).


