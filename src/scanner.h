#ifndef clox_scanner_h
#define clox_scanner_h

typedef enum
{
    TOKEN_EOF,
    TOKEN_ERROR,
    TOKEN_LEFT_PAREN,
    TOKEN_RIGHT_PAREN,
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