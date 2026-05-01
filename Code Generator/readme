# Code Generator for PIM ISA

## Overview

This project implements a complete compiler pipeline for a simplified C-like language targeting a Processing-in-Memory (PIM) Instruction Set Architecture (ISA). The compiler parses input programs, constructs an Abstract Syntax Tree (AST), performs semantic analysis, writes the AST to an intermediate representation (IR), reconstructs the AST from the IR, and generates PIM assembly code.

The generated assembly follows the ISA instructions defined in the PIM architecture.

---

## Compiler Pipeline

1. Lexical Analysis (Flex)
2. Syntax Analysis (Bison)
3. AST Construction
4. Semantic Analysis (Symbol Table + Type Checking)
5. AST → IR (File Writing)
6. IR → AST Reconstruction
7. Code Generation (PIM Assembly)

---

## Register Strategy

* Scalar variables are stored in registers
* Each variable is mapped to a unique register
* Example mapping:

  * a → $1
  * b → $2
  * c → $3

Registers are used for intermediate computation and final results.

---

## Memory Model

| Memory Type           | Usage                                    |
| --------------------- | ---------------------------------------- |
| Registers ($1, $2, …) | Scalar variables and intermediate values |
| Local Memory (LMem)   | Temporary storage for store operations   |
| Global Memory (GMem)  | Matrix storage                           |

---

## Instruction Set Summary

| Instruction | Type   | Description                         |
| ----------- | ------ | ----------------------------------- |
| sldi        | Scalar | Load immediate into register        |
| sld         | Scalar | Load from global memory to register |
| sadd        | Scalar | Add two registers                   |
| ssub        | Scalar | Subtract two registers              |
| smul        | Scalar | Multiply two registers              |
| saddi       | Scalar | Add immediate                       |
| smuli       | Scalar | Multiply immediate                  |
| ld          | Memory | Global memory → Local memory        |
| st          | Memory | Local memory → Global memory        |
| lldi        | Memory | Immediate → Local memory            |
| lmv         | Memory | Local memory copy                   |

---

## Load vs Store

| Operation                          | Instruction |
| ---------------------------------- | ----------- |
| Load from Global Memory → Register | sld         |
| Store Local Memory → Global Memory | st          |
| Immediate → Local Memory           | lldi        |

---

## Matrix Addressing

Matrix is stored in global memory using row-major order:

```
address = BASE + ((row * COLS + col) * element_size)
```

Where:

* BASE = 1000
* COLS = 5
* element_size = 4 bytes

---

## Code Generation Strategy

### Scalar Variables

Stored in registers using a variable-to-register mapping.

### Matrix Write

```
M[i][j] = value
```

Steps:

1. Compute global memory address
2. Use `lldi` to store immediate into local memory
3. Use `st` to store into global memory

### Matrix Read

```
x = M[i][j]
```

Steps:

1. Compute global memory address
2. Use `sld` to load into register

---

## Test Case 1: Basic Arithmetic (pass_1_basic.c)

### Input

```
int a = 10;
int b = 5;

int c = a + b;

print c;
```

### Code Generation Output

```
===== PIM CODE =====

# declaration a
sldi $1, 10    # integer literal
# variable a stored in register $1

# declaration b
sldi $2, 5    # integer literal
# variable b stored in register $2

# declaration c
# use variable a from $1
# use variable b from $2
# add left and right expression
sadd $3, $1, $2
# variable c stored in register $3

# print statement
# use variable c from $3
# output value stored in register $3
```

---

## Test Case 2: Expressions (pass_2_expressions.c)

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

## Test Case 3: Matrix Operations (pass_3_matrix.c)

### Input

```
M[0][0] = 10;
M[0][1] = 5;

int a = M[0][0];
int b = M[0][1];

int sum = a + b;

M[0][2] = sum;

print sum;
```

### Code Generation Output

```
===== PIM CODE =====

# matrix write M[0][0] = 10
sldi $1, 0
sldi $2, 0
sldi $3, 5
smul $4, $1, $3
sadd $4, $4, $2
sldi $5, 4
smul $4, $4, $5
sldi $6, 1000
sadd $8, $6, $4
sldi $9, 200
lldi $9, 10, 4, 0
st $8, $9, 4, 0

# matrix write M[0][1] = 5
sldi $10, 0
sldi $11, 1
sldi $12, 5
smul $13, $10, $12
sadd $13, $13, $11
sldi $14, 4
smul $13, $13, $14
sldi $15, 1000
sadd $16, $15, $13
sldi $17, 200
lldi $17, 5, 4, 0
st $16, $17, 4, 0

# matrix read M[0][0]
sldi $19, 0
sldi $20, 0
sldi $21, 5
smul $22, $19, $21
sadd $22, $22, $20
sldi $23, 4
smul $22, $22, $23
sldi $24, 1000
sadd $26, $24, $22
sld $18, $26, 0

# matrix read M[0][1]
sldi $28, 0
sldi $29, 1
sldi $30, 5
smul $31, $28, $30
sadd $31, $31, $29
sldi $32, 4
smul $31, $31, $32
sldi $33, 1000
sadd $34, $33, $31
sld $27, $34, 0

# sum = a + b
sadd $35, $18, $27

# matrix write M[0][2] = sum
sldi $36, 0
sldi $37, 2
sldi $38, 5
smul $39, $36, $38
sadd $39, $39, $37
sldi $40, 4
smul $39, $39, $40
sldi $41, 1000
sadd $42, $41, $39
sldi $43, 200
st $42, $43, 4, 0

# print sum
# output value stored in register $35
```

---

## Notes

* Registers are dynamically assigned
* Scalar variables are mapped to registers
* Matrix values are stored in global memory
* Local memory is used for intermediate storage before global writes
* Some instructions require even-numbered registers for memory operations

---

## Conclusion

This project demonstrates a full compiler pipeline targeting a PIM ISA. It supports scalar computation, complex expressions, and matrix memory operations while respecting ISA constraints. The generated assembly clearly reflects how high-level constructs map to low-level instructions.
