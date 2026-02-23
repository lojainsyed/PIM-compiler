#include <stdio.h>
#include <stdlib.h>
#include "tokens.h"

extern int yylex(void);
extern FILE *yyin;

int main(int argc, char **argv) {
    if (argc == 2) {
        yyin = fopen(argv[1], "r");
        if (!yyin) {
            perror("fopen");
            return 1;
        }
    }

    int tok;
    do {
        tok = yylex();
    } while (tok != TOK_EOF);

    if (yyin && yyin != stdin) fclose(yyin);
    return 0;
}
