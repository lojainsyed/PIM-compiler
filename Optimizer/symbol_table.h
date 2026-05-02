#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

struct scope {
    struct symbol *symbols[100];
    int count;
    int level;
    struct scope *next;
};

static struct scope *scope_head = NULL;
static struct scope *all_scopes[100];
static int scope_count = 0;

static int global_which = 0;
static int local_which = 0;
static int param_which = 0;

static inline struct symbol * symbol_create(symbol_t kind, struct type *type, char *name)
{
    struct symbol *s = malloc(sizeof(*s));
    s->kind = kind;
    s->type = type;
    s->name = name;

    if(kind == SYMBOL_GLOBAL) {
        s->which = global_which++;
    } else if(kind == SYMBOL_PARAM) {
        s->which = param_which++;
    } else {
        s->which = local_which++;
    }

    return s;
}

static inline void scope_enter(void)
{
    struct scope *s = malloc(sizeof(*s));
    s->count = 0;
    s->level = scope_head ? scope_head->level + 1 : 0;
    s->next = scope_head;

    scope_head = s;
    all_scopes[scope_count++] = s;
}

static inline void scope_exit(void)
{
    if(scope_head) {
        scope_head = scope_head->next;
    }
}

static inline struct symbol * scope_lookup_current(const char *name)
{
    if(!scope_head) return NULL;

    for(int i = 0; i < scope_head->count; i++) {
        if(strcmp(scope_head->symbols[i]->name, name) == 0) {
            return scope_head->symbols[i];
        }
    }

    return NULL;
}

static inline struct symbol * scope_lookup(const char *name)
{
    struct scope *s = scope_head;

    while(s) {
        for(int i = 0; i < s->count; i++) {
            if(strcmp(s->symbols[i]->name, name) == 0) {
                return s->symbols[i];
            }
        }
        s = s->next;
    }

    return NULL;
}

static inline int scope_bind_symbol(symbol_t kind, struct type *type, char *name)
{
    if(!scope_head) scope_enter();

    if(scope_lookup_current(name)) {
        return 0;
    }

    struct symbol *sym = symbol_create(kind, type, name);
    scope_head->symbols[scope_head->count++] = sym;

    return 1;
}

static inline void scope_print_all(void)
{
    printf("\nNested Symbol Table:\n");
    printf("====================\n");

    for(int i = scope_count - 1; i >= 0; i--) {
        struct scope *s = all_scopes[i];

        printf("\nScope Level %d:\n", s->level);
        printf("-------------------------------------\n");
        printf("%-15s %-10s %-20s %-5s\n", "Name", "Kind", "Type", "Which");

        for(int j = 0; j < s->count; j++) {
            struct symbol *sym = s->symbols[j];

            printf("%-15s ", sym->name);

            if(sym->kind == SYMBOL_GLOBAL)
                printf("%-10s ", "GLOBAL");
            else if(sym->kind == SYMBOL_PARAM)
                printf("%-10s ", "PARAM");
            else
                printf("%-10s ", "LOCAL");

            type_print(sym->type);
            printf(" %-5d\n", sym->which);
        }
    }
}

#endif
