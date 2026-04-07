#include <stdio.h>

int yyparse(void);

int main(void)
{
    if (yyparse() == 0) {
        printf("Parse successful.\n");
    } else {
        printf("Parse failed.\n");
    }

    return 0;
}
