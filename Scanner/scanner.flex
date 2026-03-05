%{
#include "token.h"
%}

DIGIT   [0-9]
LETTER  [a-zA-Z]

%%

[ \t\r\n]+                 ;
"//".*                     ;

"int"                      { return TOKEN_INT; }
"M"                        { return TOKEN_MATRIX; }

_[A-Za-z0-9_]+              { return TOKEN_ERROR; }

{LETTER}({LETTER}|{DIGIT}|_)* { return TOKEN_IDENT; }

0{DIGIT}+                  { return TOKEN_ERROR; }

0                          { return TOKEN_NUMBER; }
[1-9]{DIGIT}*              { return TOKEN_NUMBER; }

"="                        { return TOKEN_ASSIGN; }
"+"                        { return TOKEN_PLUS; }
"-"                        { return TOKEN_MINUS; }
"*"                        { return TOKEN_MUL; }

"["                        { return TOKEN_LBRACKET; }
"]"                        { return TOKEN_RBRACKET; }
";"                        { return TOKEN_SEMICOLON; }

.                          { return TOKEN_ERROR; }

%%

int yywrap() { return 1; }
