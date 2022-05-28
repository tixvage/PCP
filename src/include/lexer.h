#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include "token.h"

typedef struct lexer
{
    const char* src;
    size_t src_len;
    unsigned int i;
    char c;
} lexer_t;

lexer_t* init_lexer(const char* src);
void lexer_advance(lexer_t* lexer);
char lexer_peek(lexer_t* lexer, int offset);
token_t lexer_advance_with(lexer_t* lexer, token_t token);
token_t lexer_advance_current(lexer_t* lexer, int type);
void lexer_skip_ws(lexer_t* lexer);
token_t lexer_parse_id(lexer_t* lexer);
token_t lexer_parse_number(lexer_t* lexer);
token_t lexer_next_token(lexer_t* lexer);

#endif // !LEXER_H
