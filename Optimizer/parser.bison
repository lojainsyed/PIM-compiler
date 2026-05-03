%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

extern int yylex(void);
int yyerror(const char *s);

struct stmt *parser_result = 0;
%}

%union {
    struct stmt *stmt;
    struct decl *decl;
    struct expr *expr;
    struct type *type;
    struct param_list *param;
    char *name;
    double fvalue;
}

%token TOKEN_INT
%token TOKEN_FLOAT
%token TOKEN_PRINT
%token TOKEN_MATRIX
%token TOKEN_ERROR

%token TOKEN_PLUS TOKEN_MINUS TOKEN_MUL TOKEN_DIV
%token TOKEN_ASSIGN TOKEN_SEMICOLON

%token TOKEN_LPAREN TOKEN_RPAREN
%token TOKEN_LBRACE TOKEN_RBRACE
%token TOKEN_LBRACKET TOKEN_RBRACKET
%token TOKEN_COMMA

%token <fvalue> TOKEN_NUMBER
%token <fvalue> TOKEN_FLOAT_NUMBER
%token <name> TOKEN_IDENT

%type <stmt> program stmt_list stmt block
%type <decl> decl
%type <expr> expr term factor
%type <type> type
%type <param> param param_list param_list_opt

%%

program
    : stmt_list
      {
          parser_result = $1;
      }
;

stmt_list
    : stmt_list stmt
      {
          $$ = $1;
          struct stmt *t = $$;
          while(t->next) t = t->next;
          t->next = $2;
      }

    | stmt
      {
          $$ = $1;
      }
;

stmt
    : decl
      {
          $$ = stmt_create(STMT_DECL, $1, 0, 0, 0, 0, 0, 0);
      }

    | TOKEN_IDENT TOKEN_ASSIGN expr TOKEN_SEMICOLON
      {
          $$ = stmt_create(STMT_ASSIGN, 0, 0, $3, 0, 0, 0, 0);
          $$->name = $1;
      }

    | TOKEN_MATRIX TOKEN_LBRACKET expr TOKEN_RBRACKET
      TOKEN_LBRACKET expr TOKEN_RBRACKET
      TOKEN_ASSIGN expr TOKEN_SEMICOLON
      {
          struct expr *ref = expr_create_matrix_ref($3, $6);
          $$ = stmt_create(STMT_MATRIX_ASSIGN, 0, 0, $9, 0, 0, 0, 0);
          $$->target = ref;
      }

    | TOKEN_PRINT expr TOKEN_SEMICOLON
      {
          $$ = stmt_create(STMT_PRINT, 0, 0, $2, 0, 0, 0, 0);
      }

    | block
      {
          $$ = $1;
      }
;

decl
    : type TOKEN_IDENT TOKEN_ASSIGN expr TOKEN_SEMICOLON
      {
          $$ = decl_create($2, $1, $4, 0, 0);
      }

    | type TOKEN_IDENT TOKEN_LPAREN param_list_opt TOKEN_RPAREN block
      {
          struct type *ft = type_create(TYPE_FUNCTION, $1, $4);
          $$ = decl_create($2, ft, 0, $6, 0);
      }
;

param_list_opt
    : param_list
      {
          $$ = $1;
      }
    | /* empty */
      {
          $$ = 0;
      }
;

param_list
    : param
      {
          $$ = $1;
      }
    | param TOKEN_COMMA param_list
      {
          $$ = $1;
          $1->next = $3;
      }
;

param
    : type TOKEN_IDENT
      {
          $$ = param_list_create($2, $1, 0);
      }
;

block
    : TOKEN_LBRACE stmt_list TOKEN_RBRACE
      {
          $$ = stmt_create(STMT_BLOCK, 0, 0, 0, 0, $2, 0, 0);
      }
;

expr
    : expr TOKEN_PLUS term
      { $$ = expr_create(EXPR_ADD, $1, $3); }

    | expr TOKEN_MINUS term
      { $$ = expr_create(EXPR_SUB, $1, $3); }

    | term
      { $$ = $1; }
;

term
    : term TOKEN_MUL factor
      { $$ = expr_create(EXPR_MUL, $1, $3); }

    | term TOKEN_DIV factor
      { $$ = expr_create(EXPR_DIV, $1, $3); }

    | factor
      { $$ = $1; }
;

factor
    : TOKEN_NUMBER
      { $$ = expr_create_integer_literal((int)$1); }

    | TOKEN_FLOAT_NUMBER
      { $$ = expr_create_float_literal($1); }

    | TOKEN_IDENT
      { $$ = expr_create_name($1); }

    | TOKEN_MATRIX TOKEN_LBRACKET expr TOKEN_RBRACKET
      TOKEN_LBRACKET expr TOKEN_RBRACKET
      {
          $$ = expr_create_matrix_ref($3, $6);
      }

    | TOKEN_LPAREN expr TOKEN_RPAREN
      { $$ = $2; }
;

type
    : TOKEN_INT
      { $$ = type_create(TYPE_INTEGER, 0, 0); }

    | TOKEN_FLOAT
      { $$ = type_create(TYPE_FLOAT, 0, 0); }
;

%%

int yyerror(const char *s)
{
    printf("Parse error: %s\n", s);
    return 1;
}
