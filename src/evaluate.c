#include <stdlib.h>
#include "chunk.h"
#include "vm.h"
#include "util.h"

bool compile(const char *source, Chunk *chunk);

int evaluate(const char *path)
{
    Chunk chunk;
    initChunk(&chunk);
    char *source = readFile(path);
    if (source == NULL)
    {
        return false;
    }
    int errorCode = 0;
    initVM();
    if (compile(source, &chunk))
    {
        InterpretResult result = interpret(&chunk);
        if (result == INTERPRET_RUNTIME_ERROR)
        {
            errorCode = 70;
        }
    }
    else
    {
        errorCode = 65;
    }
    freeVM();
    free(source);
    freeChunk(&chunk);
    return errorCode;
}