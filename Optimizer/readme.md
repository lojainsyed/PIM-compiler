# 🧠 Compiler with Optimizer (AST → IR → PIM Code)

## 📌 Overview

This project implements a complete compiler pipeline with optimization support. The compiler processes input programs, builds an Abstract Syntax Tree (AST), performs semantic analysis, applies optimizations, generates Intermediate Representation (IR), reconstructs the AST, and finally produces assembly-like PIM code.

---

## ⚙️ Compiler Pipeline

```
Input Program
    ↓
Parsing (Flex + Bison)
    ↓
AST Generation
    ↓
Semantic Analysis
    ↓
Symbol Table Construction
    ↓
Optimizer
    ↓
IR Generation (File)
    ↓
AST Reconstruction
    ↓
Code Generation (PIM Assembly)
```

---

## 🚀 How to Run

### Compile

```bash
gcc main.c scanner.c parser.c semantic.c ast_io.c code_generator.c optimizer.c -o compiler.exe -w
```

### Run Single Test

```bash
./compiler.exe tests_opt/opt_combined.c
```

### Run All Tests

```bash
for file in tests_opt/*.c
do
    echo "Running $file"
    ./compiler.exe $file
done
```

---

## 🔧 Implemented Optimizations

### 1. Constant Folding

Evaluates constant expressions at compile time.

**Example**

```c
int a = 5 + 5;
```

➡ Optimized:

```c
int a = 10;
```

---

### 2. Strength Reduction

Replaces expensive operations with simpler ones.

**Example**

```c
int b = a * 2;
```

➡ Optimized:

```c
int b = a + a;
```

---

### 3. Dead Code Elimination

Removes variables that are never used.

**Example**

```c
int d = 100;
```

➡ Removed if unused

---

### 4. Algebraic Simplification (Optional)

Simplifies expressions using identities.

**Rules**

```
x + 0 → x
0 + x → x
x - 0 → x
x * 1 → x
1 * x → x
x * 0 → 0
x / 1 → x
```

---

## 🧪 Test Cases & Results

---

### ✅ Test 1: Constant Folding

**Input**

```c
int a = 5 + 5;
int b = 10 - 3;
int c = 4 * 2;
int d = 20 / 4;

print a;
print b;
print c;
print d;
```

**Output**

```c
a = 10
b = 7
c = 8
d = 5
```

**Impact**

```
Operations Before: 4
Operations After : 0
Reduction        : 100%
```

---

### ✅ Test 2: Strength Reduction

**Input**

```c
int a = 6;
int b = a * 2;
int c = 2 * a;

print b;
print c;
```

**Output**

```c
b = a + a
c = a + a
```

**Impact**

```
Operations unchanged (2 → 2)
BUT multiplication removed (cheaper execution)
```

---

### ✅ Test 3: Dead Code Elimination

**Input**

```c
int a = 10;
int b = 20;
int c = 30;

print a;
```

**Output**

```c
int a = 10;
print a;
```

**Impact**

```
Statements Before: 4
Statements After : 2
Removed          : 2
```

---

### ✅ Test 4: Algebraic Simplification

**Input**

```c
int a = 10;
int b = a + 0;
int c = 0 + a;
int d = a - 0;
int e = a * 1;
int f = 1 * a;
int g = a * 0;

print b;
print c;
print d;
print e;
print f;
print g;
```

**Output**

```c
b = a
c = a
d = a
e = a
f = a
g = 0
```

**Impact**

```
Operations Before: 6
Operations After : 0
Reduction        : 100%
```

---

### ✅ Test 5: Combined Optimization (Best Demo)

**Input**

```c
int a = 5 + 5;
int b = a * 2;
int c = b + 0;
int d = 100;
int e = 0 + c;
int f = c * 1;

print f;
```

**Output**

```c
a = 10
b = a + a
c = b
f = c
print f
```

**Impact**

```
Statements Before: 7
Statements After : 5
Removed          : 2

Operations Before: 5
Operations After : 1
Reduction        : 80%
```

---

## 🧠 Optimization Impact Summary

| Optimization             | Effect                      |
| ------------------------ | --------------------------- |
| Constant Folding         | Removes runtime computation |
| Strength Reduction       | Replaces expensive ops      |
| Dead Code                | Reduces program size        |
| Algebraic Simplification | Simplifies expressions      |

---

## 🏁 Final Output

The compiler generates:

✔ Optimized AST
✔ IR file (`ir.txt`)
✔ Reconstructed AST
✔ PIM Assembly Code

---

## 🎯 Conclusion

This project demonstrates a full compiler pipeline with multiple optimization techniques. The optimizer effectively reduces computation, simplifies expressions, and eliminates redundant code, resulting in efficient final assembly generation.

---

## 👩‍💻 Author

Lojain Syed
PhD Student – ECE
University of Arizona
