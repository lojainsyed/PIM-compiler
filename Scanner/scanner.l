%option noyywrap nodefault yylineno

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tokens.h"

static int col = 1;
static int tok_col = 1;

static void advance_col(const char *s) {
    for (const char *p = s; *p; ++p) {
        if (*p == '\n') col = 1;
        else col++;
    }
}

static void print_token(int tok, const char *lex) {
    if (tok == TOK_EOF) {
        printf("%d:%d\t%-10s\n", yylineno, col, token_name(tok));
        return;
    }
    printf("%d:%d\t%-10s\t%s\n", yylineno, tok_col, token_name(tok), lex);
}
%}

DIGIT       [0-9]
ID_START    [A-Za-z_]
ID_CONT     [A-Za-z0-9_]
ID          {ID_START}{ID_CONT}*
INT         {DIGIT}+
WS          [ \t\r]+

%%

{WS}            { advance_col(yytext); }
"//"[^\n]*      { advance_col(yytext); }
"\n"            { advance_col(yytext); }

"int"           { tok_col = col; advance_col(yytext); print_token(TOK_INT_KW, yytext); return TOK_INT_KW; }
"M"             { tok_col = col; advance_col(yytext); print_token(TOK_MATRIX_ID, yytext); return TOK_MATRIX_ID; }
{ID}            { tok_col = col; advance_col(yytext); print_token(TOK_ID, yytext); return TOK_ID; }

{INT}           { tok_col = col; advance_col(yytext); print_token(TOK_INT_LIT, yytext); return TOK_INT_LIT; }

"["             { tok_col = col; advance_col(yytext); print_token(TOK_LBRACK, yytext); return TOK_LBRACK; }
"]"             { tok_col = col; advance_col(yytext); print_token(TOK_RBRACK, yytext); return TOK_RBRACK; }
"("             { tok_col = col; advance_col(yytext); print_token(TOK_LPAREN, yytext); return TOK_LPAREN; }
")"             { tok_col = col; advance_col(yytext); print_token(TOK_RPAREN, yytext); return TOK_RPAREN; }
";"             { tok_col = col; advance_col(yytext); print_token(TOK_SEMI, yytext);   return TOK_SEMI; }
","             { tok_col = col; advance_col(yytext); print_token(TOK_COMMA, yytext);  return TOK_COMMA; }

"="             { tok_col = col; advance_col(yytext); print_token(TOK_ASSIGN, yytext); return TOK_ASSIGN; }
"+"             { tok_col = col; advance_col(yytext); print_token(TOK_PLUS, yytext);   return TOK_PLUS; }
"-"             { tok_col = col; advance_col(yytext); print_token(TOK_MINUS, yytext);  return TOK_MINUS; }
"*"             { tok_col = col; advance_col(yytext); print_token(TOK_STAR, yytext);   return TOK_STAR; }

.               {
                    tok_col = col;
                    char buf[8];
                    snprintf(buf, sizeof(buf), "%s", yytext);
                    advance_col(yytext);
                    print_token(TOK_ERROR, buf);
                    return TOK_ERROR;
                }

<<EOF>>         { print_token(TOK_EOF, ""); return TOK_EOF; }

%%
