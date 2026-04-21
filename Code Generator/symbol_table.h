#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include "ast.h"

/*****************
      SYMBOL
******************/

typedef enum {
    SYMBOL_LOCAL,
    SYMBOL_PARAM,
    SYMBOL_GLOBAL
} symbol_t;

struct symbol {
    symbol_t kind;
    struct type *type;
    char *name;
    int which;
};

static struct symbol * symbol_create(symbol_t kind, struct type *type, char *name)
{
    struct symbol *s = malloc(sizeof(*s));
    s->kind = kind;
    s->name = strdup(name);
    s->type = type;
    s->which = 0;
    return s;
}

/*****************
      BINDING
******************/

struct binding {
    char *name;
    struct symbol *symbol;
    struct binding *next;
};

/*****************
      SCOPE
******************/

struct scope {
    struct binding *bindings;
    struct scope *next;
};

static struct scope *scope_head = 0;
static int which_global = 0;
static int which_local = 0;
static int which_param = 0;

/*****************
   HELPER PRINT
******************/

static const char * symbol_kind_string(symbol_t kind)
{
    switch(kind) {
        case SYMBOL_GLOBAL: return "GLOBAL";
        case SYMBOL_LOCAL:  return "LOCAL";
        case SYMBOL_PARAM:  return "PARAM";
        default:            return "UNKNOWN";
    }
}

/*****************
   SCOPE CONTROL
******************/

static void scope_enter(void)
{
    struct scope *s = malloc(sizeof(*s));
    s->bindings = 0;
    s->next = scope_head;
    scope_head = s;

    if(scope_head && !scope_head->next) {
        which_global = 0;
    } else {
        which_local = 0;
    }
}

static void scope_exit(void)
{
    if(!scope_head) return;

    struct binding *b = scope_head->bindings;
    while(b) {
        struct binding *next = b->next;
        free(b->name);
        free(b);
        b = next;
    }

    struct scope *temp = scope_head;
    scope_head = scope_head->next;
    free(temp);
}

/*****************
   LOOKUP / BIND
******************/

static struct symbol * scope_lookup_current(const char *name)
{
    if(!scope_head) return 0;

    struct binding *b = scope_head->bindings;
    while(b) {
        if(strcmp(b->name, name) == 0) {
            return b->symbol;
        }
        b = b->next;
    }

    return 0;
}

static struct symbol * scope_lookup(const char *name)
{
    struct scope *s = scope_head;

    while(s) {
        struct binding *b = s->bindings;
        while(b) {
            if(strcmp(b->name, name) == 0) {
                return b->symbol;
            }
            b = b->next;
        }
        s = s->next;
    }

    return 0;
}

static void scope_bind(const char *name, struct symbol *sym)
{
    if(!scope_head) {
        printf("Error: no active scope for %s\n", name);
        exit(1);
    }

    struct binding *b = malloc(sizeof(*b));
    b->name = strdup(name);
    b->symbol = sym;
    b->next = scope_head->bindings;
    scope_head->bindings = b;
}

static struct symbol * scope_bind_symbol(symbol_t kind, struct type *type, char *name)
{
    if(scope_lookup_current(name)) {
        return 0;
    }

    struct symbol *s = symbol_create(kind, type, name);

    switch(kind) {
        case SYMBOL_GLOBAL:
            s->which = which_global++;
            break;
        case SYMBOL_LOCAL:
            s->which = which_local++;
            break;
        case SYMBOL_PARAM:
            s->which = which_param++;
            break;
    }

    scope_bind(name, s);
    return s;
}

/*****************
   PRINT TABLE
******************/

static void scope_print_current(void)
{
    if(!scope_head) {
        printf("(no active scope)\n");
        return;
    }

    struct binding *b = scope_head->bindings;

    printf("--------------------------------------------------\n");
    printf("%-15s %-10s %-15s %-5s\n", "Name", "Kind", "Type", "Which");
    printf("--------------------------------------------------\n");

    while(b) {
        printf("%-15s %-10s ", b->name, symbol_kind_string(b->symbol->kind));
        type_print(b->symbol->type);
        printf(" %-5d\n", b->symbol->which);
        b = b->next;
    }

    printf("--------------------------------------------------\n");
}

#endif
