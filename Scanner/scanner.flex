%option noyywrap

%{
#include <stdio.h>
%}

ID      [A-Za-z][A-Za-z0-9_]*
INT_OK  0|[1-9][0-9]*

%%

[ \t\r\n]+        ;
"//".*            ;

"int"             { printf("KW_INT\t%s\n", yytext); }
"M"               { printf("KW_MATRIX\t%s\n", yytext); }

_[A-Za-z0-9_]+    { printf("ERROR\t%s\n", yytext); }

{ID}              { printf("ID\t%s\n", yytext); }

0[0-9]+           { printf("ERROR\t%s\n", yytext); }

{INT_OK}          { printf("INT_LITERAL\t%s\n", yytext); }

"="               { printf("ASSIGN\t%s\n", yytext); }
"+"               { printf("PLUS\t%s\n", yytext); }
"-"               { printf("MINUS\t%s\n", yytext); }
"*"               { printf("MUL\t%s\n", yytext); }

"["               { printf("LBRACKET\t%s\n", yytext); }
"]"               { printf("RBRACKET\t%s\n", yytext); }
";"               { printf("SEMICOLON\t%s\n", yytext); }

.                 { printf("ERROR\t%s\n", yytext); }

%%

int main(int argc, char **argv)
{
    if (argc > 1)
        yyin = fopen(argv[1], "r");
    yylex();
    return 0;
}
