#ifndef clox_scanner_h
#define clox_scanner_h

typedef enum
{
    TOKEN_EOF,
    TOKEN_ERROR,
    TOKEN_LEFT_PAREN,
    TOKEN_RIGHT_PAREN,
    TOKEN_LEFT_BRACE,
    TOKEN_RIGHT_BRACE,
    TOKEN_COMMA,
    TOKEN_DOT,
    TOKEN_MINUS,
    TOKEN_PLUS,
    TOKEN_SEMICOLON,
    TOKEN_SLASH,
    TOKEN_STAR,
} TokenType;

typedef struct
{
    TokenType type;
    const char *start;
    int length;
    int line;
} Token;

void initScanner(const char *source);
Token scanToken();

#endif