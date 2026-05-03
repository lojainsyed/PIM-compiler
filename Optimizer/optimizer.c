#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast.h"
#include "optimizer.h"

/******** COUNT OPERATIONS ********/
int count_expr_ops(struct expr *e)
{
    if(!e) return 0;

    int count = 0;

    if(e->kind == EXPR_ADD ||
       e->kind == EXPR_SUB ||
       e->kind == EXPR_MUL ||
       e->kind == EXPR_DIV) {
        count = 1;
    }

    return count +
           count_expr_ops(e->left) +
           count_expr_ops(e->right);
}

int count_stmt_ops(struct stmt *s)
{
    if(!s) return 0;

    int count = 0;

    if(s->expr) {
        count += count_expr_ops(s->expr);
    }

    if(s->init_expr) {
        count += count_expr_ops(s->init_expr);
    }

    if(s->next_expr) {
        count += count_expr_ops(s->next_expr);
    }

    if(s->decl && s->decl->value) {
        count += count_expr_ops(s->decl->value);
    }

    return count + count_stmt_ops(s->next);
}

int count_statements(struct stmt *s)
{
    if(!s) return 0;
    return 1 + count_statements(s->next);
}

/******** CONSTANT FOLDING ********/
struct expr* constant_fold(struct expr *e)
{
    if(!e) return NULL;

    e->left = constant_fold(e->left);
    e->right = constant_fold(e->right);

    if(e->kind == EXPR_ADD ||
       e->kind == EXPR_SUB ||
       e->kind == EXPR_MUL ||
       e->kind == EXPR_DIV) {

        if(e->left && e->right &&
           e->left->kind == EXPR_INTEGER_LITERAL &&
           e->right->kind == EXPR_INTEGER_LITERAL) {

            int l = e->left->integer_value;
            int r = e->right->integer_value;
            int result = 0;

            if(e->kind == EXPR_ADD) result = l + r;
            if(e->kind == EXPR_SUB) result = l - r;
            if(e->kind == EXPR_MUL) result = l * r;

            if(e->kind == EXPR_DIV) {
                if(r == 0) {
                    printf("Warning: division by zero detected. Skipping constant folding.\n");
                    return e;
                }
                result = l / r;
            }

            return expr_create_integer_literal(result);
        }
    }

    return e;
}

/******** STRENGTH REDUCTION ********/
struct expr* strength_reduce(struct expr *e)
{
    if(!e) return NULL;

    e->left = strength_reduce(e->left);
    e->right = strength_reduce(e->right);

    /*
       Strength reduction:
       x * 2 -> x + x
    */
    if(e->kind == EXPR_MUL &&
       e->right &&
       e->right->kind == EXPR_INTEGER_LITERAL &&
       e->right->integer_value == 2) {

        return expr_create(EXPR_ADD, e->left, e->left);
    }

    /*
       2 * x -> x + x
    */
    if(e->kind == EXPR_MUL &&
       e->left &&
       e->left->kind == EXPR_INTEGER_LITERAL &&
       e->left->integer_value == 2) {

        return expr_create(EXPR_ADD, e->right, e->right);
    }

    return e;
}

/******** ALGEBRAIC SIMPLIFICATION ********/
struct expr* algebraic_simplify(struct expr *e)
{
    if(!e) return NULL;

    e->left = algebraic_simplify(e->left);
    e->right = algebraic_simplify(e->right);

    /* x + 0 -> x */
    if(e->kind == EXPR_ADD &&
       e->right &&
       e->right->kind == EXPR_INTEGER_LITERAL &&
       e->right->integer_value == 0) {
        return e->left;
    }

    /* 0 + x -> x */
    if(e->kind == EXPR_ADD &&
       e->left &&
       e->left->kind == EXPR_INTEGER_LITERAL &&
       e->left->integer_value == 0) {
        return e->right;
    }

    /* x - 0 -> x */
    if(e->kind == EXPR_SUB &&
       e->right &&
       e->right->kind == EXPR_INTEGER_LITERAL &&
       e->right->integer_value == 0) {
        return e->left;
    }

    /* x * 1 -> x */
    if(e->kind == EXPR_MUL &&
       e->right &&
       e->right->kind == EXPR_INTEGER_LITERAL &&
       e->right->integer_value == 1) {
        return e->left;
    }

    /* 1 * x -> x */
    if(e->kind == EXPR_MUL &&
       e->left &&
       e->left->kind == EXPR_INTEGER_LITERAL &&
       e->left->integer_value == 1) {
        return e->right;
    }

    /* x * 0 -> 0 */
    if(e->kind == EXPR_MUL &&
       e->right &&
       e->right->kind == EXPR_INTEGER_LITERAL &&
       e->right->integer_value == 0) {
        return expr_create_integer_literal(0);
    }

    /* 0 * x -> 0 */
    if(e->kind == EXPR_MUL &&
       e->left &&
       e->left->kind == EXPR_INTEGER_LITERAL &&
       e->left->integer_value == 0) {
        return expr_create_integer_literal(0);
    }

    /* x / 1 -> x */
    if(e->kind == EXPR_DIV &&
       e->right &&
       e->right->kind == EXPR_INTEGER_LITERAL &&
       e->right->integer_value == 1) {
        return e->left;
    }

    return e;
}

/******** EXPRESSION USE CHECK ********/
int expr_uses_name(struct expr *e, char *name)
{
    if(!e || !name) return 0;

    if(e->kind == EXPR_NAME && e->name) {
        if(strcmp(e->name, name) == 0) {
            return 1;
        }
    }

    return expr_uses_name(e->left, name) ||
           expr_uses_name(e->right, name);
}

/******** STATEMENT USE CHECK ********/
int is_used(char *name, struct stmt *s)
{
    if(!s) return 0;

    if(s->expr && expr_uses_name(s->expr, name)) {
        return 1;
    }

    if(s->init_expr && expr_uses_name(s->init_expr, name)) {
        return 1;
    }

    if(s->next_expr && expr_uses_name(s->next_expr, name)) {
        return 1;
    }

    if(s->decl && s->decl->value &&
       expr_uses_name(s->decl->value, name)) {
        return 1;
    }

    return is_used(name, s->next);
}

/******** DEAD CODE ELIMINATION ********/
struct stmt* dead_code_eliminate(struct stmt *s)
{
    if(!s) return NULL;

    s->next = dead_code_eliminate(s->next);

    if(s->kind == STMT_DECL && s->decl && s->decl->name) {
        if(!is_used(s->decl->name, s->next)) {
            return s->next;
        }
    }

    return s;
}

/******** OPTIMIZE ONE EXPRESSION ********/
struct expr *optimize_expr(struct expr *e)
{
    e = constant_fold(e);
    e = strength_reduce(e);
    e = algebraic_simplify(e);

    /*
       Run again because one optimization may create
       another optimization opportunity.
    */
    e = constant_fold(e);
    e = algebraic_simplify(e);

    return e;
}

/******** MAIN OPTIMIZER ********/
struct stmt *optimize_program(struct stmt *s)
{
    struct stmt *curr = s;

    while(curr) {

        if(curr->expr) {
            curr->expr = optimize_expr(curr->expr);
        }

        if(curr->init_expr) {
            curr->init_expr = optimize_expr(curr->init_expr);
        }

        if(curr->next_expr) {
            curr->next_expr = optimize_expr(curr->next_expr);
        }

        if(curr->kind == STMT_DECL && curr->decl && curr->decl->value) {
            curr->decl->value = optimize_expr(curr->decl->value);
        }

        curr = curr->next;
    }

    return dead_code_eliminate(s);
}