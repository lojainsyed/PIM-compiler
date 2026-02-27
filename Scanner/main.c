#include <stdio.h>
#include "tokens.h"

extern int yylex(void);
extern FILE *yyin;

int main(int argc, char **argv)
{
    if (argc == 2)
        yyin = fopen(argv[1], "r");

    while (yylex() != EOF_TOK);

    return 0;
}
