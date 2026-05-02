#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast_io.h"

/* =========================
   TYPE WRITE / READ
   ========================= */

static void type_write(struct type *t, FILE *f)
{
    if(!t) {
        fprintf(f, "TYPE_NULL\n");
        return;
    }

    fprintf(f, "TYPE %d\n", t->kind);

    /* Save subtype and params for future support */
    type_write(t->subtype, f);
}

static struct type *type_read(FILE *f)
{
    char buf[256];

    if(!fgets(buf, sizeof(buf), f)) return NULL;

    if(strncmp(buf, "TYPE_NULL", 9) == 0)
        return NULL;

    int kind;
    sscanf(buf, "TYPE %d", &kind);

    struct type *subtype = type_read(f);

    return type_create(kind, subtype, NULL);
}

/* =========================
   EXPRESSION WRITE / READ
   ========================= */

static void expr_write(struct expr *e, FILE *f)
{
    if(!e) {
        fprintf(f, "EXPR_NULL\n");
        return;
    }

    fprintf(f, "EXPR %d\n", e->kind);

    switch(e->kind) {
        case EXPR_NAME:
            fprintf(f, "%s\n", e->name);
            break;

        case EXPR_INTEGER_LITERAL:
            fprintf(f, "%d\n", e->integer_value);
            break;

        case EXPR_FLOAT_LITERAL:
            fprintf(f, "%lf\n", e->float_value);
            break;

        case EXPR_STRING_LITERAL:
            fprintf(f, "%s\n", e->string_literal);
            break;

        case EXPR_MATRIX_REF:
            expr_write(e->row, f);
            expr_write(e->col, f);
            break;

        default:
            break;
    }

    expr_write(e->left, f);
    expr_write(e->right, f);
}

static struct expr *expr_read(FILE *f)
{
    char buf[256];

    if(!fgets(buf, sizeof(buf), f)) return NULL;

    if(strncmp(buf, "EXPR_NULL", 9) == 0)
        return NULL;

    int kind;
    sscanf(buf, "EXPR %d", &kind);

    struct expr *e = expr_create(kind, NULL, NULL);

    switch(kind) {
        case EXPR_NAME:
            fgets(buf, sizeof(buf), f);
            buf[strcspn(buf, "\n")] = 0;
            e->name = strdup(buf);
            break;

        case EXPR_INTEGER_LITERAL:
            fgets(buf, sizeof(buf), f);
            e->integer_value = atoi(buf);
            break;

        case EXPR_FLOAT_LITERAL:
            fgets(buf, sizeof(buf), f);
            e->float_value = atof(buf);
            break;

        case EXPR_STRING_LITERAL:
            fgets(buf, sizeof(buf), f);
            buf[strcspn(buf, "\n")] = 0;
            e->string_literal = strdup(buf);
            break;

        case EXPR_MATRIX_REF:
            e->row = expr_read(f);
            e->col = expr_read(f);
            break;

        default:
            break;
    }

    e->left = expr_read(f);
    e->right = expr_read(f);

    return e;
}

/* =========================
   DECL WRITE / READ
   ========================= */

static void decl_write(struct decl *d, FILE *f)
{
    if(!d) {
        fprintf(f, "DECL_NULL\n");
        return;
    }

    fprintf(f, "DECL %s\n", d->name);

    type_write(d->type, f);
    expr_write(d->value, f);

    /* code and next saved for completeness */
    fprintf(f, "DECL_CODE_NULL\n");

    decl_write(d->next, f);
}

static struct decl *decl_read(FILE *f)
{
    char buf[256];

    if(!fgets(buf, sizeof(buf), f)) return NULL;

    if(strncmp(buf, "DECL_NULL", 9) == 0)
        return NULL;

    char name[256];
    sscanf(buf, "DECL %255s", name);

    struct type *type = type_read(f);
    struct expr *value = expr_read(f);

    fgets(buf, sizeof(buf), f); /* DECL_CODE_NULL */

    struct decl *next = decl_read(f);

    return decl_create(strdup(name), type, value, NULL, next);
}

/* =========================
   STMT WRITE / READ
   ========================= */

static void stmt_write(struct stmt *s, FILE *f)
{
    if(!s) {
        fprintf(f, "STMT_NULL\n");
        return;
    }

    fprintf(f, "STMT %d\n", s->kind);

    switch(s->kind) {
        case STMT_DECL:
            decl_write(s->decl, f);
            break;

        case STMT_ASSIGN:
            fprintf(f, "%s\n", s->name);
            expr_write(s->expr, f);
            break;

        case STMT_MATRIX_ASSIGN:
            expr_write(s->target, f);
            expr_write(s->expr, f);
            break;

        case STMT_PRINT:
        case STMT_RETURN:
        case STMT_EXPR:
            expr_write(s->expr, f);
            break;

        case STMT_BLOCK:
            stmt_write(s->body, f);
            break;

        default:
            expr_write(s->init_expr, f);
            expr_write(s->expr, f);
            expr_write(s->next_expr, f);
            stmt_write(s->body, f);
            stmt_write(s->else_body, f);
            break;
    }

    stmt_write(s->next, f);
}

static struct stmt *stmt_read(FILE *f)
{
    char buf[256];

    if(!fgets(buf, sizeof(buf), f)) return NULL;

    if(strncmp(buf, "STMT_NULL", 9) == 0)
        return NULL;

    int kind;
    sscanf(buf, "STMT %d", &kind);

    struct stmt *s = stmt_create(kind, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

    switch(kind) {
        case STMT_DECL:
            s->decl = decl_read(f);
            break;

        case STMT_ASSIGN:
            fgets(buf, sizeof(buf), f);
            buf[strcspn(buf, "\n")] = 0;
            s->name = strdup(buf);
            s->expr = expr_read(f);
            break;

        case STMT_MATRIX_ASSIGN:
            s->target = expr_read(f);
            s->expr = expr_read(f);
            break;

        case STMT_PRINT:
        case STMT_RETURN:
        case STMT_EXPR:
            s->expr = expr_read(f);
            break;

        case STMT_BLOCK:
            s->body = stmt_read(f);
            break;

        default:
            s->init_expr = expr_read(f);
            s->expr = expr_read(f);
            s->next_expr = expr_read(f);
            s->body = stmt_read(f);
            s->else_body = stmt_read(f);
            break;
    }

    s->next = stmt_read(f);

    return s;
}

void ast_write(struct stmt *s, FILE *f)
{
    stmt_write(s, f);
}

struct stmt *ast_read(FILE *f)
{
    return stmt_read(f);
}
