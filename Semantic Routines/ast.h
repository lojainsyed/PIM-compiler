#ifndef AST_H
#define AST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/********************************
        TYPES
********************************/
/*
    type_t is an enum.

    It represents all possible data types supported by the compiler.

    Example:
        int a;      -> TYPE_INTEGER
        float b;    -> TYPE_FLOAT
*/
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


/*
    Forward declaration.

    struct type uses struct param_list,
    so we declare param_list before fully defining it.
*/
struct param_list;
/*
    struct type represents a data type in the AST.

    kind:
        tells what type it is:
        int, float, string, array, function, etc.

    subtype:
        used for compound types.
        Example:
            array of int
            function returning int

    params:
        used only for function types.
        It stores the function parameter list.
*/
struct type {
    type_t kind;
    struct type *subtype;
    struct param_list *params;
};
/*
    struct param_list represents function parameters.

    Example:
        int add(int x, float y)

    This becomes:

        x -> y

    using the next pointer.
*/
struct param_list {
    char *name;
    struct type *type;
    struct param_list *next;
};

/*
    type_create creates a new type node.

    Example:
        type_create(TYPE_INTEGER, NULL, NULL)

    creates an int type.
*/

static inline struct type *type_create(type_t kind,
                                       struct type *subtype,
                                       struct param_list *params)
{
    struct type *t = malloc(sizeof(*t));
    t->kind = kind;
    t->subtype = subtype;
    t->params = params;
    return t;
}


/*
    param_list_create creates one parameter node.

    Example:
        int x

    name = "x"
    type = TYPE_INTEGER
*/
static inline struct param_list *param_list_create(char *name,
                                                   struct type *type,
                                                   struct param_list *next)
{
    struct param_list *p = malloc(sizeof(*p));
    p->name = name;
    p->type = type;
    p->next = next;
    return p;
}

/********************************
        EXPRESSIONS
********************************/
/********************************
        EXPRESSIONS
********************************/

/*
    expr_t tells what kind of expression this node is.

    Example:
        a + b       -> EXPR_ADD
        a - b       -> EXPR_SUB
        a * b       -> EXPR_MUL
        a / b       -> EXPR_DIV
        a           -> EXPR_NAME
        5           -> EXPR_INTEGER_LITERAL
        2.5         -> EXPR_FLOAT_LITERAL
        "hello"     -> EXPR_STRING_LITERAL
        M[0][1]     -> EXPR_MATRIX_REF
*/
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
/*
    struct expr represents an expression node in the AST.

    kind:
        tells what kind of expression it is.

    left/right:
        used for binary expressions like:
            a + b
            x * y

    name:
        used for variable names.
        Example:
            a

    integer_value:
        used for integer literals.
        Example:
            10

    float_value:
        used for float literals.
        Example:
            2.5

    string_literal:
        used for string literals.
        Example:
            "hello"

    row/col:
        used for matrix reference.
        Example:
            M[0][1]
            row = 0
            col = 1
*/
struct expr {
    expr_t kind;

    struct expr *left;
    struct expr *right;

    char *name;
    int integer_value;
    double float_value;
    char *string_literal;

    struct expr *row;
    struct expr *col;
};
/*
    expr_create creates a general expression node.

    For example:
        expr_create(EXPR_ADD, left, right)

    creates:
        left + right
*/
static inline struct expr *expr_create(expr_t kind,
                                       struct expr *left,
                                       struct expr *right)
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
/*
    Creates an expression node for a variable name.

    Example:
        a

    AST:
        EXPR_NAME
        name = "a"
*/
static inline struct expr *expr_create_name(char *n)
{
    struct expr *e = expr_create(EXPR_NAME, 0, 0);
    e->name = n;
    return e;
}
/*
    Creates an expression node for an integer literal.

    Example:
        10
*/
static inline struct expr *expr_create_integer_literal(int i)
{
    struct expr *e = expr_create(EXPR_INTEGER_LITERAL, 0, 0);
    e->integer_value = i;
    return e;
}
/*
    Creates an expression node for a float literal.

    Example:
        2.5
*/

static inline struct expr *expr_create_float_literal(double f)
{
    struct expr *e = expr_create(EXPR_FLOAT_LITERAL, 0, 0);
    e->float_value = f;
    return e;
}
/*
    Creates an expression node for a string literal.

    Example:
        "hello"
*/
static inline struct expr *expr_create_string_literal(char *s)
{
    struct expr *e = expr_create(EXPR_STRING_LITERAL, 0, 0);
    e->string_literal = s;
    return e;
}

/*
    Creates an expression node for matrix reference.

    Example:
        M[0][1]

    row = 0
    col = 1
*/
static inline struct expr *expr_create_matrix_ref(struct expr *row,
                                                  struct expr *col)
{
    struct expr *e = expr_create(EXPR_MATRIX_REF, 0, 0);
    e->row = row;
    e->col = col;
    return e;
}

/********************************
        DECLARATIONS
********************************/
/*
    Forward declaration.

    struct decl uses struct stmt,
    so we declare stmt before fully defining it.
*/
struct stmt;
/*
    struct decl represents variable declarations.

    Example:
        int a = 5;

    name:
        a

    type:
        int

    value:
        5

    code:
        used for function body if needed.

    next:
        links multiple declarations.
*/
struct decl {
    char *name;
    struct type *type;
    struct expr *value;
    struct stmt *code;
    struct decl *next;
};
/*
    decl_create creates one declaration node.

    Example:
        int a = 5;

    creates:
        name = a
        type = int
        value = 5
*/
static inline struct decl *decl_create(char *name,
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
/********************************
        STATEMENTS
********************************/

/*
    stmt_t tells what kind of statement this node represents.

    Examples:
        int a = 5;       -> STMT_DECL
        a = b + 5;       -> STMT_ASSIGN
        M[0][1] = a;     -> STMT_MATRIX_ASSIGN
        print a;         -> STMT_PRINT
*/
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

static inline struct stmt *stmt_create(stmt_t kind,
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

/********************************
        READABLE PRINT
********************************/

static inline void type_print(struct type *t);

static inline void param_list_print(struct param_list *p)
{
    if(!p) return;

    printf("%s: ", p->name);
    type_print(p->type);

    if(p->next) {
        printf(", ");
        param_list_print(p->next);
    }
}

static inline void type_print(struct type *t)
{
    if(!t) return;

    switch(t->kind) {
        case TYPE_BOOLEAN: printf("bool"); break;
        case TYPE_CHARACTER: printf("char"); break;
        case TYPE_INTEGER: printf("int"); break;
        case TYPE_FLOAT: printf("float"); break;
        case TYPE_VOID: printf("void"); break;
        case TYPE_STRING: printf("string"); break;

        case TYPE_ARRAY:
            printf("array of ");
            type_print(t->subtype);
            break;

        case TYPE_FUNCTION:
            printf("function returning ");
            type_print(t->subtype);
            printf(" params(");
            param_list_print(t->params);
            printf(")");
            break;
    }
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

        case EXPR_STRING_LITERAL:
            printf("\"%s\"", e->string_literal);
            break;

        case EXPR_NAME:
            printf("%s", e->name);
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

static inline void stmt_print(struct stmt *s);

static inline void decl_print(struct decl *d)
{
    while(d) {
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

        d = d->next;
    }
}

static inline void stmt_print(struct stmt *s)
{
    while(s) {
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

            case STMT_BLOCK:
                printf("{\n");
                stmt_print(s->body);
                printf("}\n");
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
        }

        s = s->next;
    }
}

/********************************
        TREE PRINT - BOOK STYLE WITH next
********************************/

static inline void indent(int lvl)
{
    for(int i = 0; i < lvl; i++) {
        printf("  ");
    }
}

static inline void type_print_tree(struct type *t, int lvl)
{
    if(!t) return;

    indent(lvl);

    switch(t->kind) {
        case TYPE_INTEGER:
            printf("TYPE int\n");
            break;

        case TYPE_FLOAT:
            printf("TYPE float\n");
            break;

        case TYPE_BOOLEAN:
            printf("TYPE bool\n");
            break;

        case TYPE_CHARACTER:
            printf("TYPE char\n");
            break;

        case TYPE_STRING:
            printf("TYPE string\n");
            break;

        case TYPE_VOID:
            printf("TYPE void\n");
            break;

        case TYPE_ARRAY:
            printf("TYPE_ARRAY\n");
            indent(lvl + 1);
            printf("subtype:\n");
            type_print_tree(t->subtype, lvl + 2);
            break;

        case TYPE_FUNCTION:
            printf("TYPE_FUNCTION\n");

            indent(lvl + 1);
            printf("return_type:\n");
            type_print_tree(t->subtype, lvl + 2);

            indent(lvl + 1);
            printf("params:\n");

            if(t->params) {
                struct param_list *p = t->params;
                indent(lvl + 2);
                printf("PARAM name=%s\n", p->name);

                indent(lvl + 3);
                printf("type:\n");
                type_print_tree(p->type, lvl + 4);

                while(p->next) {
                    p = p->next;
                    indent(lvl + 2);
                    printf("next:\n");

                    indent(lvl + 3);
                    printf("PARAM name=%s\n", p->name);

                    indent(lvl + 4);
                    printf("type:\n");
                    type_print_tree(p->type, lvl + 5);
                }
            }
            break;
    }
}

static inline void expr_print_tree(struct expr *e, int lvl)
{
    if(!e) return;

    indent(lvl);

    switch(e->kind) {
        case EXPR_INTEGER_LITERAL:
            printf("EXPR_INT value=%d\n", e->integer_value);
            break;

        case EXPR_FLOAT_LITERAL:
            printf("EXPR_FLOAT value=%f\n", e->float_value);
            break;

        case EXPR_STRING_LITERAL:
            printf("EXPR_STRING value=\"%s\"\n", e->string_literal);
            break;

        case EXPR_NAME:
            printf("EXPR_NAME name=%s\n", e->name);
            break;

        case EXPR_MATRIX_REF:
            printf("EXPR_MATRIX_REF\n");

            indent(lvl + 1);
            printf("row:\n");
            expr_print_tree(e->row, lvl + 2);

            indent(lvl + 1);
            printf("col:\n");
            expr_print_tree(e->col, lvl + 2);
            break;

        case EXPR_ADD:
            printf("EXPR_ADD\n");

            indent(lvl + 1);
            printf("left:\n");
            expr_print_tree(e->left, lvl + 2);

            indent(lvl + 1);
            printf("right:\n");
            expr_print_tree(e->right, lvl + 2);
            break;

        case EXPR_SUB:
            printf("EXPR_SUB\n");

            indent(lvl + 1);
            printf("left:\n");
            expr_print_tree(e->left, lvl + 2);

            indent(lvl + 1);
            printf("right:\n");
            expr_print_tree(e->right, lvl + 2);
            break;

        case EXPR_MUL:
            printf("EXPR_MUL\n");

            indent(lvl + 1);
            printf("left:\n");
            expr_print_tree(e->left, lvl + 2);

            indent(lvl + 1);
            printf("right:\n");
            expr_print_tree(e->right, lvl + 2);
            break;

        case EXPR_DIV:
            printf("EXPR_DIV\n");

            indent(lvl + 1);
            printf("left:\n");
            expr_print_tree(e->left, lvl + 2);

            indent(lvl + 1);
            printf("right:\n");
            expr_print_tree(e->right, lvl + 2);
            break;
    }
}

static inline void stmt_print_tree(struct stmt *s, int lvl);

static inline void decl_print_tree(struct decl *d, int lvl)
{
    if(!d) return;

    indent(lvl);
    printf("DECL name=%s\n", d->name);

    indent(lvl + 1);
    printf("type:\n");
    type_print_tree(d->type, lvl + 2);

    if(d->value) {
        indent(lvl + 1);
        printf("value:\n");
        expr_print_tree(d->value, lvl + 2);
    }

    if(d->code) {
        indent(lvl + 1);
        printf("code:\n");
        stmt_print_tree(d->code, lvl + 2);
    }

    if(d->next) {
        indent(lvl);
        printf("next:\n");
        decl_print_tree(d->next, lvl + 1);
    }
}

static inline void stmt_print_tree(struct stmt *s, int lvl)
{
    if(!s) return;

    indent(lvl);

    switch(s->kind) {
        case STMT_DECL:
            printf("STMT_DECL\n");

            indent(lvl + 1);
            printf("decl:\n");
            decl_print_tree(s->decl, lvl + 2);
            break;

        case STMT_EXPR:
            printf("STMT_EXPR\n");

            indent(lvl + 1);
            printf("expr:\n");
            expr_print_tree(s->expr, lvl + 2);
            break;

        case STMT_ASSIGN:
            printf("STMT_ASSIGN name=%s\n", s->name);

            indent(lvl + 1);
            printf("value:\n");
            expr_print_tree(s->expr, lvl + 2);
            break;

        case STMT_MATRIX_ASSIGN:
            printf("STMT_MATRIX_ASSIGN\n");

            indent(lvl + 1);
            printf("target:\n");
            expr_print_tree(s->target, lvl + 2);

            indent(lvl + 1);
            printf("value:\n");
            expr_print_tree(s->expr, lvl + 2);
            break;

        case STMT_PRINT:
            printf("STMT_PRINT\n");

            indent(lvl + 1);
            printf("expr:\n");
            expr_print_tree(s->expr, lvl + 2);
            break;

        case STMT_RETURN:
            printf("STMT_RETURN\n");

            indent(lvl + 1);
            printf("expr:\n");
            expr_print_tree(s->expr, lvl + 2);
            break;

        case STMT_BLOCK:
            printf("STMT_BLOCK\n");

            indent(lvl + 1);
            printf("body:\n");
            stmt_print_tree(s->body, lvl + 2);
            break;

        case STMT_IF_ELSE:
            printf("STMT_IF_ELSE\n");

            indent(lvl + 1);
            printf("condition:\n");
            expr_print_tree(s->expr, lvl + 2);

            indent(lvl + 1);
            printf("body:\n");
            stmt_print_tree(s->body, lvl + 2);

            if(s->else_body) {
                indent(lvl + 1);
                printf("else_body:\n");
                stmt_print_tree(s->else_body, lvl + 2);
            }
            break;

        case STMT_FOR:
            printf("STMT_FOR\n");

            indent(lvl + 1);
            printf("init_expr:\n");
            expr_print_tree(s->init_expr, lvl + 2);

            indent(lvl + 1);
            printf("condition:\n");
            expr_print_tree(s->expr, lvl + 2);

            indent(lvl + 1);
            printf("next_expr:\n");
            expr_print_tree(s->next_expr, lvl + 2);

            indent(lvl + 1);
            printf("body:\n");
            stmt_print_tree(s->body, lvl + 2);
            break;
    }

    if(s->next) {
        indent(lvl);
        printf("next:\n");
        stmt_print_tree(s->next, lvl + 1);
    }
}

#endif
