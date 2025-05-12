#include <stdio.h>
#include <stdlib.h>
#include "common.h"
#include "util.h"
#include "scanner.h"

typedef struct
{
    Token previous;
    Token current;
    bool hadError;
    bool panicMode;
} Parser;

Parser parser;

static void advance()
{
    parser.previous = parser.current;
    parser.current = scanToken();
    // TODO: handle TOKEN_ERROR
}

static void errorAt(Token *token, const char *message)
{
    if (parser.panicMode)
    {
        return;
    }
    parser.panicMode = true;
    fprintf(stderr, "[line %d] Error", token->line);
    if (token->type == TOKEN_EOF)
    {
        fprintf(stderr, " at end");
    }
    else if (token->type == TOKEN_ERROR)
    {
        // nothing
    }
    else
    {
        fprintf(stderr, " at '%.*s'", token->length, token->start);
    }
    fprintf(stderr, ": %s\n", message);
    parser.hadError = true;
}

static void errorAtCurrent(const char *message)
{
    errorAt(&parser.current, message);
}

static void error(const char *message)
{
    errorAt(&parser.previous, message);
}

static void consume(TokenType type, const char *message)
{
    if (parser.current.type != type)
    {
        errorAtCurrent(message);
        return;
    }
    advance();
}

static void expression()
{
    printf("%.*s\n", parser.current.length, parser.current.start);
    advance();
}

bool compile(const char *source)
{
    parser.hadError = false;
    parser.panicMode = false;
    initScanner(source);
    advance();
    expression();
    consume(TOKEN_EOF, "Expect end of expression.");
    return !parser.hadError;
}

bool evaluate(const char *path)
{
    char *source = readFile(path);
    if (source == NULL) {
        return false;
    }
    bool success = compile(source);
    free(source);
    return !success;
}