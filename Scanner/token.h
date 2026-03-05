#ifndef TOKEN_H
#define TOKEN_H

typedef enum {
    TOKEN_EOF = 0,

    TOKEN_INT,        /* "int" */
    TOKEN_MATRIX,     /* "M"   */

    TOKEN_IDENT,      /* variable names */
    TOKEN_NUMBER,     /* integer literals */

    TOKEN_ASSIGN,     /* = */
    TOKEN_PLUS,       /* + */
    TOKEN_MINUS,      /* - */
    TOKEN_MUL,        /* * */

    TOKEN_LBRACKET,   /* [ */
    TOKEN_RBRACKET,   /* ] */
    TOKEN_SEMICOLON,  /* ; */

    TOKEN_ERROR       /* anything invalid */
} token_t;

#endif
