# Compiler Project – Initial Definition

## 1. Student Information
- **Name:Lojain Syed
- **Program:Ph.D. in Electrical and Computer Engineering
- **Course:Compiler Design
- **Semester / Year: Spring 2026

## 2. Project Overview
This project implements a compiler for a Processing-in-Memory (PIM) architecture that translates  C language into a domain-specific PIM Instruction Set Architecture (ISA). The target ISA is based on the specification in [1] and is validated using a publicly available PIM simulator, which serves solely as an external tool to verify the correctness and semantics of the generated instructions.

The project demonstrates a complete compilation pipeline, including lexical analysis, parsing, intermediate representation (IR) generation, and code generation, targeting an emerging memory-centric computing architecture
## 3. Source Language
- **Name of the source code language:**
Restricted C Language

The source language supports:

integer variable declarations

two-dimensional integer matrix declarations

matrix indexing using [][]

arithmetic expressions using +, -, *

assignment statements

semicolon-terminated statements

The language is intentionally restricted to simplify parsing and to enable a direct and transparent mapping from high-level matrix operations to PIM-oriented instructions, such as matrix-vector multiplication, as defined in the target PIM ISA.

## 4. Target / Output Language
- **Output of the compiler:**  
The output of the compiler is a  Processing-in-Memory (PIM) ISA instructions defined by a research-based, domain-specific instruction set [1]. These instructions explicitly represent scalar, vector, and matrix operations intended for execution within memory-centric architectures.
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
- [x] README file added
- [ ] Initial project structure committed

## 9. Additional Notes
The target Processing-in-Memory (PIM) ISA used in this project is designed for RRAM(resistive random-access memory)-based PIM architectures, which currently exist primarily as research prototypes rather than commercial products [2]. As a result, the project does not rely on physical hardware and instead uses an existing PIM simulator [2] to validate the correctness of the compiler-generated ISA instructions.
References

[1]Chen X. Instruction set architecture (isa) for processing-in-memory dnn accelerators. arXiv preprint arXiv:2308.06449. 2023 Aug 12

https://arxiv.org/pdf/2308.06449

[2] https://github.com/wangxy-2000/pimsim-nn









