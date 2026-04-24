#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "ast.h"

void semantic_analyze(struct stmt *root);
void semantic_print_symbol_table(void);

#endif
