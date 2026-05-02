#include <stdio.h>
#include <stdlib.h>

#include "ast.h"
#include "ast_io.h"
#include "code_generator.h"
#include "semantic.h"

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

    if (yyparse() == 0) {
        printf("Parse successful!\n");

        printf("\n=== AST READABLE FORM ===\n");
        stmt_print(parser_result);

        printf("\n=== AST TREE ===\n");
        printf("PROGRAM\n");
        printf("  stmt_list:\n");
        stmt_print_tree(parser_result, 2);

        printf("\n=== Semantic Analysis ===\n");
        semantic_analyze(parser_result);
        semantic_print_symbol_table();

        FILE *f = fopen("ir.txt", "w");
        ast_write(parser_result, f);
        fclose(f);

        printf("\nIR written to ir.txt\n");

        f = fopen("ir.txt", "r");
        struct stmt *new_ast = ast_read(f);
        fclose(f);

        printf("\n=== Reconstructed AST READABLE FORM ===\n");
        stmt_print(new_ast);

        printf("\n=== Reconstructed AST TREE ===\n");
        printf("PROGRAM\n");
        printf("  stmt_list:\n");
        stmt_print_tree(new_ast, 2);

        printf("\n=== Code Generation ===\n");
        generate_code(new_ast);

    } else {
        printf("Parse failed.\n");
    }

    return 0;
}
