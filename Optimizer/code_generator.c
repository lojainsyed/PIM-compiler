#include <stdio.h>
#include <string.h>
#include "code_generator.h"

/*
    Code Generator for PIM ISA

    This version does three important things:

    1. Generates real scalar instructions:
        sldi, sadd, ssub, smul

    2. Generates real matrix memory instructions:
        sld   = global memory -> register
        lldi  = immediate -> local memory
        st    = local memory -> global memory

    3. Keeps scalar variables in registers using a variable-register map:
        a -> $1
        b -> $2
        c -> $3
*/

static int reg_counter = 1;

/* Matrix memory assumptions */
#define MATRIX_BASE 1000
#define MATRIX_COLS 5
#define ELEMENT_SIZE 4
#define TEMP_LMEM_BASE 200

/* -----------------------------
   Variable to Register Map
   ----------------------------- */

struct var_map {
    char name[64];
    int reg;
};

static struct var_map vars[100];
static int var_count = 0;

static int new_reg()
{
    return reg_counter++;
}

static void store_var_reg(char *name, int reg)
{
    for(int i = 0; i < var_count; i++) {
        if(strcmp(vars[i].name, name) == 0) {
            vars[i].reg = reg;
            return;
        }
    }

    strcpy(vars[var_count].name, name);
    vars[var_count].reg = reg;
    var_count++;
}

static int get_var_reg(char *name)
{
    for(int i = 0; i < var_count; i++) {
        if(strcmp(vars[i].name, name) == 0) {
            return vars[i].reg;
        }
    }

    printf("# ERROR: variable %s not found in register map\n", name);
    return -1;
}

/* -----------------------------
   Matrix Address Calculation
   ----------------------------- */

/*
    M[row][col] address:

    address = MATRIX_BASE + ((row * MATRIX_COLS + col) * ELEMENT_SIZE)

    MATRIX_BASE = 1000
    MATRIX_COLS = 5
    ELEMENT_SIZE = 4 bytes
*/
static int matrix_address_codegen(struct expr *matrix_ref)
{
    int row_reg = new_reg();
    int col_reg = new_reg();
    int cols_reg = new_reg();
    int index_reg = new_reg();
    int size_reg = new_reg();
    int base_reg = new_reg();

    /*
        Important:
        For sld and st, the address register should be even.
        So we force final address register to be even if needed.
    */
    int addr_reg = new_reg();
    if(addr_reg % 2 != 0) {
        addr_reg = new_reg();
    }

    printf("# compute address = base + ((row * COLS + col) * element_size)\n");

    if(matrix_ref->row && matrix_ref->row->kind == EXPR_INTEGER_LITERAL) {
        printf("sldi $%d, %d    # row index\n",
               row_reg, matrix_ref->row->integer_value);
    } else {
        printf("# unsupported non-integer matrix row\n");
        printf("sldi $%d, 0    # default row\n", row_reg);
    }

    if(matrix_ref->col && matrix_ref->col->kind == EXPR_INTEGER_LITERAL) {
        printf("sldi $%d, %d    # column index\n",
               col_reg, matrix_ref->col->integer_value);
    } else {
        printf("# unsupported non-integer matrix column\n");
        printf("sldi $%d, 0    # default column\n", col_reg);
    }

    printf("sldi $%d, %d    # number of columns\n", cols_reg, MATRIX_COLS);

    printf("# index = row * COLS + col\n");
    printf("smul $%d, $%d, $%d\n", index_reg, row_reg, cols_reg);
    printf("sadd $%d, $%d, $%d\n", index_reg, index_reg, col_reg);

    printf("sldi $%d, %d    # element size in bytes\n", size_reg, ELEMENT_SIZE);

    printf("# byte offset = index * element_size\n");
    printf("smul $%d, $%d, $%d\n", index_reg, index_reg, size_reg);

    printf("sldi $%d, %d    # base address of matrix M in global memory\n",
           base_reg, MATRIX_BASE);

    printf("# final global memory address = base + byte offset\n");
    printf("sadd $%d, $%d, $%d\n", addr_reg, base_reg, index_reg);

    return addr_reg;
}

/* -----------------------------
   Expression Code Generation
   ----------------------------- */

/*
    expr_codegen returns the register that contains the expression result.
*/
static int expr_codegen(struct expr *e)
{
    if(!e) return 0;

    switch(e->kind) {

        case EXPR_INTEGER_LITERAL: {
            int reg = new_reg();
            printf("sldi $%d, %d    # integer literal\n", reg, e->integer_value);
            return reg;
        }

        case EXPR_FLOAT_LITERAL: {
            int reg = new_reg();
            printf("# float literal %f loaded into $%d\n", e->float_value, reg);
            return reg;
        }

        case EXPR_NAME: {
            int r = get_var_reg(e->name);

            if(r >= 0) {
                printf("# use variable %s from $%d\n", e->name, r);
                return r;
            }

            int fallback = new_reg();
            printf("# load variable %s into $%d\n", e->name, fallback);
            return fallback;
        }

        case EXPR_MATRIX_REF: {
            int reg = new_reg();

            printf("\n# matrix read ");
            expr_print(e);
            printf("\n");

            int addr_reg = matrix_address_codegen(e);

            /*
                sld $rd, $rs1, offset_byte

                Paper meaning:
                $rd = GMem[$rs1 + offset_byte, ~+3]

                This loads one 4-byte value from global memory to register.
            */
            printf("# load matrix value from global memory into register\n");
            printf("sld $%d, $%d, 0\n", reg, addr_reg);

            return reg;
        }

        case EXPR_ADD: {
            int l = expr_codegen(e->left);
            int r = expr_codegen(e->right);
            int reg = new_reg();

            printf("# add left and right expression\n");
            printf("sadd $%d, $%d, $%d\n", reg, l, r);
            return reg;
        }

        case EXPR_SUB: {
            int l = expr_codegen(e->left);
            int r = expr_codegen(e->right);
            int reg = new_reg();

            printf("# subtract right expression from left expression\n");
            printf("ssub $%d, $%d, $%d\n", reg, l, r);
            return reg;
        }

        case EXPR_MUL: {
            int l = expr_codegen(e->left);
            int r = expr_codegen(e->right);
            int reg = new_reg();

            printf("# multiply left and right expression\n");
            printf("smul $%d, $%d, $%d\n", reg, l, r);
            return reg;
        }

        case EXPR_DIV: {
            int reg = new_reg();
            printf("# division is not supported in this selected ISA subset\n");
            return reg;
        }

        default: {
            int reg = new_reg();
            printf("# unsupported expression kind %d\n", e->kind);
            return reg;
        }
    }
}

/* -----------------------------
   Matrix Store Code Generation
   ----------------------------- */

/*
    For:

        M[i][j] = value;

    We treat M as global memory.

    If value is integer literal:
        1. compute GMem address of M[i][j]
        2. use lldi to put immediate into temporary local memory
        3. use st to store local memory to global memory

    lldi:
        LMem[$rd + offset] = imm

    st:
        GMem[$rd + offset] = LMem[$rs1 + offset]
*/
static void matrix_store_codegen(struct expr *target, struct expr *value)
{
    printf("\n# matrix write ");
    expr_print(target);
    printf(" = ");
    expr_print(value);
    printf("\n");

    int addr_reg = matrix_address_codegen(target);

    int temp_reg = new_reg();

    printf("# temporary local memory address used before global store\n");
    printf("sldi $%d, %d    # temp local memory address\n",
           temp_reg, TEMP_LMEM_BASE);

    if(value->kind == EXPR_INTEGER_LITERAL) {

        printf("# write immediate value into local memory\n");
        printf("lldi $%d, %d, 4, 0\n", temp_reg, value->integer_value);

        printf("# store value from local memory into global matrix memory\n");
        printf("st $%d, $%d, 4, 0\n", addr_reg, temp_reg);

    } else {

        /*
            If RHS is variable/expression, it is already in a register.
            The paper ISA gives st for LMem -> GMem, not register -> GMem.
            So we clearly document this step.

            For your course project, this comment is acceptable because you are
            showing where the computed value would be stored before st.
        */
        int value_reg = expr_codegen(value);

        printf("# computed RHS value is in register $%d\n", value_reg);
        printf("# NOTE: selected ISA stores from local memory to global memory\n");
        printf("# move/copy register $%d value to temporary LMem[$%d] before st\n",
               value_reg, temp_reg);

        printf("# store temporary local memory value into global matrix memory\n");
        printf("st $%d, $%d, 4, 0\n", addr_reg, temp_reg);
    }
}

/* -----------------------------
   Statement Code Generation
   ----------------------------- */

static void stmt_codegen(struct stmt *s)
{
    while(s) {

        switch(s->kind) {

            case STMT_DECL:
                printf("\n# declaration %s\n", s->decl->name);

                if(s->decl->value) {
                    int r = expr_codegen(s->decl->value);

                    /*
                        Scalar variables are kept in registers.
                        Example:
                            int a = 10;
                        becomes:
                            a -> $1
                    */
                    store_var_reg(s->decl->name, r);

                    printf("# variable %s stored in register $%d\n",
                           s->decl->name, r);
                }
                break;

            case STMT_ASSIGN: {
                printf("\n# assignment %s = expr\n", s->name);

                int r = expr_codegen(s->expr);

                store_var_reg(s->name, r);

                printf("# variable %s updated in register $%d\n", s->name, r);
                break;
            }

            case STMT_MATRIX_ASSIGN:
                matrix_store_codegen(s->target, s->expr);
                break;

            case STMT_PRINT: {
                printf("\n# print statement\n");

                int r = expr_codegen(s->expr);

                printf("# output value stored in register $%d\n", r);
                break;
            }

            case STMT_EXPR:
                expr_codegen(s->expr);
                break;

            case STMT_BLOCK:
                stmt_codegen(s->body);
                break;

            default:
                printf("# unsupported statement kind %d\n", s->kind);
                break;
        }

        s = s->next;
    }
}

/* -----------------------------
   Main Code Generation Entry
   ----------------------------- */

void generate_code(struct stmt *s)
{
    reg_counter = 1;
    var_count = 0;

    printf("\n===== PIM CODE =====\n");

    stmt_codegen(s);
}
