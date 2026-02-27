# Scanner (Flex-Based)

This folder contains a lexical scanner for the restricted C-like language used in the PIM compiler project.

---

## 🔧 Build Instructions

From inside the `Scanner` directory:

make clean
make

This will:
- Run `flex scanner.flex`
- Compile `lex.yy.c`
- Produce an executable named `scanner`

---

## ▶️ Run Instructions

To run the scanner on a test file:

./scanner tests/valid_basic.c
./scanner tests/invalid_chars.c

---

## 🧠 What the Scanner Recognizes

- Keywords: `int`, `M`
- Identifiers: `[A-Za-z][A-Za-z0-9_]*`
- Integers: `0 | [1-9][0-9]*`
- Operators: `= + - *`
- Delimiters: `[ ] ;`
- Single-line comments starting with `//`
- Whitespace (ignored)
- Invalid tokens produce `ERROR`

---

## 📁 Test Files

The `tests/` folder contains:
- `valid_basic.c`
- `invalid_chars.c`
