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

struct stmt;

static void stmt_print(struct stmt *s);
static void stmt_print_postorder(struct stmt *s);
static void param_list_print(struct param_list *p);

static struct type * type_create(type_t kind, struct type *subtype, struct param_list *params)
{
    struct type *t = malloc(sizeof(*t));
    t->kind = kind;
    t->subtype = subtype;
    t->params = params;
    return t;
}

static struct param_list * param_list_create(char *name, struct type *type, struct param_list *next)
{
    struct param_list *p = malloc(sizeof(*p));
    p->name = name;
    p->type = type;
    p->next = next;
    return p;
}

static void type_print(struct type *t)
{
    if(!t) return;

    switch(t->kind) {
        case TYPE_VOID:      printf("void"); break;
        case TYPE_BOOLEAN:   printf("bool"); break;
        case TYPE_CHARACTER: printf("char"); break;
        case TYPE_INTEGER:   printf("int"); break;
        case TYPE_STRING:    printf("string"); break;

        case TYPE_ARRAY:
            printf("array of ");
            type_print(t->subtype);
            break;

        case TYPE_FUNCTION:
            printf("function ");
            type_print(t->subtype);
            if(t->params) {
                printf(" (");
                param_list_print(t->params);
                printf(")");
            }
            break;
    }
}

static void param_list_print(struct param_list *p)
{
    if(!p) return;

    printf("%s: ", p->name);
    type_print(p->type);

    if(p->next) {
        printf(", ");
        param_list_print(p->next);
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
    EXPR_STRING_LITERAL,
    EXPR_MATRIX_REF
} expr_t;

struct expr {
    expr_t kind;
    struct expr *left;
    struct expr *right;

    const char *name;
    int integer_value;
    const char *string_literal;

    int row;
    int col;
};

static struct expr * expr_create(expr_t kind, struct expr *left, struct expr *right)
{
    struct expr *e = malloc(sizeof(*e));
    e->kind = kind;
    e->left = left;
    e->right = right;
    e->name = 0;
    e->integer_value = 0;
    e->string_literal = 0;
    e->row = 0;
    e->col = 0;
    return e;
}

static struct expr * expr_create_name(const char *n)
{
    struct expr *e = expr_create(EXPR_NAME, 0, 0);
    e->name = n;
    return e;
}

static struct expr * expr_create_integer_literal(int i)
{
    struct expr *e = expr_create(EXPR_INTEGER_LITERAL, 0, 0);
    e->integer_value = i;
    return e;
}

static struct expr * expr_create_string_literal(const char *str)
{
    struct expr *e = expr_create(EXPR_STRING_LITERAL, 0, 0);
    e->string_literal = str;
    return e;
}

static struct expr * expr_create_matrix_ref(int row, int col)
{
    struct expr *e = expr_create(EXPR_MATRIX_REF, 0, 0);
    e->row = row;
    e->col = col;
    return e;
}

static void expr_print(struct expr *e)
{
    if(!e) return;

    switch(e->kind) {
        case EXPR_NAME:
            printf("%s", e->name);
            break;

        case EXPR_INTEGER_LITERAL:
            printf("%d", e->integer_value);
            break;

        case EXPR_STRING_LITERAL:
            printf("\"%s\"", e->string_literal);
            break;

        case EXPR_MATRIX_REF:
            printf("M[%d][%d]", e->row, e->col);
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

static void expr_print_postorder(struct expr *e)
{
    if(!e) return;

    expr_print_postorder(e->left);
    expr_print_postorder(e->right);

    switch(e->kind) {
        case EXPR_NAME:
            printf("%s ", e->name);
            break;

        case EXPR_INTEGER_LITERAL:
            printf("%d ", e->integer_value);
            break;

        case EXPR_STRING_LITERAL:
            printf("\"%s\" ", e->string_literal);
            break;

        case EXPR_MATRIX_REF:
            printf("M[%d][%d] ", e->row, e->col);
            break;

        case EXPR_ADD:
            printf("+ ");
            break;

        case EXPR_SUB:
            printf("- ");
            break;

        case EXPR_MUL:
            printf("* ");
            break;

        case EXPR_DIV:
            printf("/ ");
            break;
    }
}

/********************************
        DECLARATIONS
********************************/

struct decl {
    char *name;
    struct type *type;
    struct expr *value;
    struct stmt *code;
    struct decl *next;
};

static struct decl * decl_create(char *name,
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

static void decl_print(struct decl *d)
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

static void decl_print_postorder(struct decl *d)
{
    if(!d) return;

    if(d->value) {
        expr_print_postorder(d->value);
    }

    if(d->code) {
        stmt_print_postorder(d->code);
    }

    printf("DECL(%s) ", d->name);

    decl_print_postorder(d->next);
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

    struct expr *init_expr;
    struct expr *expr;
    struct expr *next_expr;

    struct stmt *body;
    struct stmt *else_body;
    struct stmt *next;

    char *name;           /* for scalar assignment */
    struct expr *target;  /* for matrix assignment */
};

static struct stmt * stmt_create(stmt_t kind,
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
    s->init_expr = init_expr;
    s->expr = expr;
    s->next_expr = next_expr;
    s->body = body;
    s->else_body = else_body;
    s->next = next;
    s->name = 0;
    s->target = 0;
    return s;
}

static struct stmt * stmt_create_decl(struct decl *decl)
{
    return stmt_create(STMT_DECL, decl, 0, 0, 0, 0, 0, 0);
}

static struct stmt * stmt_create_expr(struct expr *expr)
{
    return stmt_create(STMT_EXPR, 0, 0, expr, 0, 0, 0, 0);
}

static struct stmt * stmt_create_assign(char *name, struct expr *expr)
{
    struct stmt *s = stmt_create(STMT_ASSIGN, 0, 0, expr, 0, 0, 0, 0);
    s->name = name;
    return s;
}

static struct stmt * stmt_create_matrix_assign(struct expr *target, struct expr *expr)
{
    struct stmt *s = stmt_create(STMT_MATRIX_ASSIGN, 0, 0, expr, 0, 0, 0, 0);
    s->target = target;
    return s;
}

static struct stmt * stmt_append(struct stmt *list, struct stmt *node)
{
    if(!list) return node;

    struct stmt *p = list;
    while(p->next) p = p->next;
    p->next = node;
    return list;
}

static void stmt_print(struct stmt *s)
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
            printf("for (");
            expr_print(s->init_expr);
            printf("; ");
            expr_print(s->expr);
            printf("; ");
            expr_print(s->next_expr);
            printf(")\n");
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

static void stmt_print_postorder(struct stmt *s)
{
    if(!s) return;

    switch(s->kind) {
        case STMT_DECL:
            if(s->decl) decl_print_postorder(s->decl);
            printf("STMT_DECL ");
            break;

        case STMT_EXPR:
            expr_print_postorder(s->expr);
            printf("STMT_EXPR ");
            break;

        case STMT_ASSIGN:
            expr_print_postorder(s->expr);
            printf("ASSIGN(%s) ", s->name);
            break;

        case STMT_MATRIX_ASSIGN:
            expr_print_postorder(s->target);
            expr_print_postorder(s->expr);
            printf("MATRIX_ASSIGN ");
            break;

        case STMT_PRINT:
            expr_print_postorder(s->expr);
            printf("STMT_PRINT ");
            break;

        case STMT_RETURN:
            expr_print_postorder(s->expr);
            printf("STMT_RETURN ");
            break;

        case STMT_IF_ELSE:
            if(s->body) stmt_print_postorder(s->body);
            if(s->else_body) stmt_print_postorder(s->else_body);
            expr_print_postorder(s->expr);
            printf("STMT_IF_ELSE ");
            break;

        case STMT_FOR:
            expr_print_postorder(s->init_expr);
            expr_print_postorder(s->expr);
            expr_print_postorder(s->next_expr);
            if(s->body) stmt_print_postorder(s->body);
            printf("STMT_FOR ");
            break;

        case STMT_BLOCK:
            if(s->body) stmt_print_postorder(s->body);
            printf("STMT_BLOCK ");
            break;
    }

    stmt_print_postorder(s->next);
}

#endif
