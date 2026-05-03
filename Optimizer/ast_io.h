#ifndef AST_IO_H
#define AST_IO_H

#include <stdio.h>
#include "ast.h"

/* Write AST into IR file */
void ast_write(struct stmt *s, FILE *f);

/* Read IR file and reconstruct AST */
struct stmt *ast_read(FILE *f);

#endif
