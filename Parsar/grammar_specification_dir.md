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

State | int | id | = | ; | + | - | * | M | [int][int] | $ || P | L | S | X | Y | R
--------------------------------------------------------------------------------------
0     | s3  | s4 |   |   |   |   |   | s6|            |   ||   | 1 | 2 |   |   | 5
1     | s3  | s4 |   |   |   |   |   | s6|            |acc||   |   | 7 |   |   | 5
2     | r2  | r2 |   |   |   |   |   | r2|            |r2 ||   |   |   |   |   |
3     |     | s8 |   |   |   |   |   |   |            |   ||   |   |   |   |   |
4     |     |    | s9|   |   |   |   |   |            |   ||   |   |   |   |   |
5     |     |    |s10|   |   |   |   |   |            |   ||   |   |   |   |   |
6     |     |    |   |   |   |   |   |   | s11        |   ||   |   |   |   |   |
7     | r1  | r1 |   |   |   |   |   | r1|            |r1 ||   |   |   |   |   |
8     |     |    |s12|   |   |   |   |   |            |   ||   |   |   |   |   |
9     | s16 | s15|   |   |   |   |   | s18|           |   ||   |   |   |13 |14 |17
10    | s16 | s15|   |   |   |   |   | s18|           |   ||   |   |   |19 |14 |17
11    |     |    |r13|   |   |   |   |   |            |   ||   |   |   |   |   |
12    | s16 | s15|   |   |   |   |   | s18|           |   ||   |   |   |20 |14 |17
13    |     |    |   |s21|   |   |   |   |            |   ||   |   |   |   |   |
14    |     |    |   |r6 |s22|s23|s24|   |            |   ||   |   |   |   |   |
15    |     |    |   |r10|r10|r10|r10|   |            |   ||   |   |   |   |   |
16    |     |    |   |r11|r11|r11|r11|   |            |   ||   |   |   |   |   |
17    |     |    |   |r12|r12|r12|r12|   |            |   ||   |   |   |   |   |
18    |     |    |   |   |   |   |   |   | s25        |   ||   |   |   |   |   |
19    |     |    |   |s26|   |   |   |   |            |   ||   |   |   |   |   |
20    |     |    |   |s27|   |   |   |   |            |   ||   |   |   |   |   |
21    | r4  | r4 |   |   |   |   |   | r4|            |r4 ||   |   |   |   |   |
22    | s30 | s29|   |   |   |   |   | s32|           |   ||   |   |   |   |28 |31
23    | s30 | s29|   |   |   |   |   | s32|           |   ||   |   |   |   |33 |31
24    | s30 | s29|   |   |   |   |   | s32|           |   ||   |   |   |   |34 |31
25    |     |    |   |r13|r13|r13|r13|   |            |   ||   |   |   |   |   |
26    | r5  | r5 |   |   |   |   |   | r5|            |r5 ||   |   |   |   |   |
27    | r3  | r3 |   |   |   |   |   | r3|            |r3 ||   |   |   |   |   |
28    |     |    |   |r7 |   |   |   |   |            |   ||   |   |   |   |   |
29    |     |    |   |r10|   |   |   |   |            |   ||   |   |   |   |   |
30    |     |    |   |r11|   |   |   |   |            |   ||   |   |   |   |   |
31    |     |    |   |r12|   |   |   |   |            |   ||   |   |   |   |   |
32    |     |    |   |   |   |   |   |   | s35        |   ||   |   |   |   |   |
33    |     |    |   |r8 |   |   |   |   |            |   ||   |   |   |   |   |
34    |     |    |   |r9 |   |   |   |   |            |   ||   |   |   |   |   |
35    |     |    |   |r13|   |   |   |   |            |   ||   |   |   |   |   |

no shift/reduce conflicts

no reduce/reduce conflicts

### LALR Automaton

Re-draw the LR(1) automaton after merging states

Present in directory 



