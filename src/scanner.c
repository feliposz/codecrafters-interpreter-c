#include <stdio.h>
#include <string.h>
#include "common.h"
#include "scanner.h"

typedef struct
{
    const char *start;
    const char *current;
    int line;
} Scanner;

Scanner scanner;

void initScanner(const char *source)
{
    scanner.start = source;
    scanner.current = source;
    scanner.line = 1;
}

Token makeToken(TokenType type)
{
    Token token;
    token.type = type;
    token.start = scanner.start;
    token.length = (int)(scanner.current - scanner.start);
    token.line = scanner.line;
    return token;
}

Token errorToken(const char *message)
{
    Token token;
    token.type = TOKEN_ERROR;
    token.start = message;
    token.length = strlen(message);
    token.line = scanner.line;
    return token;
}

char advance()
{
    return *scanner.current++;
}

bool isAtEnd()
{
    return *scanner.current == '\0';
}

bool match(char expected)
{
    if (isAtEnd())
    {
        return false;
    }
    if (*scanner.current != expected)
    {
        return false;
    }
    scanner.current++;
    return true;
}

char peek()
{
    return *scanner.current;
}

char peekNext()
{
    if (isAtEnd())
    {
        return '\0';
    }
    return scanner.current[1];
}

void skipWhitespace()
{
    for (;;)
    {
        switch (peek())
        {
        case ' ':
        case '\r':
        case '\t':
            advance();
            break;
        case '\n':
            scanner.line++;
            advance();
            break;
        case '/':
            if (peekNext() == '/')
            {
                while (peek() != '\n' && !isAtEnd())
                {
                    advance();
                }
            }
            else
            {
                return;
            }
            break;
        default:
            return;
        }
    }
}

Token string()
{
    while (peek() != '"' && !isAtEnd())
    {
        if (peek() == '\n')
        {
            scanner.line++;
        }
        advance();
    }
    if (isAtEnd())
    {
        fprintf(stderr, "[line %d] Error: Unterminated string.\n", scanner.line);
        return errorToken("Unterminated string.");
    }
    advance();
    return makeToken(TOKEN_STRING);
}

bool isDigit(char c)
{
    return c >= '0' && c <= '9';
}

Token number()
{
    while (isDigit(peek()))
    {
        advance();
    }
    if (peek() == '.' && isDigit(peekNext()))
    {
        advance();
        while (isDigit(peek()))
        {
            advance();
        }
    }
    return makeToken(TOKEN_NUMBER);
}

bool isAlpha(char c)
{
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_';
}

Token identifier()
{
    while (isAlpha(peek()) || isDigit(peek()))
    {
        advance();
    }
    return makeToken(TOKEN_IDENTIFIER);
}
Token scanToken()
{
    skipWhitespace();
    scanner.start = scanner.current;
    if (isAtEnd())
    {
        return makeToken(TOKEN_EOF);
    }
    char c = advance();
    if (isAlpha(c))
    {
        return identifier();
    }
    if (isDigit(c))
    {
        return number();
    }
    switch (c)
    {
    case '(':
        return makeToken(TOKEN_LEFT_PAREN);
    case ')':
        return makeToken(TOKEN_RIGHT_PAREN);
    case '{':
        return makeToken(TOKEN_LEFT_BRACE);
    case '}':
        return makeToken(TOKEN_RIGHT_BRACE);
    case ',':
        return makeToken(TOKEN_COMMA);
    case '.':
        return makeToken(TOKEN_DOT);
    case '-':
        return makeToken(TOKEN_MINUS);
    case '+':
        return makeToken(TOKEN_PLUS);
    case ';':
        return makeToken(TOKEN_SEMICOLON);
    case '/':
        return makeToken(TOKEN_SLASH);
    case '*':
        return makeToken(TOKEN_STAR);
    case '=':
        return makeToken(match('=') ? TOKEN_EQUAL_EQUAL : TOKEN_EQUAL);
    case '!':
        return makeToken(match('=') ? TOKEN_BANG_EQUAL : TOKEN_BANG);
    case '<':
        return makeToken(match('=') ? TOKEN_LESS_EQUAL : TOKEN_LESS);
    case '>':
        return makeToken(match('=') ? TOKEN_GREATER_EQUAL : TOKEN_GREATER);
    case '"':
        return string();
    }
    fprintf(stderr, "[line %d] Error: Unexpected character: %c\n", scanner.line, c);
    return errorToken("Unexpected character");
}
