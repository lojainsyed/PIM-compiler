#ifndef TOKENS_H
#define TOKENS_H

typedef enum {
    TOK_INT_KW = 256,
    TOK_ID,
    TOK_INT_LIT,

    TOK_MATRIX_ID,

    TOK_LBRACK,
    TOK_RBRACK,
    TOK_LPAREN,
    TOK_RPAREN,

    TOK_SEMI,
    TOK_COMMA,

    TOK_ASSIGN,

    TOK_PLUS,
    TOK_MINUS,
    TOK_STAR,

    TOK_EOF,
    TOK_ERROR
} token_t;

static inline const char* token_name(int t) {
    switch (t) {
        case TOK_INT_KW:    return "KW_INT";
        case TOK_ID:        return "ID";
        case TOK_INT_LIT:   return "INT_LIT";
        case TOK_MATRIX_ID: return "MATRIX_ID";
        case TOK_LBRACK:    return "LBRACK";
        case TOK_RBRACK:    return "RBRACK";
        case TOK_LPAREN:    return "LPAREN";
        case TOK_RPAREN:    return "RPAREN";
        case TOK_SEMI:      return "SEMI";
        case TOK_COMMA:     return "COMMA";
        case TOK_ASSIGN:    return "ASSIGN";
        case TOK_PLUS:      return "PLUS";
        case TOK_MINUS:     return "MINUS";
        case TOK_STAR:      return "STAR";
        case TOK_EOF:       return "EOF";
        case TOK_ERROR:     return "ERROR";
        default:            return "<UNKNOWN>";
    }
}

#endif
