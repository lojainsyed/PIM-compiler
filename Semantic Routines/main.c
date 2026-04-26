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
        printf("Usage: %s <input-file>\n", argv[0]);
        return 1;
    }

    yyin = fopen(argv[1], "r");

    if(!yyin) {
        printf("Could not open file: %s\n", argv[1]);
        return 1;
    }

    if(yyparse() == 0) {
        printf("Parse successful!\n");

        printf("\nAST:\n");
        printf("----------------\n");
        stmt_print(parser_result);

        printf("\nSemantic Analysis:\n");
        printf("----------------\n");
        semantic_analyze(parser_result);

        printf("\nSymbol Table:\n");
        printf("----------------\n");
        semantic_print_symbol_table();

        printf("\nExecution:\n");
        printf("----------------\n");
        exec_stmt(parser_result);

    } else {
        printf("Parse failed.\n");
    }

    fclose(yyin);
    return 0;
}
