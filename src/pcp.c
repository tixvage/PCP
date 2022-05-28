#include "include/pcp.h"
#include "include/lexer.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <sys/types.h>

void pcp_compile(char* src)
{
    lexer_t* lexer = init_lexer(src);
    token_t tok;

    while ((tok = lexer_next_token(lexer)).type != TOKEN_EOF)
    {
        printf("Token value : %s\nToken type : %s\n\n", tok.value, token_type_to_str(tok));
    }

    free(lexer);
}

void pcp_compile_file(const char* filename)
{
    char* src = pcp_read_file(filename);
    pcp_compile(src);
    free(src);
}

char* pcp_read_file(const char* filename)
{
    FILE* fp;
    char* line = NULL;
    size_t len = 0;
    ssize_t read;

    fp = fopen(filename, "rb");
    if (fp == NULL)
    {
        printf("Couldn't read file '%s'", filename);
        exit(1);
    }

    char* buffer = calloc(1, sizeof(char));
    buffer[0] = '\0';
    
    while ((read = getline(&line, &len, fp)) != -1)
    {
        buffer = realloc(buffer, (strlen(buffer) + strlen(line) + 1) * sizeof(char));
        strcat(buffer, line);
    }

    fclose(fp);
    if (line) free(line);
    
    return buffer;
}
