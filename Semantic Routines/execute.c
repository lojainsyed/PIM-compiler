#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "execute.h"

struct runtime_value {
    char *name;
    double value;
};

static struct runtime_value runtime_table[100];
static int runtime_count = 0;

static double M_runtime[100][100];

static void runtime_store(const char *name, double value)
{
    for(int i = 0; i < runtime_count; i++) {
        if(strcmp(runtime_table[i].name, name) == 0) {
            runtime_table[i].value = value;
            return;
        }
    }

    runtime_table[runtime_count].name = strdup(name);
    runtime_table[runtime_count].value = value;
    runtime_count++;
}

static double runtime_lookup(const char *name)
{
    for(int i = 0; i < runtime_count; i++) {
        if(strcmp(runtime_table[i].name, name) == 0) {
            return runtime_table[i].value;
        }
    }

    printf("Runtime error: variable %s not found\n", name);
    return 0;
}

static double eval_expr(struct expr *e)
{
    if(!e) return 0;

    switch(e->kind) {
        case EXPR_INTEGER_LITERAL:
            return e->integer_value;

        case EXPR_FLOAT_LITERAL:
            return e->float_value;

        case EXPR_NAME:
            return runtime_lookup(e->name);

        case EXPR_MATRIX_REF: {
            int r = (int)eval_expr(e->row);
            int c = (int)eval_expr(e->col);
            return M_runtime[r][c];
        }

        case EXPR_ADD:
            return eval_expr(e->left) + eval_expr(e->right);

        case EXPR_SUB:
            return eval_expr(e->left) - eval_expr(e->right);

        case EXPR_MUL:
            return eval_expr(e->left) * eval_expr(e->right);

        case EXPR_DIV:
            return eval_expr(e->left) / eval_expr(e->right);

        default:
            return 0;
    }
}

static void exec_decl(struct decl *d);

void exec_stmt(struct stmt *s)
{
    while(s) {
        switch(s->kind) {
            case STMT_DECL:
                exec_decl(s->decl);
                break;

            case STMT_ASSIGN: {
                double value = eval_expr(s->expr);
                runtime_store(s->name, value);
                break;
            }

            case STMT_MATRIX_ASSIGN: {
                int r = (int)eval_expr(s->target->row);
                int c = (int)eval_expr(s->target->col);
                double value = eval_expr(s->expr);
                M_runtime[r][c] = value;
                break;
            }

            case STMT_PRINT: {
                double value = eval_expr(s->expr);

                if(value == (int)value)
                    printf("OUTPUT: %d\n", (int)value);
                else
                    printf("OUTPUT: %f\n", value);

                break;
            }

            case STMT_BLOCK:
                exec_stmt(s->body);
                break;

            default:
                break;
        }

        s = s->next;
    }
}

static void exec_decl(struct decl *d)
{
    while(d) {
        if(d->value) {
            double value = eval_expr(d->value);
            runtime_store(d->name, value);
        }

        if(d->code) {
            exec_stmt(d->code);
        }

        d = d->next;
    }
}
