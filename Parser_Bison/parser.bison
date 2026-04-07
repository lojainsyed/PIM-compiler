%{
#include <stdio.h>
#include <stdlib.h>

int yylex(void);
void yyerror(const char *s);

%}

%token TOKEN_INT
%token TOKEN_MATRIX
%token TOKEN_IDENT
%token TOKEN_NUMBER
%token TOKEN_ASSIGN
%token TOKEN_PLUS
%token TOKEN_MINUS
%token TOKEN_MUL
%token TOKEN_LBRACKET
%token TOKEN_RBRACKET
%token TOKEN_SEMICOLON
%token TOKEN_ERROR

%%

program
    : stmt_list
    ;

stmt_list
    : stmt_list stmt
    | stmt
    ;

stmt
    : TOKEN_INT TOKEN_IDENT TOKEN_ASSIGN expr TOKEN_SEMICOLON
    | TOKEN_IDENT TOKEN_ASSIGN expr TOKEN_SEMICOLON
    | matrix_ref TOKEN_ASSIGN expr TOKEN_SEMICOLON
    ;

expr
    : value
    | value TOKEN_PLUS value
    | value TOKEN_MINUS value
    | value TOKEN_MUL value
    ;

value
    : TOKEN_IDENT
    | TOKEN_NUMBER
    | matrix_ref
    ;

matrix_ref
    : TOKEN_MATRIX TOKEN_LBRACKET TOKEN_NUMBER TOKEN_RBRACKET
      TOKEN_LBRACKET TOKEN_NUMBER TOKEN_RBRACKET
    ;

%%

void yyerror(const char *s)
{
    fprintf(stderr, "Syntax error: %s\n", s);
}
