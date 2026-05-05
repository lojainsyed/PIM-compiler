# 🧠 IM Compiler

> A complete compiler pipeline for a restricted C-like language, targeting a **Processing-in-Memory (PIM)** Instruction Set Architecture (ISA).

---

## 📖 Overview

This project translates high-level programs into PIM assembly instructions, demonstrating how computation can be performed directly inside memory.

---

## 🎯 Features

- ✅ Lexical Analysis (Flex)
- ✅ Syntax Analysis (Bison)
- ✅ AST Construction
- ✅ Semantic Analysis (Type + Scope Checking)
- ✅ Intermediate Representation (IR)
- ✅ Optimization
- ✅ Code Generation (PIM ISA)
- ✅ Execution

---

## 🧠 Source Language

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
└── STMT_LIST
    ├── DECL a = 10
    ├── DECL b = 5
    ├── DECL add = (a + b)
    ├── MATRIX_ASSIGN M[0][2] = add
    └── PRINT add
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

```c
M[0][0] = 10;
M[0][1] = 5;

int a = M[0][0];
int b = M[0][1];

int add = a + b;

M[0][2] = add;

print add;
```

### Generated PIM Assembly

```asm
sldi $1, 10
sldi $2, 5
sadd $3, $1, $2
st M[0][2], $3
```

### Output

```
OUTPUT: 15
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
- Debugging issues in Flex/Bison integration
- Clarifying how to structure AST nodes and semantic analysis
- Generating examples for testing and validation
- Improving documentation and README formatting

### Important Note

All core implementation — including the Scanner (Flex), Parser (Bison), AST design, Semantic analysis, Code generation, and Optimizer logic — was written and understood by me. AI was used only as a support tool for explanation and guidance.

### Example Prompt

> **Prompt:** "Explain how `stmt_create` works in my AST and how it connects to parser rules"
>
> **How it helped:** This helped me understand how parser rules construct AST nodes and how different fields (`expr`, `decl`, `next`) are used to represent program structure. I then applied this understanding to debug and extend my compiler.

---

*If I had not used any AI tools, I would explicitly state that. However, in this project, AI was used responsibly as a learning aid and not as a replacement for implementation work.*
