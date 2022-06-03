#include "include/lexer.h"
#include "include/token.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

lexer_t* init_lexer(const char* src)
{
    lexer_t* lexer = calloc(1, sizeof(struct lexer));
    lexer->src_len = strlen(src);
    lexer->src = src;
    lexer->i = 0;
    lexer->c = src[lexer->i];

    return lexer;
}

void lexer_advance(lexer_t* lexer)
{
    if (lexer->i < lexer->src_len && lexer->c != '\0') 
    {
        lexer->i += 1;
        lexer->c = lexer->src[lexer->i];
    }
}

char lexer_peek(lexer_t* lexer, int offset)
{
    return lexer->src[lexer->i + offset];
}

token_t lexer_advance_with(lexer_t* lexer, token_t token)
{
    lexer_advance(lexer);
    return token;
}

token_t lexer_advance_current(lexer_t* lexer, int type)
{
    const char value[2] = {lexer->c, '\0'};
    token_t token = init_token(value, type);
    lexer_advance(lexer);

    return token;
}

void lexer_skip_ws(lexer_t* lexer)
{
    while (isspace(lexer->c))
        lexer_advance(lexer);
}

token_t lexer_parse_id(lexer_t* lexer)
{
    char* value = calloc(1, sizeof(char));

    while (isalpha(lexer->c) || isdigit(lexer->c) || lexer->c == '_')
    {
        value = realloc(value, (strlen(value) + 2) * sizeof(char));
        strcat(value, (char[]){lexer->c, 0});
        lexer_advance(lexer);
    }

    char unfreed_value[strlen (value)];
    strcpy(unfreed_value, value);
    free(value);

    return init_token(unfreed_value, TOKEN_ID);
}

token_t lexer_parse_number(lexer_t* lexer)
{
    char* value = calloc(1, sizeof(char));

    while (isdigit(lexer->c))
    {
        value = realloc(value, (strlen(value) + 2) * sizeof(char));
        strcat(value, (char[]){lexer->c, 0});
        lexer_advance(lexer);
    }

    char unfreed_value[strlen (value)];
    strcpy(unfreed_value, value);
    free(value);

    return init_token(unfreed_value, TOKEN_INT);
}

token_t lexer_parse_string(lexer_t* lexer)
{
    char* value = calloc(1, sizeof(char));
    
    lexer_advance(lexer);
    bool escaped = false;
    char current_char = 0;

    while (lexer->c != '"' && !escaped)
    {
        current_char = lexer->c;

        if (lexer->c == '\\')
        {
            escaped = true;
            lexer_advance(lexer);
        }
        
        if (escaped)
        {
            if (lexer->c == 'n')
                current_char = '\n';
            else if (lexer->c == 't')
                current_char = '\t';
            else if (lexer-> c == '"')
                current_char = '\"';
            else
            {
                printf("Invalid escape character: '%c'\n", lexer->c);
                exit(1); // TODO: errors
            }
            
            escaped = false;
        }

        value = realloc(value, (strlen(value) + 2) * sizeof(char));
        strcat(value, (char[]){current_char, 0});
        lexer_advance(lexer);
    }

    lexer_advance(lexer);

    char unfreed_value[strlen (value)];
    strcpy(unfreed_value, value);
    free(value);
    
    return init_token(unfreed_value, TOKEN_STRING);
}

token_t lexer_next_token(lexer_t* lexer)
{
    while (lexer->c != '\0')
    {
        lexer_skip_ws(lexer);
        if (isalpha(lexer->c) || lexer->c == '_') //TODO: some special characters
            return lexer_parse_id(lexer);
        if (isdigit(lexer->c))
            return lexer_parse_number(lexer);
        if (lexer->c == '\"')
            return lexer_parse_string(lexer);
        
        switch(lexer->c)
        {
            case ':': {
                if (lexer_peek(lexer, 1) == ':')
                    return lexer_advance_with(lexer, lexer_advance_with(lexer, init_token("::", TOKEN_ASSIGN_CONST)));
                else if (lexer_peek(lexer, 1) == '=')
                    return lexer_advance_with(lexer, lexer_advance_with(lexer, init_token(":=", TOKEN_ASSIGN_VALUE)));

                return lexer_advance_with(lexer, init_token(":", TOKEN_ASSIGN_TYPE));
            } break;

            case '=': {
                if (lexer_peek(lexer, 1) == '>')
                    return lexer_advance_with(lexer, lexer_advance_with(lexer, init_token("=>", TOKEN_ARROW_RIGHT)));
                
                return lexer_advance_with(lexer, init_token("=", TOKEN_ASSIGN));
            } break;

            case '(': {
                return lexer_advance_current(lexer, TOKEN_LPAREN);
            } break;
            
            case ')': {
                return lexer_advance_current(lexer, TOKEN_RPAREN);
            } break;

            case '{': {
                return lexer_advance_current(lexer, TOKEN_LBRACE);
            } break;
            
            case '}': {
                return lexer_advance_current(lexer, TOKEN_RBRACE);
            } break;

            case ',': {
                return lexer_advance_current(lexer, TOKEN_COMMA);
            } break;

            case ';': {
                return lexer_advance_current(lexer, TOKEN_SEMI);
            } break;

            case '.': {
                return lexer_advance_current(lexer, TOKEN_DOT);
            } break;

            case '@': {
                return lexer_advance_current(lexer, TOKEN_AT);
            } break;
            
            case '\0': break;

            default: printf("Unexpected character '%c'", lexer->c); exit(1);
        }
    }

    return init_token(0, TOKEN_EOF);
}
