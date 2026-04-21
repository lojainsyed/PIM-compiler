#include <stdio.h>
#include "semantic.h"
#include "symbol_table.h"

static int semantic_error_count = 0;

static void semantic_error(const char *msg, const char *name)
{
    semantic_error_count++;
    if(name) {
        printf("semantic error: %s '%s'\n", msg, name);
    } else {
        printf("semantic error: %s\n", msg);
    }
}

/********************************
    NAME RESOLUTION
********************************/

static void expr_resolve(struct expr *e)
{
    if(!e) return;

    expr_resolve(e->left);
    expr_resolve(e->right);

    if(e->kind == EXPR_NAME) {
        if(!scope_lookup(e->name)) {
            semantic_error("undeclared identifier", e->name);
        }
    }
}

static void decl_resolve(struct decl *d)
{
    while(d) {
        symbol_t kind;

        if(scope_head && scope_head->next) {
            kind = SYMBOL_LOCAL;
        } else {
            kind = SYMBOL_GLOBAL;
        }

        if(!scope_bind_symbol(kind, d->type, d->name)) {
            semantic_error("redeclaration of identifier", d->name);
        }

        if(d->value) {
            expr_resolve(d->value);
        }

        d = d->next;
    }
}

static void stmt_resolve(struct stmt *s)
{
    while(s) {
        switch(s->kind) {
            case STMT_DECL:
                decl_resolve(s->decl);
                break;

            case STMT_EXPR:
                expr_resolve(s->expr);
                break;

            case STMT_ASSIGN:
                if(!scope_lookup(s->name)) {
                    semantic_error("assignment to undeclared identifier", s->name);
                }
                expr_resolve(s->expr);
                break;

            case STMT_MATRIX_ASSIGN:
                expr_resolve(s->target);
                expr_resolve(s->expr);
                break;

            case STMT_PRINT:
            case STMT_RETURN:
                expr_resolve(s->expr);
                break;

            case STMT_IF_ELSE:
                expr_resolve(s->expr);

                scope_enter();
                stmt_resolve(s->body);
                scope_exit();

                if(s->else_body) {
                    scope_enter();
                    stmt_resolve(s->else_body);
                    scope_exit();
                }
                break;

            case STMT_FOR:
                scope_enter();
                expr_resolve(s->init_expr);
                expr_resolve(s->expr);
                expr_resolve(s->next_expr);
                stmt_resolve(s->body);
                scope_exit();
                break;

            case STMT_BLOCK:
                scope_enter();
                stmt_resolve(s->body);
                scope_exit();
                break;
        }

        s = s->next;
    }
}

/********************************
       TYPE CHECKING
********************************/

static int type_equals(struct type *a, struct type *b)
{
    if(!a || !b) return 0;
    if(a->kind != b->kind) return 0;

    if(a->kind == TYPE_ARRAY || a->kind == TYPE_FUNCTION) {
        return type_equals(a->subtype, b->subtype);
    }

    return 1;
}

static struct type * expr_typecheck(struct expr *e)
{
    if(!e) return 0;

    switch(e->kind) {
        case EXPR_INTEGER_LITERAL:
            return type_create(TYPE_INTEGER, 0, 0);

        case EXPR_STRING_LITERAL:
            return type_create(TYPE_STRING, 0, 0);

        case EXPR_MATRIX_REF:
            return type_create(TYPE_INTEGER, 0, 0);

        case EXPR_NAME: {
            struct symbol *s = scope_lookup(e->name);
            if(!s) {
                semantic_error("undeclared identifier", e->name);
                return type_create(TYPE_VOID, 0, 0);
            }
            return s->type;
        }

        case EXPR_ADD:
        case EXPR_SUB:
        case EXPR_MUL:
        case EXPR_DIV: {
            struct type *left_type = expr_typecheck(e->left);
            struct type *right_type = expr_typecheck(e->right);

            if(!left_type || !right_type) {
                semantic_error("invalid expression type", 0);
                return type_create(TYPE_VOID, 0, 0);
            }

            if(left_type->kind != TYPE_INTEGER || right_type->kind != TYPE_INTEGER) {
                semantic_error("arithmetic operands must be integers", 0);
                return type_create(TYPE_VOID, 0, 0);
            }

            return type_create(TYPE_INTEGER, 0, 0);
        }
    }

    return type_create(TYPE_VOID, 0, 0);
}

static void decl_typecheck(struct decl *d)
{
    while(d) {
        if(d->value) {
            struct type *value_type = expr_typecheck(d->value);

            if(value_type && !type_equals(d->type, value_type)) {
                semantic_error("type mismatch in declaration", d->name);
            }
        }

        d = d->next;
    }
}

static void stmt_typecheck(struct stmt *s)
{
    while(s) {
        switch(s->kind) {
            case STMT_DECL:
                decl_typecheck(s->decl);
                break;

            case STMT_EXPR:
                expr_typecheck(s->expr);
                break;

            case STMT_ASSIGN: {
                struct symbol *sym = scope_lookup(s->name);
                struct type *expr_type = expr_typecheck(s->expr);

                if(sym && expr_type && !type_equals(sym->type, expr_type)) {
                    semantic_error("type mismatch in assignment", s->name);
                }
                break;
            }

            case STMT_MATRIX_ASSIGN: {
                struct type *expr_type = expr_typecheck(s->expr);
                if(expr_type && expr_type->kind != TYPE_INTEGER) {
                    semantic_error("matrix assignment requires integer value", 0);
                }
                break;
            }

            case STMT_PRINT:
            case STMT_RETURN:
                expr_typecheck(s->expr);
                break;

            case STMT_IF_ELSE:
                expr_typecheck(s->expr);

                scope_enter();
                stmt_typecheck(s->body);
                scope_exit();

                if(s->else_body) {
                    scope_enter();
                    stmt_typecheck(s->else_body);
                    scope_exit();
                }
                break;

            case STMT_FOR:
                scope_enter();
                expr_typecheck(s->init_expr);
                expr_typecheck(s->expr);
                expr_typecheck(s->next_expr);
                stmt_typecheck(s->body);
                scope_exit();
                break;

            case STMT_BLOCK:
                scope_enter();
                stmt_typecheck(s->body);
                scope_exit();
                break;
        }

        s = s->next;
    }
}

/********************************
       MAIN SEMANTIC PASS
********************************/

void semantic_analyze(struct stmt *root)
{
    semantic_error_count = 0;

    scope_enter();          /* global scope */
    stmt_resolve(root);     /* build symbol table + name resolution */
    stmt_typecheck(root);   /* type checking */

    printf("\nSemantic analysis finished with %d error(s).\n", semantic_error_count);
}

void semantic_print_symbol_table(void)
{
    printf("\nSymbol Table:\n");
    printf("=============\n");
    scope_print_current();
}
