#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "tokenizer.h"

bool evaluate(const char *source);
void testChunk();
void testVM();

int main(int argc, char *argv[])
{
    // Disable output buffering
    setbuf(stdout, NULL);
    setbuf(stderr, NULL);

    if (argc < 2)
    {
        fprintf(stderr, "Usage: ./your_program <mode> [<filename>]\n");
        return 1;
    }

    const char *command = argv[1];

    if (strcmp(command, "tokenize") == 0)
    {
        bool hadError = tokenizer(argv[2]);
        if (hadError)
        {
            exit(65);
        }
    }
    else if (strcmp(command, "evaluate") == 0)
    {
        bool hadError = evaluate(argv[2]);
        if (hadError)
        {
            exit(65);
        }
    }
    else if (strcmp(command, "testchunk") == 0)
    {
        testChunk();
    }
    else if (strcmp(command, "testvm") == 0)
    {
        testVM();
    }
    else
    {
        fprintf(stderr, "Unknown command: %s\n", command);
        return 1;
    }

    return 0;
}
