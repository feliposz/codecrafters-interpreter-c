#include <stdlib.h>
#include "chunk.h"
#include "vm.h"
#include "util.h"

bool compile(const char *source, Chunk *chunk);

bool evaluate(const char *path)
{
    Chunk chunk;
    initChunk(&chunk);
    char *source = readFile(path);
    if (source == NULL)
    {
        return false;
    }
    bool hadError = true;
    initVM();
    if (compile(source, &chunk))
    {
        InterpretResult result = interpret(&chunk);
        hadError = result != INTERPRET_OK;
    }
    freeVM();
    free(source);
    freeChunk(&chunk);
    return hadError;
}