#include "include/pcp.h"
#include "include/parser.h"
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
    parser_t* parser = init_parser(lexer);
    expr_t expr_tree = parser_parse(parser);
    print_tree(&expr_tree, -3);

    free(lexer);
    free(parser);
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

