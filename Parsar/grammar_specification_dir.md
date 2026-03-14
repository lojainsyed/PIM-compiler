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

| State | int | id   | =    | ;    | +    | -    | *    | integer | M   | [    | ]    | $   | P | L  | S  | X   | Y   | R   |
| ----- | --- | ---- | ---- | ---- | ---- | ---- | ---- | ------- | --- | ---- | ---- | --- | - | -- | -- | --- | --- | --- |
| J0    | sJ3 | sJ4  |      |      |      |      |      |         | sJ6 |      |      |     |   | J1 | J2 |     |     | J5  |
| J1    | sJ3 | sJ4  |      |      |      |      |      |         | sJ6 |      |      | acc |   |    | J7 |     |     | J5  |
| J2    | r2  | r2   |      |      |      |      |      |         | r2  |      |      | r2  |   |    |    |     |     |     |
| J3    |     | sJ8  |      |      |      |      |      |         |     |      |      |     |   |    |    |     |     |     |
| J4    |     |      | sJ9  |      |      |      |      |         |     |      |      |     |   |    |    |     |     |     |
| J5    |     |      | sJ10 |      |      |      |      |         |     |      |      |     |   |    |    |     |     |     |
| J6    |     |      |      |      |      |      |      |         |     | sJ11 |      |     |   |    |    |     |     |     |
| J7    | r1  | r1   |      |      |      |      |      |         | r1  |      |      | r1  |   |    |    |     |     |     |
| J8    |     |      | sJ12 |      |      |      |      |         |     |      |      |     |   |    |    |     |     |     |
| J9    |     | sJ15 |      |      |      |      |      | sJ16    | sJ6 |      |      |     |   |    |    | J13 | J14 | J17 |
| J10   |     | sJ15 |      |      |      |      |      | sJ16    | sJ6 |      |      |     |   |    |    | J18 | J14 | J17 |
| J11   |     |      |      |      |      |      |      | sJ19    |     |      |      |     |   |    |    |     |     |     |
| J12   |     | sJ15 |      |      |      |      |      | sJ16    | sJ6 |      |      |     |   |    |    | J20 | J14 | J17 |
| J13   |     |      |      | sJ21 |      |      |      |         |     |      |      |     |   |    |    |     |     |     |
| J14   |     |      |      | r6   | sJ22 | sJ23 | sJ24 |         |     |      |      |     |   |    |    |     |     |     |
| J15   |     |      |      | r10  | r10  | r10  | r10  |         |     |      |      |     |   |    |    |     |     |     |
| J16   |     |      |      | r11  | r11  | r11  | r11  |         |     |      |      |     |   |    |    |     |     |     |
| J17   |     |      |      | r12  | r12  | r12  | r12  |         |     |      |      |     |   |    |    |     |     |     |
| J18   |     |      |      | sJ25 |      |      |      |         |     |      |      |     |   |    |    |     |     |     |
| J19   |     |      |      |      |      |      |      |         |     |      | sJ26 |     |   |    |    |     |     |     |
| J20   |     |      |      | sJ27 |      |      |      |         |     |      |      |     |   |    |    |     |     |     |
| J21   | r4  | r4   |      |      |      |      |      |         | r4  |      |      | r4  |   |    |    |     |     |     |
| J22   |     | sJ15 |      |      |      |      |      | sJ16    | sJ6 |      |      |     |   |    |    |     | J28 | J17 |
| J23   |     | sJ15 |      |      |      |      |      | sJ16    | sJ6 |      |      |     |   |    |    |     | J29 | J17 |
| J24   |     | sJ15 |      |      |      |      |      | sJ16    | sJ6 |      |      |     |   |    |    |     | J30 | J17 |
| J25   | r5  | r5   |      |      |      |      |      |         | r5  |      |      | r5  |   |    |    |     |     |     |
| J26   |     |      |      |      |      |      |      |         |     | sJ31 |      |     |   |    |    |     |     |     |
| J27   | r3  | r3   |      |      |      |      |      |         | r3  |      |      | r3  |   |    |    |     |     |     |
| J28   |     |      |      | r7   |      |      |      |         |     |      |      |     |   |    |    |     |     |     |
| J29   |     |      |      | r8   |      |      |      |         |     |      |      |     |   |    |    |     |     |     |
| J30   |     |      |      | r9   |      |      |      |         |     |      |      |     |   |    |    |     |     |     |
| J31   |     |      |      |      |      |      |      | sJ32    |     |      |      |     |   |    |    |     |     |     |
| J32   |     |      |      |      |      |      |      |         |     |      | sJ33 |     |   |    |    |     |     |     |
| J33   |     |      | r13  | r13  | r13  | r13  | r13  |         |     |      |      |     |   |    |    |     |     |     |


no shift/reduce conflicts

no reduce/reduce conflicts

### LALR Automaton

Re-draw the LR(1) automaton after merging states

Present in directory 



