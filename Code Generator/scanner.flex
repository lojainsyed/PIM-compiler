%{
#include "token.h"
#include <stdlib.h>
#include <string.h>
%}

DIGIT   [0-9]
LETTER  [a-zA-Z]

%%

[ \t\r\n]+                     ;
"//".*                         ;

"print"                        { return TOKEN_PRINT; }
"int"                          { return TOKEN_INT; }
"float"                        { return TOKEN_FLOAT; }
"M"                            { return TOKEN_MATRIX; }

_[A-Za-z0-9_]+                 { return TOKEN_ERROR; }

{DIGIT}+"."{DIGIT}+            { yylval.fvalue = atof(yytext); return TOKEN_FLOAT_NUMBER; }

0{DIGIT}+                      { return TOKEN_ERROR; }

0                              { yylval.fvalue = atof(yytext); return TOKEN_NUMBER; }
[1-9]{DIGIT}*                  { yylval.fvalue = atof(yytext); return TOKEN_NUMBER; }     /*  yylval.fvalue = atof(yytext)  Convert the integer text into a numeric value */

{LETTER}({LETTER}|{DIGIT}|_)*  { yylval.name = strdup(yytext); return TOKEN_IDENT; }   /* yylval.name = strdup(yytext) Copy the identifier name into memory so the parser/AST can use it later */

"="                            { return TOKEN_ASSIGN; }
"+"                            { return TOKEN_PLUS; }
"-"                            { return TOKEN_MINUS; }
"*"                            { return TOKEN_MUL; }
"/"                            { return TOKEN_DIV; }

"("                            { return TOKEN_LPAREN; }
")"                            { return TOKEN_RPAREN; }
"{"                            { return TOKEN_LBRACE; }
"}"                            { return TOKEN_RBRACE; }

"["                            { return TOKEN_LBRACKET; }
"]"                            { return TOKEN_RBRACKET; }

","                            { return TOKEN_COMMA; }
";"                            { return TOKEN_SEMICOLON; }

.                              { return TOKEN_ERROR; }

%%

int yywrap() { return 1; }
