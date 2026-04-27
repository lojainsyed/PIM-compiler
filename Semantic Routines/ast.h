#ifndef AST_H
#define AST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/********************************
      TYPES
********************************/

typedef enum {
    TYPE_BOOLEAN,
    TYPE_CHARACTER,
    TYPE_INTEGER,
    TYPE_FLOAT,
    TYPE_VOID,
    TYPE_STRING,
    TYPE_ARRAY,
    TYPE_FUNCTION
} type_t;

struct param_list;

struct type {
    type_t kind;
    struct type *subtype;
    struct param_list *params;
};

struct param_list {
    char *name;
    struct type *type;
    struct param_list *next;
};

static inline struct type * type_create(type_t kind, struct type *subtype, struct param_list *params)
{
    struct type *t = malloc(sizeof(*t));
    t->kind = kind;
    t->subtype = subtype;
    t->params = params;
    return t;
}

static inline struct param_list * param_list_create(char *name,
                                                    struct type *type,
                                                    struct param_list *next)
{
    struct param_list *p = malloc(sizeof(*p));
    p->name = name;
    p->type = type;
    p->next = next;
    return p;
}

static inline void type_print(struct type *t)
{
    if(!t) return;

    switch(t->kind) {
        case TYPE_BOOLEAN:
            printf("bool");
            break;

        case TYPE_CHARACTER:
            printf("char");
            break;

        case TYPE_INTEGER:
            printf("int");
            break;

        case TYPE_FLOAT:
            printf("float");
            break;

        case TYPE_VOID:
            printf("void");
            break;

        case TYPE_STRING:
            printf("string");
            break;

        case TYPE_ARRAY:
            printf("array of ");
            type_print(t->subtype);
            break;

        case TYPE_FUNCTION:
            printf("function ");
            type_print(t->subtype);

            if(t->params) {
                printf(" (");
                struct param_list *p = t->params;
                while(p) {
                    printf("%s: ", p->name);
                    type_print(p->type);

                    if(p->next) {
                        printf(", ");
                    }

                    p = p->next;
                }
                printf(")");
            }
            break;
    }
}

/********************************
      EXPRESSIONS
********************************/

typedef enum {
    EXPR_ADD,
    EXPR_SUB,
    EXPR_MUL,
    EXPR_DIV,
    EXPR_NAME,
    EXPR_INTEGER_LITERAL,
    EXPR_FLOAT_LITERAL,
    EXPR_STRING_LITERAL,
    EXPR_MATRIX_REF
} expr_t;

struct expr {
    expr_t kind;
    struct expr *left;
    struct expr *right;

    const char *name;
    int integer_value;
    double float_value;
    const char *string_literal;

    struct expr *row;
    struct expr *col;
};

static inline struct expr * expr_create(expr_t kind, struct expr *left, struct expr *right)
{
    struct expr *e = malloc(sizeof(*e));
    e->kind = kind;
    e->left = left;
    e->right = right;
    e->name = NULL;
    e->integer_value = 0;
    e->float_value = 0.0;
    e->string_literal = NULL;
    e->row = NULL;
    e->col = NULL;
    return e;
}

static inline struct expr * expr_create_name(const char *n)
{
    struct expr *e = expr_create(EXPR_NAME, 0, 0);
    e->name = n;
    return e;
}

static inline struct expr * expr_create_integer_literal(int i)
{
    struct expr *e = expr_create(EXPR_INTEGER_LITERAL, 0, 0);
    e->integer_value = i;
    return e;
}

static inline struct expr * expr_create_float_literal(double f)
{
    struct expr *e = expr_create(EXPR_FLOAT_LITERAL, 0, 0);
    e->float_value = f;
    return e;
}

static inline struct expr * expr_create_matrix_ref(struct expr *row, struct expr *col)
{
    struct expr *e = expr_create(EXPR_MATRIX_REF, 0, 0);
    e->row = row;
    e->col = col;
    return e;
}

static inline void expr_print(struct expr *e)
{
    if(!e) return;

    switch(e->kind) {
        case EXPR_INTEGER_LITERAL:
            printf("%d", e->integer_value);
            break;

        case EXPR_FLOAT_LITERAL:
            printf("%f", e->float_value);
            break;

        case EXPR_NAME:
            printf("%s", e->name);
            break;

        case EXPR_STRING_LITERAL:
            printf("%s", e->string_literal);
            break;

        case EXPR_MATRIX_REF:
            printf("M[");
            expr_print(e->row);
            printf("][");
            expr_print(e->col);
            printf("]");
            break;

        case EXPR_ADD:
            printf("(");
            expr_print(e->left);
            printf(" + ");
            expr_print(e->right);
            printf(")");
            break;

        case EXPR_SUB:
            printf("(");
            expr_print(e->left);
            printf(" - ");
            expr_print(e->right);
            printf(")");
            break;

        case EXPR_MUL:
            printf("(");
            expr_print(e->left);
            printf(" * ");
            expr_print(e->right);
            printf(")");
            break;

        case EXPR_DIV:
            printf("(");
            expr_print(e->left);
            printf(" / ");
            expr_print(e->right);
            printf(")");
            break;
    }
}

/********************************
      DECLARATIONS
********************************/

struct stmt;

struct decl {
    char *name;
    struct type *type;
    struct expr *value;
    struct stmt *code;
    struct decl *next;
};

static inline struct decl * decl_create(char *name,
                                        struct type *type,
                                        struct expr *value,
                                        struct stmt *code,
                                        struct decl *next)
{
    struct decl *d = malloc(sizeof(*d));
    d->name = name;
    d->type = type;
    d->value = value;
    d->code = code;
    d->next = next;
    return d;
}

/********************************
      STATEMENTS
********************************/

typedef enum {
    STMT_DECL,
    STMT_EXPR,
    STMT_ASSIGN,
    STMT_MATRIX_ASSIGN,
    STMT_IF_ELSE,
    STMT_FOR,
    STMT_PRINT,
    STMT_RETURN,
    STMT_BLOCK
} stmt_t;

struct stmt {
    stmt_t kind;

    struct decl *decl;

    char *name;
    struct expr *target;

    struct expr *init_expr;
    struct expr *expr;
    struct expr *next_expr;

    struct stmt *body;
    struct stmt *else_body;
    struct stmt *next;
};

static inline struct stmt * stmt_create(stmt_t kind,
                                        struct decl *decl,
                                        struct expr *init_expr,
                                        struct expr *expr,
                                        struct expr *next_expr,
                                        struct stmt *body,
                                        struct stmt *else_body,
                                        struct stmt *next)
{
    struct stmt *s = malloc(sizeof(*s));
    s->kind = kind;
    s->decl = decl;
    s->name = NULL;
    s->target = NULL;
    s->init_expr = init_expr;
    s->expr = expr;
    s->next_expr = next_expr;
    s->body = body;
    s->else_body = else_body;
    s->next = next;
    return s;
}

static inline void stmt_print(struct stmt *s);

static inline void decl_print(struct decl *d)
{
    if(!d) return;

    printf("var [%s] : ", d->name);
    type_print(d->type);

    if(d->value) {
        printf(" = expr ");
        expr_print(d->value);
    }

    if(d->code) {
        printf(" code ");
        stmt_print(d->code);
    } else {
        printf(";\n");
    }

    decl_print(d->next);
}

static inline void stmt_print(struct stmt *s)
{
    if(!s) return;

    switch(s->kind) {
        case STMT_DECL:
            decl_print(s->decl);
            break;

        case STMT_EXPR:
            expr_print(s->expr);
            printf(";\n");
            break;

        case STMT_ASSIGN:
            printf("%s = ", s->name);
            expr_print(s->expr);
            printf(";\n");
            break;

        case STMT_MATRIX_ASSIGN:
            expr_print(s->target);
            printf(" = ");
            expr_print(s->expr);
            printf(";\n");
            break;

        case STMT_PRINT:
            printf("print ");
            expr_print(s->expr);
            printf(";\n");
            break;

        case STMT_RETURN:
            printf("return ");
            expr_print(s->expr);
            printf(";\n");
            break;

        case STMT_IF_ELSE:
            printf("if (");
            expr_print(s->expr);
            printf(")\n");
            stmt_print(s->body);
            if(s->else_body) {
                printf("else\n");
                stmt_print(s->else_body);
            }
            break;

        case STMT_FOR:
            printf("for (...)\n");
            stmt_print(s->body);
            break;

        case STMT_BLOCK:
            printf("{\n");
            stmt_print(s->body);
            printf("}\n");
            break;
    }

    stmt_print(s->next);
}

#endif
