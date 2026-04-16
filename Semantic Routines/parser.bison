%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

extern char *yytext;
extern int yylex(void);
void yyerror(const char *s);

struct stmt *parser_result = 0;
%}

%union {
    struct stmt *stmt;
    struct expr *expr;
    char *name;
    int value;
}

%token TOKEN_INT
%token TOKEN_MATRIX
%token TOKEN_ASSIGN
%token TOKEN_PLUS
%token TOKEN_MINUS
%token TOKEN_MUL
%token TOKEN_LBRACKET
%token TOKEN_RBRACKET
%token TOKEN_SEMICOLON
%token TOKEN_ERROR

%token <name> TOKEN_IDENT
%token <value> TOKEN_NUMBER

%type <stmt> program stmt_list stmt
%type <expr> expr value matrix_ref

%%

program
    : stmt_list
      {
          parser_result = $1;
          $$ = $1;
      }
    ;

stmt_list
    : stmt_list stmt
      {
          $$ = stmt_append($1, $2);
      }
    | stmt
      {
          $$ = $1;
      }
    ;

stmt
    : TOKEN_INT TOKEN_IDENT TOKEN_ASSIGN expr TOKEN_SEMICOLON
      {
          struct decl *d = decl_create(
              $2,
              type_create(TYPE_INTEGER, 0, 0),
              $4,
              0,
              0
          );
          $$ = stmt_create_decl(d);
      }
    | TOKEN_IDENT TOKEN_ASSIGN expr TOKEN_SEMICOLON
      {
          $$ = stmt_create_assign($1, $3);
      }
    | matrix_ref TOKEN_ASSIGN expr TOKEN_SEMICOLON
      {
          $$ = stmt_create_matrix_assign($1, $3);
      }
    ;

expr
    : value
      {
          $$ = $1;
      }
    | value TOKEN_PLUS value
      {
          $$ = expr_create(EXPR_ADD, $1, $3);
      }
    | value TOKEN_MINUS value
      {
          $$ = expr_create(EXPR_SUB, $1, $3);
      }
    | value TOKEN_MUL value
      {
          $$ = expr_create(EXPR_MUL, $1, $3);
      }
    ;

value
    : TOKEN_IDENT
      {
          $$ = expr_create_name($1);
      }
    | TOKEN_NUMBER
      {
          $$ = expr_create_integer_literal($1);
      }
    | matrix_ref
      {
          $$ = $1;
      }
    ;

matrix_ref
    : TOKEN_MATRIX TOKEN_LBRACKET TOKEN_NUMBER TOKEN_RBRACKET
      TOKEN_LBRACKET TOKEN_NUMBER TOKEN_RBRACKET
      {
          $$ = expr_create_matrix_ref($3, $6);
      }
    ;

%%

void yyerror(const char *s)
{
    printf("parse error: %s\n", s);
}
