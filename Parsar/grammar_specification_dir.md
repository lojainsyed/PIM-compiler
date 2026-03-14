# Context-Free Grammar Specification


Write the description of your context-free grammar here.

Example: *This grammar defines simple arithmetic expressions with addition, subtraction, multiplication, division, and numbers.*


## Formal Definition

The grammar G is a 4-tuple (V, Σ, R, S):

### V (Variables/Non-terminals):

Write the Non-terminals set here.

V = {P, L, S, X, Y, R}

### Σ (Terminals):

Write the Terminals set here.

Σ = {int, id, =, ;, +, -, *, M, [, ]}

### S (Start Symbol):

Write the Start Symbol here.
S = P

### R (Production Rules):

P --> L
L --> L S
L --> S
S --> int id = X ;
S --> id = X ;
S --> R = X ;
X --> Y
X --> Y + Y
X --> Y - Y
X --> Y * Y
Y --> id
Y --> int
Y --> R
R --> M [ int ] [ int ]


## LR(1) Automaton


Draw the LR(1) automaton for your grammar


Its Present in the directory


## LALR Verification

### Item Sets: 

List the states that can be merged


### Parse Table and Conflict Check


| State | int | id  | =   | ;   | +   | -   | *   | integer | M   | [   | ]   | $   | P | L | S | X  | Y  | R  |
| ----- | --- | --- | --- | --- | --- | --- | --- | ------- | --- | --- | --- | --- | - | - | - | -- | -- | -- |
| 0     | s3  | s4  |     |     |     |     |     |         | s6  |     |     |     |   | 1 | 2 |    |    | 5  |
| 1     | s3  | s4  |     |     |     |     |     |         | s6  |     |     | acc |   |   | 7 |    |    | 5  |
| 2     | r2  | r2  |     |     |     |     |     |         | r2  |     |     | r2  |   |   |   |    |    |    |
| 3     |     | s8  |     |     |     |     |     |         |     |     |     |     |   |   |   |    |    |    |
| 4     |     |     | s9  |     |     |     |     |         |     |     |     |     |   |   |   |    |    |    |
| 5     |     |     | s10 |     |     |     |     |         |     |     |     |     |   |   |   |    |    |    |
| 6     |     |     |     |     |     |     |     |         |     | s11 |     |     |   |   |   |    |    |    |
| 7     | r1  | r1  |     |     |     |     |     |         | r1  |     |     | r1  |   |   |   |    |    |    |
| 8     |     |     | s12 |     |     |     |     |         |     |     |     |     |   |   |   |    |    |    |
| 9     |     | s15 |     |     |     |     |     | s16     | s18 |     |     |     |   |   |   | 13 | 14 | 17 |
| 10    |     | s15 |     |     |     |     |     | s16     | s18 |     |     |     |   |   |   | 19 | 14 | 17 |
| 11    |     |     |     |     |     |     |     | s20     |     |     |     |     |   |   |   |    |    |    |
| 12    |     | s15 |     |     |     |     |     | s16     | s18 |     |     |     |   |   |   | 21 | 14 | 17 |
| 13    |     |     |     | s22 |     |     |     |         |     |     |     |     |   |   |   |    |    |    |
| 14    |     |     |     | r6  | s23 | s24 | s25 |         |     |     |     |     |   |   |   |    |    |    |
| 15    |     |     |     | r10 | r10 | r10 | r10 |         |     |     |     |     |   |   |   |    |    |    |
| 16    |     |     |     | r11 | r11 | r11 | r11 |         |     |     |     |     |   |   |   |    |    |    |
| 17    |     |     |     | r12 | r12 | r12 | r12 |         |     |     |     |     |   |   |   |    |    |    |
| 18    |     |     |     |     |     |     |     |         |     | s26 |     |     |   |   |   |    |    |    |
| 19    |     |     |     | s27 |     |     |     |         |     |     |     |     |   |   |   |    |    |    |
| 20    |     |     |     |     |     |     |     |         |     |     | s28 |     |   |   |   |    |    |    |
| 21    |     |     |     | s29 |     |     |     |         |     |     |     |     |   |   |   |    |    |    |
| 22    | r4  | r4  |     |     |     |     |     |         | r4  |     |     | r4  |   |   |   |    |    |    |
| 23    |     | s31 |     |     |     |     |     | s32     | s34 |     |     |     |   |   |   |    | 30 | 33 |
| 24    |     | s31 |     |     |     |     |     | s32     | s34 |     |     |     |   |   |   |    | 35 | 33 |
| 25    |     | s31 |     |     |     |     |     | s32     | s34 |     |     |     |   |   |   |    | 36 | 33 |
| 26    |     |     |     |     |     |     |     | s37     |     |     |     |     |   |   |   |    |    |    |
| 27    | r5  | r5  |     |     |     |     |     |         | r5  |     |     | r5  |   |   |   |    |    |    |
| 28    |     |     |     |     |     |     |     |         |     | s38 |     |     |   |   |   |    |    |    |
| 29    | r3  | r3  |     |     |     |     |     |         | r3  |     |     | r3  |   |   |   |    |    |    |
| 30    |     |     |     | r7  |     |     |     |         |     |     |     |     |   |   |   |    |    |    |
| 31    |     |     |     | r10 |     |     |     |         |     |     |     |     |   |   |   |    |    |    |
| 32    |     |     |     | r11 |     |     |     |         |     |     |     |     |   |   |   |    |    |    |
| 33    |     |     |     | r12 |     |     |     |         |     |     |     |     |   |   |   |    |    |    |
| 34    |     |     |     |     |     |     |     |         |     | s39 |     |     |   |   |   |    |    |    |
| 35    |     |     |     | r8  |     |     |     |         |     |     |     |     |   |   |   |    |    |    |
| 36    |     |     |     | r9  |     |     |     |         |     |     |     |     |   |   |   |    |    |    |
| 37    |     |     |     |     |     |     |     |         |     |     | s40 |     |   |   |   |    |    |    |
| 38    |     |     |     |     |     |     |     | s41     |     |     |     |     |   |   |   |    |    |    |
| 39    |     |     |     |     |     |     |     | s42     |     |     |     |     |   |   |   |    |    |    |
| 40    |     |     |     |     |     |     |     |         |     | s43 |     |     |   |   |   |    |    |    |
| 41    |     |     |     |     |     |     |     |         |     |     | s44 |     |   |   |   |    |    |    |
| 42    |     |     |     |     |     |     |     |         |     |     | s45 |     |   |   |   |    |    |    |
| 43    |     |     |     |     |     |     |     | s46     |     |     |     |     |   |   |   |    |    |    |
| 44    |     |     | r13 |     |     |     |     |         |     |     |     |     |   |   |   |    |    |    |
| 45    |     |     |     |     |     |     |     |         |     | s47 |     |     |   |   |   |    |    |    |
| 46    |     |     |     |     |     |     |     |         |     |     | s48 |     |   |   |   |    |    |    |
| 47    |     |     |     |     |     |     |     | s49     |     |     |     |     |   |   |   |    |    |    |
| 48    |     |     |     | r13 | r13 | r13 | r13 |         |     |     |     |     |   |   |   |    |    |    |
| 49    |     |     |     |     |     |     |     |         |     |     | s50 |     |   |   |   |    |    |    |
| 50    |     |     |     | r13 |     |     |     |         |     |     |     |     |   |   |   |    |    |    |


no shift/reduce conflicts

no reduce/reduce conflicts

### LALR Automaton

Re-draw the LR(1) automaton after merging states

Present in directory 



