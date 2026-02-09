# Compiler Project – Initial Definition

## 1. Student Information
- **Name:Lojain Syed
- **Program:Ph.D. in Electrical and Computer Engineering
- **Course:Compiler Design
- **Semester / Year: Spring 2026

## 2. Project Overview
This project focuses on the design and implementation of a compiler for a Processing-in-Memory (PIM) architecture, with support for validation using an existing PIM simulator[2]. The compiler translates a restricted C-like source language into an assembly-like instruction sequence defined by a domain-specific PIM Instruction Set Architecture (ISA), which can be functionally evaluated using a publicly available PIM simulator [1].
The target ISA is a research-defined PIM ISA proposed for executing matrix- and vector-intensive workloads directly inside memory hardware [1]. The same work also provides a simulator (PIMSIM-NN) that models the functional and cycle-level behavior of the PIM architecture without requiring physical hardware. In this project, the simulator is used only as an external validation tool to verify that the compiler-generated instructions conform to the semantics of the target ISA.

The primary objective of the project is to demonstrate the complete compilation pipeline, including lexical analysis, parsing, intermediate representation (IR) generation, and code generation, while targeting an emerging memory-centric architecture. This project is only a compiler: it does not implement an assembler, does not generate machine code, and does not execute instructions on real hardware. The compiler terminates at the generation of ISA-level, assembly-like PIM instructions, which correspond directly to the instruction definitions provided in the PIM ISA specification [1].
## 3. Source Language
- **Name of the source code language:**
Restricted C-like Matrix Language

The source language supports:

integer variable declarations

two-dimensional integer matrix declarations

matrix indexing using [][]

arithmetic expressions using +, -, *

assignment statements

semicolon-terminated statements

The language is intentionally restricted to simplify parsing and to enable a direct and transparent mapping from high-level matrix operations to PIM-oriented instructions, such as matrix-vector multiplication, as defined in the target PIM ISA [1].

## 4. Target / Output Language
- **Output of the compiler:**  
The output of the compiler is an assembly-like sequence of Processing-in-Memory (PIM) ISA instructions, defined by a domain-specific, research-based instruction set [1]. These instructions explicitly represent scalar, vector, and matrix operations intended for execution within memory-centric architectures.
- **Reason for choosing this output language:**
The PIM ISA described in [1] introduces specialized instructions that significantly reduce data movement by performing computation directly inside memory. Targeting this ISA allows the compiler to clearly demonstrate how high-level matrix-based programs can be translated into domain-specific PIM instructions, highlighting the interaction between compiler design and emerging hardware architectures.

## 5. Compiler Features (Planned)
Lexical analysis

Syntax analysis 

Semantic analysis

Intermediate representation (IR)

Basic optimization

Code generation (PIM ISA)

## 6. Tools and Libraries
Programming language: C

Lexer generator: Flex 

Parser generator: Bison

Build tool: Makefile

Libraries: Standard C library

## 7. Repository Information
- **Repository hosting service:Github
 
- **Repository URL:https://github.com/lojainsyed/PIM-compiler/edit/main/

## 8. Project Status
- [x] Repository created ] Repository created
- [ ] README file added
- [ ] Initial project structure committed

## 9. Additional Notes
The target Processing-in-Memory (PIM) ISA used in this project is designed for RRAM-based PIM architectures, which currently exist primarily as research prototypes rather than commercial products. As a result, the project does not rely on physical hardware and instead uses an existing PIM simulator [2] to validate the correctness of the compiler-generated ISA instructions.
References
[1]Chen X. Instruction set architecture (isa) for processing-in-memory dnn accelerators. arXiv preprint arXiv:2308.06449. 2023 Aug 12.
[2] https://github.com/wangxy-2000/pimsim-nn

