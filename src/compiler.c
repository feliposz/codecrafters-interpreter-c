#include <stdio.h>
#include <stdlib.h>
#include "common.h"
#include "util.h"
#include "scanner.h"

static void expression();

typedef struct
{
    Token previous;
    Token current;
    bool hadError;
    bool panicMode;
} Parser;

Parser parser;

typedef void (*ParseFn)();

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

static void literal()
{
    printf("%.*s\n", parser.previous.length, parser.previous.start);
}

static void string()
{
    printf("%.*s\n", parser.previous.length - 2, parser.previous.start + 1);
}

static void number()
{
    double value = strtod(parser.previous.start, NULL);
    printf("%g\n", value);
}

static void grouping()
{
    expression();
    consume(TOKEN_RIGHT_PAREN, "Expect ')' after expression.");
}

typedef enum
{
    PREC_NONE,
} Precedence;

typedef struct
{
    ParseFn prefix;
    ParseFn infix;
    Precedence precedence;
} ParseRule;

ParseRule rules[] = {
    [TOKEN_LEFT_PAREN] = {grouping, NULL, PREC_NONE},
    [TOKEN_STRING] = {string, NULL, PREC_NONE},
    [TOKEN_NUMBER] = {number, NULL, PREC_NONE},
    [TOKEN_FALSE] = {literal, NULL, PREC_NONE},
    [TOKEN_NIL] = {literal, NULL, PREC_NONE},
    [TOKEN_TRUE] = {literal, NULL, PREC_NONE},
};

static ParseRule *getRule(TokenType type)
{
    return &rules[type];
}

static void parsePrecedence(Precedence precedence)
{
    advance();
    ParseFn prefixRule = getRule(parser.previous.type)->prefix;
    if (prefixRule == NULL)
    {
        error("Expect expression");
        return;
    }
    prefixRule();
}

static void expression()
{
    parsePrecedence(PREC_NONE);
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
    if (source == NULL)
    {
        return false;
    }
    bool success = compile(source);
    free(source);
    return !success;
}