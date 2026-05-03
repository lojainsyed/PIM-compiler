#include <stdio.h>
#include <stdlib.h>


/********************************
	  TYPES
********************************/

typedef enum {
	TYPE_VOID,
	TYPE_BOOLEAN,
	TYPE_CHARACTER,
	TYPE_INTEGER,
	TYPE_STRING,
	TYPE_ARRAY,
	TYPE_FUNCTION
} type_t;



struct type {
	type_t kind;
	struct type *subtype;
	struct param_list *params;
};


struct param_list {
	char *name;
	struct type *type;
	struct param_list *next;
};




/********************************
	EXPRESSIONS
********************************/

typedef enum {
        EXPR_ADD,
        EXPR_SUB,
        EXPR_MUL,
        EXPR_DIV,
        EXPR_NAME,
        EXPR_INTEGER_LITERAL,
        EXPR_STRING_LITERAL
} expr_t;


struct expr {
        expr_t kind;
        struct expr *left;
        struct expr *right;
        const char *name;
        int integer_value;
        const char *string_literal;
};



struct expr * expr_create(expr_t kind, struct expr *left, struct expr *right )
{
	struct expr *e = malloc(sizeof(*e));
	e->kind = kind;
	e->left = left;
	e->right = right;
        e->name = '\0';
        e->integer_value = 0;
        e->string_literal = '\0';
	return e;
}

struct expr * expr_create_name( const char *n )
{
	struct expr *e = expr_create(EXPR_NAME,0,0);
	e->name = n;
	return e;
}

struct expr * expr_create_integer_literal( int i )
{
	struct expr *e = expr_create(EXPR_INTEGER_LITERAL,0,0);
	e->integer_value = i;
	return e;
}

struct expr * expr_create_string_literal( const char *str )
{
	struct expr *e = expr_create(EXPR_STRING_LITERAL,0,0);
	e->string_literal = str;
	return e;
}



// struct expr * expr_create_boolean_literal( int b );
// struct expr * expr_create_char_literal( char c );



void expr_print( struct expr *e )
{
	if(!e) return;
		printf("(");
	
	expr_print(e->left);
	switch(e->kind) {
		case EXPR_INTEGER_LITERAL: 	printf("%d",e->integer_value); break;
		case EXPR_ADD: 		printf("+"); break;
		case EXPR_SUB: printf("-"); break;
		case EXPR_MUL: printf("*"); break;
		case EXPR_DIV: 	printf("/"); break;
	}
	expr_print(e->right);
	printf(")");
}



/********************************
	DECLARATIONS
********************************/

struct decl {
	char *name;
	struct type *type;
	struct expr *value;
	struct stmt *code;
	struct decl *next;
};

struct decl * decl_create( char *name,
                           struct type *type,
                           struct expr *value,
                           struct stmt *code,
                           struct decl *next )
{
	struct decl *d = malloc(sizeof(*d));
	d->name = name;
	d->type = type;
	d->value = value;
	d->code = code;
	d->next = next;
	return d;
}


/********************************
	STATEMENTS
********************************/

typedef enum {
	STMT_DECL,
	STMT_EXPR,
	STMT_IF_ELSE,
	STMT_FOR,
	STMT_PRINT,
	STMT_RETURN,
	STMT_BLOCK
} stmt_t;


struct stmt {
	stmt_t kind;
	struct decl *decl;
	struct expr *init_expr;
	struct expr *expr;
	struct expr *next_expr;
	struct stmt *body;
	struct stmt *else_body;
	struct stmt *next;
};





/* struct stmt * stmt_create( stmt_t kind,
                           struct decl *decl, struct expr *init_expr,
                           struct expr *expr, struct expr *next_expr,
                           struct stmt *body, struct stmt *else_body,
                           struct stmt *next ); */


