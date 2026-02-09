# PIM Compiler

This repository contains a compiler for a **Processing-in-Memory (PIM)** architecture. The compiler translates a **restricted C-like source language** into a **domain-specific PIM Instruction Set Architecture (ISA)** and validates the generated instructions using an existing open-source PIM simulator.

The project is developed as part of a **Compiler Design** course and focuses on demonstrating the **complete compilation pipeline** rather than targeting commercial hardware.

---

## Project Objectives
- Design a simple, well-defined source language suitable for PIM execution
- Implement core compiler phases:
  - Lexical analysis
  - Parsing
  - Semantic analysis
  - Intermediate Representation (IR) generation
  - Code generation
- Translate high-level matrix-based programs into **PIM ISA instructions**
- Validate correctness using a PIM simulator

---

## Source Language
The source language is a **restricted C-like language** designed for simplicity and clarity. It supports:
- Integer scalar variable declarations
- Two-dimensional integer matrix declarations
- Matrix indexing using `[][]`
- Arithmetic expressions using `+`, `-`, `*`
- Assignment statements
- Semicolon-terminated statements
- Single-line comments (`//`)

The restrictions simplify parsing and enable a direct mapping to PIM-oriented operations.

---

## Target / Output Language
The output of the compiler is a sequence of **Processing-in-Memory (PIM) ISA instructions**, based on a research-defined instruction set architecture.  
These instructions explicitly represent scalar, vector, and matrix operations intended to execute directly within memory-centric architectures.

---

## Compiler Pipeline
The compiler follows a traditional multi-stage design:

1. **Lexical Analysis**  
   Tokenizes the source code using Flex.

2. **Syntax Analysis**  
   Parses tokens according to the grammar using Bison.

3. **Semantic Analysis**  
   Performs basic semantic checks and symbol table management.

4. **Intermediate Representation (IR)**  
   Generates an intermediate form to decouple parsing from code generation.

5. **Code Generation**  
   Emits PIM ISA instructions corresponding to the IR.

---

## Tools and Technologies
- **Programming Language:** C  
- **Lexer Generator:** Flex  
- **Parser Generator:** Bison  
- **Build System:** Makefile  
- **Libraries:** Standard C library  

---

## Repository Structure
