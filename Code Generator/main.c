#include <stdio.h>
#include "ast.h"
#include "semantic.h"

extern struct stmt *parser_result;
extern int yyparse(void);
extern FILE *yyin;

int main(int argc, char **argv)
{
    if(argc < 2) {
        printf("File name is missing!\n");
        return -1;
    }

    yyin = fopen(argv[1], "r");
    if(!yyin) {
        printf("Could not open the program!\n");
        return -1;
    }

    if(yyparse() == 0) {
        printf("Parse successful!\n\n");

        printf("AST (Readable Form):\n");
        printf("--------------------\n");
        stmt_print(parser_result);


        semantic_analyze(parser_result);
        semantic_print_symbol_table();
    } else {
        printf("Parse failed.\n");
    }

    fclose(yyin);
    return 0;
}
