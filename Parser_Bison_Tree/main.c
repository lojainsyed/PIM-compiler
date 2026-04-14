#include <stdio.h>
#include "ast.h"

/* These are provided by Bison/Flex */
extern struct stmt *parser_result;
extern int yyparse(void);
extern FILE *yyin;

int main(int argc, char **argv)
{
    /* Check if input file is provided */
    if (argc < 2) {
        printf("Usage: %s <input_file>\n", argv[0]);
        return -1;
    }

    /* Open input file */
    yyin = fopen(argv[1], "r");
    if (!yyin) {
        printf("Error: Could not open file %s\n", argv[1]);
        return -1;
    }

    /* Run parser */
    if (yyparse() == 0) {
        printf("Parse successful!\n\n");

        printf("AST:\n");
        printf("-------\n");

        /* Print the AST */
        stmt_print(parser_result);

        printf("\n");
    } else {
        printf("Parse failed.\n");
    }

    /* Close file */
    fclose(yyin);

    return 0;
}
