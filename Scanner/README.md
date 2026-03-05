
# Scanner (Flex-Based)

This directory contains a **Flex-based lexical scanner** for the restricted C-like language used in the PIM compiler project.
The scanner reads a source file and prints the tokens recognized from the input.

---

## Directory Structure

```

    Scanner/
    ├── scanner.flex
    ├── main.c
    ├── token.h
    ├── Makefile
    ├── scanner
    ├── lex.yy.c
    └── tests/
        ├── valid_all_ops.c
        ├── valid_comments.c
        ├── valid_ids.c
        ├── valid_ints.c
        ├── valid_whitespace.c
        ├── invalid_bad_comment.c
        ├── invalid_chars.c
        ├── invalid_ids.c
        └── invalid_int.c
```

---

## Build

From inside the `Scanner` directory:

```

make
```

ThIS command execute:

```
flex scanner.flex
gcc lex.yy.c main.c -o scanner
```

This produces the executable `scanner`.

---

## Run

```
./scanner tests/<filename>
```

Example:

```
./scanner tests/valid_all_ops.c
```

---

## Output Format

```
token: <token_number> text: <lexeme>
```

Example:

```
token: 1 text: int
token: 3 text: a
token: 5 text: =
token: 2 text: M
token: 9 text: [
token: 4 text: 0
token: 10 text: ]
token: 11 text: ;
```

---

## Token Numbers

Defined in `token.h`.

| Token | Meaning    |
| ----- | ---------- |
| 1     | int        |
| 2     | M          |
| 3     | Identifier |
| 4     | Integer    |
| 5     | =          |
| 6     | +          |
| 7     | -          |
| 8     | *          |
| 9     | [          |
| 10    | ]          |
| 11    | ;          |
| 12    | ERROR      |

---

## What the Scanner Recognizes

The scanner identifies the following elements of the language:

* **Keywords:** `int`, `M`
* **Identifiers:** `[A-Za-z][A-Za-z0-9_]*`
* **Integers:** `0` or `[1-9][0-9]*`
* **Operators:** `=`, `+`, `-`, `*`
* **Delimiters:** `[`, `]`, `;`
* **Comments:** Single-line comments beginning with `//`
* **Whitespace:** Spaces, tabs, and newlines (ignored)

Any character that does not match these rules is reported as an **ERROR token**.

---

## Tests

The `tests` folder contains valid and invalid programs for verifying scanner behavior.
Invalid inputs should produce **ERROR tokens**.

---

## Clean

```
make clean
```

Removes generated files (`scanner`, `lex.yy.c`).
