# Compiler

> A complete compiler pipeline for a restricted C-like language, targeting a **Processing-in-Memory (PIM)** Instruction Set Architecture (ISA).

---

##  Overview

This project translates high-level programs into PIM assembly instructions, demonstrating how computation can be performed directly inside memory.

---

##  Features

- Scanner (Flex)
-  Parser (Bison)
-  AST Construction
-  Semantic Analysis (Type + Scope Checking)
-  Optimization
- Code Generation (PIM ISA)


---

##  Source Language

Supports:
- `int`, `float` variables
- Arithmetic expressions: `+` `-` `*`
- Matrix access: `M[i][j]`
- Assignments, Print statements, Blocks `{}`

### Example

```c
int a = 10;
int b = 5;

int sum = a + b;
print sum;
```

---

## 🔤 Tokens (Lexical Analysis)

### Keywords

| Token | Lexeme | Description |
|---|---|---|
| `TOKEN_INT` | `int` | Integer type |
| `TOKEN_FLOAT` | `float` | Float type |
| `TOKEN_PRINT` | `print` | Output |
| `TOKEN_MATRIX` | `M` | Matrix |

### Identifiers

| Token | Pattern | Example |
|---|---|---|
| `TOKEN_IDENT` | `[A-Za-z][A-Za-z0-9_]*` | `a`, `temp_1` |

### Numeric Literals

| Token | Pattern | Example |
|---|---|---|
| `TOKEN_NUMBER` | `0 \| [1-9][0-9]*` | `10`, `25` |
| `TOKEN_FLOAT_NUMBER` | `[0-9]+\.[0-9]+` | `2.5` |

### Operators

| Token | Symbol | Description |
|---|---|---|
| `TOKEN_ASSIGN` | `=` | Assignment |
| `TOKEN_PLUS` | `+` | Addition |
| `TOKEN_MINUS` | `-` | Subtraction |
| `TOKEN_MUL` | `*` | Multiplication |

### Delimiters

| Token | Symbol | Description |
|---|---|---|
| `TOKEN_LBRACKET` | `[` | Matrix index |
| `TOKEN_RBRACKET` | `]` | Matrix index |
| `TOKEN_LPAREN` | `(` | Grouping |
| `TOKEN_RPAREN` | `)` | Grouping |
| `TOKEN_LBRACE` | `{` | Block start |
| `TOKEN_RBRACE` | `}` | Block end |
| `TOKEN_SEMICOLON` | `;` | Statement end |

---

## 📜 Grammar Rules (Bison)

### Program
```
Program → StmtList
```

### Statements
```
StmtList → StmtList Stmt
StmtList → Stmt
Stmt     → Decl
Stmt     → id = Expr ;
Stmt     → M [ Expr ] [ Expr ] = Expr ;
Stmt     → print Expr ;
```

### Declarations
```
Decl → Type id = Expr ;
```

### Expressions
```
Expr   → Expr + Term
Expr   → Expr - Term
Expr   → Term
Term   → Term * Factor
Term   → Factor
Factor → id
Factor → number
Factor → M [ Expr ] [ Expr ]
```

---

## 🌳 AST (Abstract Syntax Tree)

### AST Node Table

| Source Construct | AST Node | Meaning |
|---|---|---|
| `int a = 10;` | `STMT_DECL → DECL(a)` | Declaration |
| `a + b` | `EXPR_ADD` | Addition |
| `M[i][j]` | `EXPR_MATRIX` | Matrix access |
| `print a;` | `STMT_PRINT` | Print |

### AST Tree Example

```
PROGRAM
└── stmt_list:
    ├── STMT_DECL
    │   └── DECL name=a
    │       ├── TYPE int
    │       └── EXPR_INT value=10
    ├── STMT_DECL
    │   └── DECL name=b
    │       ├── TYPE int
    │       └── EXPR_INT value=5
    ├── STMT_DECL
    │   └── DECL name=sum
    │       ├── TYPE int
    │       └── EXPR_ADD
    │           ├── EXPR_NAME name=a
    │           └── EXPR_NAME name=b
    ├── STMT_MATRIX_ASSIGN
    │   ├── EXPR_MATRIX_REF [row=0][col=2]
    │   └── EXPR_NAME name=sum
    └── STMT_PRINT
        └── EXPR_NAME name=sum
```

---

## 🧠 Symbol Table

| Name | Kind | Type | Scope | Which |
|---|---|---|---|---|
| `a` | GLOBAL | int | 0 | 0 |
| `b` | GLOBAL | int | 0 | 1 |
| `add` | GLOBAL | int | 0 | 2 |

---

## 🔁 Intermediate Representation (IR)

### Example IR

```
STMT 0
DECL a
TYPE 2
EXPR 5
10
```

---

## ⚡ PIM Instruction Set

### Scalar Instructions

| Instruction | Description |
|---|---|
| `sldi` | Load immediate |
| `sadd` | Add |
| `ssub` | Subtract |
| `smul` | Multiply |
| `sld` | Load |

### Memory Instructions

| Instruction | Description |
|---|---|
| `ld` | Global → Local |
| `st` | Local → Global |
| `lldi` | Immediate → Local |

---

## 📐 Matrix Addressing

```
address = BASE + ((row × COLS + col) × 4)
```

| Parameter | Value |
|---|---|
| BASE | 1000 |
| COLS | 5 |
| Element size | 4 bytes |

---

## ⚙️ Code Generation Mapping

| Source Code | PIM Instruction |
|---|---|
| `int a = 10;` | `sldi $1, 10` |
| `c = a + b;` | `sadd $3, $1, $2` |
| `c = a - b;` | `ssub $3, $1, $2` |
| `c = a * b;` | `smul $3, $1, $2` |
| `a = M[i][j];` | `sld $r, addr` |
| `M[i][j] = val;` | `st addr, $r` |

---

## ✅ Test Case (End-to-End)
### Input

```
int a = 8;
int b = 4;
int c = 2;

int result = a + b * c - (3 + 1);

print result;
```

### Code Generation Output

```
===== PIM CODE =====

# declaration a
sldi $1, 8    # integer literal
# variable a stored in register $1

# declaration b
sldi $2, 4    # integer literal
# variable b stored in register $2

# declaration c
sldi $3, 2    # integer literal
# variable c stored in register $3

# declaration result
# use variable a from $1
# use variable b from $2
# use variable c from $3
# multiply left and right expression
smul $4, $2, $3
# add left and right expression
sadd $5, $1, $4
sldi $6, 3    # integer literal
sldi $7, 1    # integer literal
# add left and right expression
sadd $8, $6, $7
# subtract right expression from left expression
ssub $9, $5, $8
# variable result stored in register $9

# print statement
# use variable result from $9
# output value stored in register $9
```

---

## 🛠️ Build & Run

### Build

```bash
bison --defines=token.h --output=parser.c parser.bison -v
flex -o scanner.c scanner.flex
gcc main.c semantic.c execute.c scanner.c parser.c -o compiler.exe -w
```

### Run

```bash
./compiler.exe test.c
```

---

## 📂 Project Structure

```
.
├── scanner.flex
├── parser.bison
├── ast.h
├── semantic.c
├── optimizer.c
├── execute.c
├── code_generator.c
├── main.c
├── tests/
└── README.md
```

---

## 🤖 Generative AI Usage Statement

For this project, I used generative AI tools to support my understanding and development of the compiler.

### Tools Used
- ChatGPT (OpenAI)

### How I Used AI

I used ChatGPT for:
- Understanding compiler concepts such as AST, symbol tables, and code generation
- Debugging issues 
- Clarifying how to structure AST nodes and semantic analysis
- Generating examples for testing and validation
- Improving documentation and README formatting


### Example Prompt

> **Prompt:** "Explain how `stmt_create` works in my AST and how it connects to parser rules"
>
> **How it helped:** This helped me understand how parser rules construct AST nodes and how different fields (`expr`, `decl`, `next`) are used to represent program structure. I then applied this understanding to debug and extend my compiler.

---


