#include <stdio.h>
#include "semantic.h"
#include "symbol_table.h"

static int semantic_error_count = 0;

/* Forward declarations */
static void stmt_resolve(struct stmt *s);
static void decl_resolve(struct decl *d);
static void expr_resolve(struct expr *e);

static struct type *expr_typecheck(struct expr *e);
static void stmt_typecheck(struct stmt *s);
static void decl_typecheck(struct decl *d);

static void semantic_error(const char *msg, const char *name)
{
    semantic_error_count++;

    if(name) {
        printf("semantic error: %s '%s'\n", msg, name);
    } else {
        printf("semantic error: %s\n", msg);
    }
}

static int is_numeric(struct type *t)
{
    return t && (t->kind == TYPE_INTEGER || t->kind == TYPE_FLOAT);
}

static int type_equals(struct type *a, struct type *b)
{
    if(!a || !b) return 0;
    return a->kind == b->kind;
}

static int assignment_compatible(struct type *target, struct type *value)
{
    if(!target || !value) return 0;

    if(type_equals(target, value)) return 1;

    /* int can be assigned to float */
    if(target->kind == TYPE_FLOAT && value->kind == TYPE_INTEGER) {
        return 1;
    }

    /* float cannot be assigned to int */
    return 0;
}

/********************************
    NAME RESOLUTION
********************************/

static void expr_resolve(struct expr *e)
{
    if(!e) return;

    expr_resolve(e->left);
    expr_resolve(e->right);
    expr_resolve(e->row);
    expr_resolve(e->col);

    if(e->kind == EXPR_NAME) {
        if(!scope_lookup(e->name)) {
            semantic_error("undeclared identifier", e->name);
        }
    }
}

static void bind_params(struct param_list *p)
{
    while(p) {
        if(!scope_bind_symbol(SYMBOL_PARAM, p->type, p->name)) {
            semantic_error("redeclaration of parameter", p->name);
        }
        p = p->next;
    }
}

static void decl_resolve(struct decl *d)
{
    while(d) {
        symbol_t kind = (scope_head && scope_head->next) ? SYMBOL_LOCAL : SYMBOL_GLOBAL;

        if(!scope_bind_symbol(kind, d->type, d->name)) {
            semantic_error("redeclaration of identifier", d->name);
        }

        if(d->value) {
            expr_resolve(d->value);
        }

        /*
           Function handling:
           Level 0: global scope
           Level 1: parameter scope
           Level 2: local/body scope

           Important:
           We resolve AND typecheck function body here while these scopes exist.
           We do NOT recreate these scopes again in decl_typecheck().
        */
        if(d->code) {
            scope_enter();          /* parameter scope */
            bind_params(d->type->params);

            scope_enter();          /* local/body scope */

            if(d->code->kind == STMT_BLOCK) {
                stmt_resolve(d->code->body);
                stmt_typecheck(d->code->body);
            } else {
                stmt_resolve(d->code);
                stmt_typecheck(d->code);
            }

            scope_exit();           /* exit local/body scope */
            scope_exit();           /* exit parameter scope */
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
                stmt_typecheck(s->body);
                scope_exit();

                if(s->else_body) {
                    scope_enter();
                    stmt_resolve(s->else_body);
                    stmt_typecheck(s->else_body);
                    scope_exit();
                }
                break;

            case STMT_FOR:
                scope_enter();
                expr_resolve(s->init_expr);
                expr_resolve(s->expr);
                expr_resolve(s->next_expr);
                stmt_resolve(s->body);
                stmt_typecheck(s->body);
                scope_exit();
                break;

            case STMT_BLOCK:
                scope_enter();
                stmt_resolve(s->body);
                stmt_typecheck(s->body);
                scope_exit();
                break;
        }

        s = s->next;
    }
}

/********************************
    TYPE CHECKING
********************************/

static struct type *expr_typecheck(struct expr *e)
{
    if(!e) {
        return type_create(TYPE_VOID, 0, 0);
    }

    switch(e->kind) {
        case EXPR_INTEGER_LITERAL:
            return type_create(TYPE_INTEGER, 0, 0);

        case EXPR_FLOAT_LITERAL:
            return type_create(TYPE_FLOAT, 0, 0);

        case EXPR_STRING_LITERAL:
            return type_create(TYPE_STRING, 0, 0);

        case EXPR_MATRIX_REF:
            expr_typecheck(e->row);
            expr_typecheck(e->col);
            return type_create(TYPE_INTEGER, 0, 0);

        case EXPR_NAME: {
            struct symbol *sym = scope_lookup(e->name);

            if(!sym) {
                return type_create(TYPE_VOID, 0, 0);
            }

            return sym->type;
        }

        case EXPR_ADD:
        case EXPR_SUB:
        case EXPR_MUL:
        case EXPR_DIV: {
            struct type *lt = expr_typecheck(e->left);
            struct type *rt = expr_typecheck(e->right);

            if(!is_numeric(lt) || !is_numeric(rt)) {
                semantic_error("arithmetic operands must be numeric", 0);
                return type_create(TYPE_VOID, 0, 0);
            }

            if(lt->kind == TYPE_FLOAT || rt->kind == TYPE_FLOAT) {
                return type_create(TYPE_FLOAT, 0, 0);
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
            struct type *vt = expr_typecheck(d->value);

            if(!assignment_compatible(d->type, vt)) {
                semantic_error("type mismatch in declaration", d->name);
            }
        }

        /*
           Do NOT typecheck d->code here.
           Function bodies are typechecked inside decl_resolve()
           while parameter/local scopes are active.
        */

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
                struct type *et = expr_typecheck(s->expr);

                if(sym && !assignment_compatible(sym->type, et)) {
                    semantic_error("type mismatch in assignment", s->name);
                }
                break;
            }

            case STMT_MATRIX_ASSIGN:
                expr_typecheck(s->target);
                expr_typecheck(s->expr);
                break;

            case STMT_PRINT:
            case STMT_RETURN:
                expr_typecheck(s->expr);
                break;

            case STMT_IF_ELSE:
                expr_typecheck(s->expr);
                break;

            case STMT_FOR:
                expr_typecheck(s->init_expr);
                expr_typecheck(s->expr);
                expr_typecheck(s->next_expr);
                break;

            case STMT_BLOCK:
                break;
        }

        s = s->next;
    }
}

/********************************
    MAIN SEMANTIC ANALYSIS
********************************/

void semantic_analyze(struct stmt *root)
{
    semantic_error_count = 0;

    global_which = 0;
    local_which = 0;
    param_which = 0;
    scope_head = NULL;
    scope_count = 0;

    scope_enter();          /* global scope */

    stmt_resolve(root);
    stmt_typecheck(root);   /* top-level typecheck only */

    printf("\nSemantic analysis finished with %d error(s).\n", semantic_error_count);
}

void semantic_print_symbol_table(void)
{
    scope_print_all();
}
