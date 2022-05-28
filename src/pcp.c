#include "include/pcp.h"
#include "include/lexer.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>

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

static struct stat s;
static int fd;

void pcp_compile_file(const char* filename)
{
    char* src = pcp_read_file_mmap(filename);
    pcp_compile(src);

    munmap(src, s.st_size);
    close(fd);
}

char* pcp_read_file_mmap(const char* filename)
{
    char *f;
    int size;
    fd = open(filename, O_RDONLY);

    int status = fstat(fd, &s);
    size = s.st_size;

    f = (char *) mmap (0, size, PROT_READ, MAP_PRIVATE, fd, 0);

    return f;
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
