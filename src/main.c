#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "tokenizer.h"

int main(int argc, char *argv[])
{
    // Disable output buffering
    setbuf(stdout, NULL);
    setbuf(stderr, NULL);

    if (argc < 3)
    {
        fprintf(stderr, "Usage: ./your_program tokenize <filename>\n");
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
    else
    {
        fprintf(stderr, "Unknown command: %s\n", command);
        return 1;
    }

    return 0;
}
