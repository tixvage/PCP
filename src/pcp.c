#include "include/pcp.h"
#include "include/lexer.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <assert.h>

void pcp_compile(char* src) {
    lexer_t* lexer = init_lexer(src);
    token_t tok;

    while ((tok = lexer_next_token(lexer)).type != TOKEN_EOF) {
        printf("Token value : %s\nToken type : %s\n\n", tok.value, token_type_to_str(tok));
    }

    free(lexer);
}

static struct stat s;
static int fd;

void pcp_compile_file(const char* filename) {
    char* src = pcp_read_file_mmap(filename);
    pcp_compile(src);

    munmap(src, s.st_size);
    close(fd);
}

char* pcp_read_file_mmap(const char* filename) {
    char *f;
    int size;
    fd = open(filename, O_RDONLY);

    fstat(fd, &s);
    size = s.st_size;

    f = (char *) mmap (0, size, PROT_READ, MAP_PRIVATE, fd, 0);

    return f;
}

