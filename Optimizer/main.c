#include <stdio.h>
#include <stdlib.h>

#include "ast.h"
#include "ast_io.h"
#include "code_generator.h"
#include "semantic.h"
#include "optimizer.h"

extern int yyparse(void);
extern FILE *yyin;
extern struct stmt *parser_result;

int main(int argc, char **argv)
{
    if (argc < 2) {
        printf("Usage: ./compiler.exe <input-file>\n");
        return 1;
    }

    yyin = fopen(argv[1], "r");
    if (!yyin) {
        printf("Error: could not open file %s\n", argv[1]);
        return 1;
    }

    printf("\n=== Parsing ===\n");

    if (yyparse() == 0) {

        fclose(yyin);

        printf("Parse successful!\n");

        printf("\n=== AST BEFORE OPTIMIZATION ===\n");
        stmt_print(parser_result);

        printf("\n=== AST TREE BEFORE OPTIMIZATION ===\n");
        printf("PROGRAM\n");
        printf("  stmt_list:\n");
        stmt_print_tree(parser_result, 2);

        int before_ops = count_stmt_ops(parser_result);
        int before_stmts = count_statements(parser_result);

        printf("\n=== Semantic Analysis ===\n");
        semantic_analyze(parser_result);

        printf("\n=== Symbol Table ===\n");
        semantic_print_symbol_table();

        printf("\n=== OPTIMIZER ===\n");

        parser_result = optimize_program(parser_result);

        int after_ops = count_stmt_ops(parser_result);
        int after_stmts = count_statements(parser_result);

        printf("\n=== AST AFTER OPTIMIZATION ===\n");
        stmt_print(parser_result);

        printf("\n=== AST TREE AFTER OPTIMIZATION ===\n");
        printf("PROGRAM\n");
        printf("  stmt_list:\n");
        stmt_print_tree(parser_result, 2);

        printf("\n=== OPTIMIZATION IMPACT ===\n");
        printf("Statements Before : %d\n", before_stmts);
        printf("Statements After  : %d\n", after_stmts);
        printf("Statements Removed: %d\n", before_stmts - after_stmts);
        printf("Operations Before : %d\n", before_ops);
        printf("Operations After  : %d\n", after_ops);
        printf("Operations Reduced: %d\n", before_ops - after_ops);

        if(before_ops > 0) {
            float reduction = ((float)(before_ops - after_ops) / before_ops) * 100.0;
            printf("Operation Reduction Percentage: %.2f%%\n", reduction);
        }

        printf("\n=== IR Generation ===\n");

        FILE *f = fopen("ir.txt", "w");
        if(!f) {
            printf("Error: could not write ir.txt\n");
            return 1;
        }

        ast_write(parser_result, f);
        fclose(f);

        printf("IR written to ir.txt\n");

        printf("\n=== AST FROM IR ===\n");

        f = fopen("ir.txt", "r");
        if(!f) {
            printf("Error: could not read ir.txt\n");
            return 1;
        }

        struct stmt *new_ast = ast_read(f);
        fclose(f);

        printf("\n=== Reconstructed AST READABLE FORM ===\n");
        stmt_print(new_ast);

        printf("\n=== Reconstructed AST TREE ===\n");
        printf("PROGRAM\n");
        printf("  stmt_list:\n");
        stmt_print_tree(new_ast, 2);

        printf("\n=== Assembly / PIM Code ===\n");
        generate_code(new_ast);

        printf("\n=== DONE ===\n");

    } else {
        printf("Parse failed.\n");
        fclose(yyin);
        return 1;
    }

    return 0;
}
