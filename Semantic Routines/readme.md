# Semantic Analysis – Compiler Project


```

## Build and Run

    bison --defines=token.h --output=parser.c parser.bison -v
    flex -o scanner.c scanner.flex
    gcc main.c semantic.c scanner.c parser.c -o compiler.exe -w

    ./compiler.exe tests/valid_all_ops.c
    ./compiler.exe tests/error_redeclaration.c
    ./compiler.exe tests/error_undeclared.c

## Test Results

## Test 1: Valid Program

### Input Source

```c
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
```

### AST (Readable)

```
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
```

### AST (Post-Order)

```
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
```

### Result

```
Semantic analysis finished with 0 error(s).
```

### Symbol Table

| Name | Kind | Type | Which |
|------|------|------|-------|
| mul  | GLOBAL | int | 4 |
| sub  | GLOBAL | int | 3 |
| add  | GLOBAL | int | 2 |
| b    | GLOBAL | int | 1 |
| a    | GLOBAL | int | 0 |

## Test 2: Redeclaration Error

### Input Source

```c
int a = 5;
int b = 10;
int a = b + 1;

M[0][0] = a;
```

### AST (Readable)

```
var [a] : int = expr 5;
var [b] : int = expr 10;
var [a] : int = expr (b + 1);
M[0][0] = a;
```

### AST (Post-Order)

```
5 DECL(a) STMT_DECL
10 DECL(b) STMT_DECL
b 1 + DECL(a) STMT_DECL
M[0][0] a MATRIX_ASSIGN
```

### Errors

```
semantic error: redeclaration of identifier 'a'
Semantic analysis finished with 1 error(s).
```

### Symbol Table

| Name | Kind | Type | Which |
|------|------|------|-------|
| b    | GLOBAL | int | 1 |
| a    | GLOBAL | int | 0 |

## Test 3: Undeclared Variable

### Input Source

```c
M[0][0] = 10;

int a = M[0][0];
int b = a + c;

M[0][1] = b;
```

### AST (Readable)

```
M[0][0] = 10;
var [a] : int = expr M[0][0];
var [b] : int = expr (a + c);
M[0][1] = b;
```

### AST (Post-Order)

```
M[0][0] 10 MATRIX_ASSIGN
M[0][0] DECL(a) STMT_DECL
a c + DECL(b) STMT_DECL
M[0][1] b MATRIX_ASSIGN
```

### Errors

```
semantic error: undeclared identifier 'c'
semantic error: undeclared identifier 'c'
semantic error: arithmetic operands must be integers
semantic error: type mismatch in declaration 'b'
Semantic analysis finished with 4 error(s).
```

### Symbol Table

| Name | Kind | Type | Which |
|------|------|------|-------|
| b    | GLOBAL | int | 1 |
| a    | GLOBAL | int | 0 |

## Conclusion

The compiler successfully performs semantic analysis, symbol table construction, and error detection for valid and invalid programs.
