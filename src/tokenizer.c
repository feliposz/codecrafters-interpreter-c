#include <stdio.h>
#include <stdlib.h>
#include "common.h"
#include "util.h"
#include "scanner.h"
#include "tokenizer.h"

void printToken(Token token)
{
    char *desc = "???";
    char *value = "null";
    switch (token.type)
    {
    case TOKEN_LEFT_PAREN:
        desc = "LEFT_PAREN";
        break;
    case TOKEN_RIGHT_PAREN:
        desc = "RIGHT_PAREN";
        break;
    case TOKEN_LEFT_BRACE:
        desc = "LEFT_BRACE";
        break;
    case TOKEN_RIGHT_BRACE:
        desc = "RIGHT_BRACE";
        break;
    case TOKEN_EOF:
        desc = "EOF";
        break;
    case TOKEN_COMMA:
        desc = "COMMA";
        break;
    case TOKEN_DOT:
        desc = "DOT";
        break;
    case TOKEN_MINUS:
        desc = "MINUS";
        break;
    case TOKEN_PLUS:
        desc = "PLUS";
        break;
    case TOKEN_SEMICOLON:
        desc = "SEMICOLON";
        break;
    case TOKEN_STAR:
        desc = "STAR";
        break;
    case TOKEN_EQUAL:
        desc = "EQUAL";
        break;
    case TOKEN_EQUAL_EQUAL:
        desc = "EQUAL_EQUAL";
        break;
    default:
        return;
    }
    printf("%s %.*s %s\n", desc, token.length, token.start, value);
}

bool tokenizer(const char *path)
{
    bool hadError = false;
    char *source = readFile(path);
    initScanner(source);
    int line = -1;
    for (;;)
    {
        Token token = scanToken();
        printToken(token);
        if (token.type == TOKEN_ERROR)
        {
            hadError = true;
        }
        if (token.type == TOKEN_EOF)
        {
            break;
        }
    }
    free(source);
    return hadError;
}