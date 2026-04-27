#include <stdio.h>
#include "ast.h"
#include "semantic.h"
#include "execute.h"

extern FILE *yyin;
extern int yyparse(void);
extern struct stmt *parser_result;

int main(int argc, char **argv)
{
    if(argc < 2) {
        printf("File name is missing!\n");
        return 1;
    }

    yyin = fopen(argv[1], "r");

    if(!yyin) {
        printf("Could not open the program!\n");
        return 1;
    }

    if(yyparse() == 0) {

        printf("Parse successful!\n\n");

        // ===== READABLE =====
        printf("AST READABLE FORM:\n");
        printf("------------------\n");
        stmt_print(parser_result);

        // ===== TREE =====
        printf("\nAST TREE FORM (BOOK STYLE):\n");
        printf("---------------------------\n");
        printf("PROGRAM\n");
        printf("  stmt_list:\n");
        stmt_print_tree(parser_result, 2);

        // ===== SEMANTIC =====
        printf("\nSemantic Analysis:\n");
        printf("------------------\n");
        semantic_analyze(parser_result);

        printf("\nSymbol Table:\n");
        printf("------------------\n");
        semantic_print_symbol_table();

        // ===== EXECUTE =====
        printf("\nExecution:\n");
        printf("------------------\n");
        exec_stmt(parser_result);
    }
    else {
        printf("Parse failed.\n");
    }

    fclose(yyin);
    return 0;
}
