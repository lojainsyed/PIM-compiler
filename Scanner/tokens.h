#ifndef TOKENS_H
#define TOKENS_H

typedef enum {
    KW_INT = 256,
    KW_MATRIX,

    ID,
    INT_LITERAL,

    ASSIGN,
    PLUS,
    MINUS,
    MUL,

    LBRACKET,
    RBRACKET,
    SEMICOLON,

    EOF_TOK,
    ERROR_TOK
} token_t;

static inline const char* token_name(int t) {
    switch (t) {
        case KW_INT:      return "KW_INT";
        case KW_MATRIX:   return "KW_MATRIX";
        case ID:          return "ID";
        case INT_LITERAL: return "INT_LITERAL";
        case ASSIGN:      return "ASSIGN";
        case PLUS:        return "PLUS";
        case MINUS:       return "MINUS";
        case MUL:         return "MUL";
        case LBRACKET:    return "LBRACKET";
        case RBRACKET:    return "RBRACKET";
        case SEMICOLON:   return "SEMICOLON";
        case EOF_TOK:     return "EOF";
        case ERROR_TOK:   return "ERROR";
        default:          return "UNKNOWN";
    }
}

#endif
