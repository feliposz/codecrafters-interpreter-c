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
    default:
        return;
    }
    printf("%s %.*s %s\n", desc, token.length, token.start, value);
}

void tokenizer(const char *path)
{
    char *source = readFile(path);
    initScanner(source);
    int line = -1;
    for (;;)
    {
        Token token = scanToken();
        printToken(token);
        if (token.type == TOKEN_EOF)
        {
            break;
        }
    }
    free(source);
}