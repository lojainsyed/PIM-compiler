#ifndef OPTIMIZER_H
#define OPTIMIZER_H

#include "ast.h"

/* Main optimizer */
struct stmt *optimize_program(struct stmt *s);

/* Optimization measurement */
int count_expr_ops(struct expr *e);
int count_stmt_ops(struct stmt *s);
int count_statements(struct stmt *s);

#endif
