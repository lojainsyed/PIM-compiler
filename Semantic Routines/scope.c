
Andrew Shatz  2 days ago
scope.c


// scope.c - implementation skeleton (with fixes and explanations)
#include "scope.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>   // for error reporting if needed

static scope_t scope_stack[MAX_SCOPE_DEPTH];
static int current_level = 0;   // starts at global scope (level 0)

void scope_enter(void) {
    if (current_level >= MAX_SCOPE_DEPTH - 1) {
        fprintf(stderr, "Error: scope depth exceeded\n");
        exit(1);
    }
    current_level++;
    scope_stack[current_level].num_symbols = 0;   // new empty scope
    // printf("Entered scope level %d\n", current_level);  // for debugging
}

void scope_exit(void) {
    if (current_level <= 0) {
        fprintf(stderr, "Error: exiting global scope\n");
        return;
    }
    // Optional: you can free symbols here if you own them, but usually not (symbols live longer)
    current_level--;
    // printf("Exited to scope level %d\n", current_level);
}

int scope_level(void) {
    return current_level;
}

// Bind a symbol to the CURRENT (top) scope only
void scope_bind(const char *name, struct symbol *sym) {
    if (!name || !sym) return;

    scope_t *current = &scope_stack[current_level];

    // Optional: check for redeclaration in current scope (good practice)
    if (scope_lookup_current(name) != NULL) {
        fprintf(stderr, "Error: redeclaration of '%s' in current scope\n", name);
        // You may want to continue or handle error per your project's policy
    }

    if (current->num_symbols >= MAX_SYMBOLS_PER_SCOPE) {
        fprintf(stderr, "Error: too many symbols in current scope\n");
        return;
    }

    current->symbols[current->num_symbols++] = sym;
    sym->name = strdup(name);   // own a copy if needed (or assume caller does)
}

// Lookup from current scope ONLY (used for redeclaration checks)
struct symbol *scope_lookup_current(const char *name) {
    if (!name) return NULL;

    scope_t *current = &scope_stack[current_level];
    for (int i = 0; i < current->num_symbols; i++) {
        struct symbol *s = current->symbols[i];
        if (s && s->name && strcmp(s->name, name) == 0) {
            return s;
        }
    }
    return NULL;
}

// Lookup from innermost (current) to outermost (global) scope
struct symbol *scope_lookup(const char *name) {
    if (!name) return NULL;

    for (int level = current_level; level >= 0; level--) {
        scope_t *sc = &scope_stack[level];
        for (int i = 0; i < sc->num_symbols; i++) {
            struct symbol *s = sc->symbols[i];
            if (s && s->name && strcmp(s->name, name) == 0) {
                return s;
            }
        }
    }
    return NULL;   // not found
