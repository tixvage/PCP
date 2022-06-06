#include "include/lexer.h"
#include "include/token.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

lexer_t* init_lexer(const char* src) {
    lexer_t* lexer = calloc(1, sizeof(struct lexer_t));
    lexer->src_len = strlen(src);
    lexer->src = src;
    lexer->i = 0;
    lexer->c = src[lexer->i];

    return lexer;
}

void lexer_advance(lexer_t* lexer) {
    if (lexer->i < lexer->src_len && lexer->c != '\0') {
        lexer->i += 1;
        lexer->c = lexer->src[lexer->i];
    }
}

char lexer_peek(lexer_t* lexer, int offset) {
    return lexer->src[lexer->i + offset];
}

token_t lexer_advance_with(lexer_t* lexer, token_t token) {
    lexer_advance(lexer);
    return token;
}

token_t lexer_advance_current(lexer_t* lexer, int type) {
    const char value[] = {lexer->c, '\0'};

    static char buffer[1] = { 0 };
    strcpy(buffer, value);

    token_t token = init_token(buffer, type);
    lexer_advance(lexer);

    return token;
}

token_t lexer_advance_two(lexer_t* lexer, int type) {
    return lexer_advance_with(lexer, lexer_advance_with(lexer, init_token(KEYS[type], type)));
}

void lexer_skip_ws(lexer_t* lexer) {
    while (isspace(lexer->c)) lexer_advance(lexer);
}

token_t lexer_parse_id(lexer_t* lexer) {
    char* value = calloc(1, sizeof(char));

    while (isalpha(lexer->c) || isdigit(lexer->c) || lexer->c == '_') {
        value = realloc(value, (strlen(value) + 2) * sizeof(char));
        strcat(value, (char[]){lexer->c, 0});
        lexer_advance(lexer);
    }

    static char buffer[MAX_ID_LENGTH] = { 0 };
    strcpy(buffer, value);
    free(value);

    for (int i = KEYWORD_START + 1; i < COUNT_TOKEN; i++) {
        if (streql(KEYS[i], buffer)) return init_token(buffer, i);
    }

    return init_token(buffer, TOKEN_IDENTIFIER);
}

token_t lexer_parse_number(lexer_t* lexer) {
    char* value = calloc(1, sizeof(char));

    while (isdigit(lexer->c)) {
        value = realloc(value, (strlen(value) + 2) * sizeof(char));
        strcat(value, (char[]){lexer->c, 0});
        lexer_advance(lexer);
    }

    static char buffer[MAX_ID_LENGTH] = { 0 };
    strcpy(buffer, value);
    free(value);

    return init_token(buffer, TOKEN_INTEGER_LITERAL);
}

token_t lexer_parse_string(lexer_t* lexer) {
    char* value = calloc(1, sizeof(char));
    
    lexer_advance(lexer);
    bool escaped = false;
    char current_char = 0;

    while (lexer->c != '"' && !escaped) {
        current_char = lexer->c;

        if (lexer->c == '\\') {
            escaped = true;
            lexer_advance(lexer);
        }
        
        if (escaped) {
            if (lexer->c == 'n') current_char = '\n';
            else if (lexer->c == 't') current_char = '\t';
            else if (lexer-> c == '"') current_char = '\"';
            else {
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
    
    static char buffer[MAX_ID_LENGTH] = { 0 };
    strcpy(buffer, value);
    free(value);

    return init_token(buffer, TOKEN_STRING_LITERAL);
}

token_t lexer_next_token(lexer_t* lexer) {
    while (lexer->c != '\0') {
        lexer_skip_ws(lexer);
        if (isalpha(lexer->c) || lexer->c == '_') return lexer_parse_id(lexer);
        else if (isdigit(lexer->c)) return lexer_parse_number(lexer);
        else if (lexer->c == '\"') return lexer_parse_string(lexer);
        
        switch (lexer->c) {
            case '!': {
                if (lexer_peek(lexer, 1) == '=') return lexer_advance_two(lexer, TOKEN_BANG_EQUAL);
                return lexer_advance_current(lexer, TOKEN_BANG);
            } break;

            case ':': {
                if (lexer_peek(lexer, 1) == ':') return lexer_advance_two(lexer, TOKEN_COLON_COLON);
                else if (lexer_peek(lexer, 1) == '=') return lexer_advance_two(lexer, TOKEN_COLON_EQUAL);
                return lexer_advance_current(lexer, TOKEN_COLON);
            } break;

            case '+': {
                if (lexer_peek(lexer, 1) == '+') return lexer_advance_two(lexer, TOKEN_PLUS_PLUS);
                else if (lexer_peek(lexer, 1) == '=') return lexer_advance_two(lexer, TOKEN_PLUS_EQUAL);
                return lexer_advance_current(lexer, TOKEN_PLUS);
            } break;

            case '-': {
                if (lexer_peek(lexer, 1) == '-') return lexer_advance_two(lexer, TOKEN_MINUS_MINUS);
                else if (lexer_peek(lexer, 1) == '>') return lexer_advance_two(lexer, TOKEN_ARROW);
                else if (lexer_peek(lexer, 1) == '=') return lexer_advance_two(lexer, TOKEN_MINUS_EQUAL);
                return lexer_advance_current(lexer, TOKEN_MINUS);
            } break;

            case '=': {
                if (lexer_peek(lexer, 1) == '=') return lexer_advance_two(lexer, TOKEN_EQUAL_EQUAL);
                return lexer_advance_current(lexer, TOKEN_EQUAL);
            } break;

            case '(': {
                return lexer_advance_current(lexer, TOKEN_L_PAREN);
            } break;
            
            case ')': {
                return lexer_advance_current(lexer, TOKEN_R_PAREN);
            } break;

            case '{': {
                return lexer_advance_current(lexer, TOKEN_L_BRACE);
            } break;
            
            case '}': {
                return lexer_advance_current(lexer, TOKEN_R_BRACE);
            } break;

            case '[': {
                return lexer_advance_current(lexer, TOKEN_L_BRACKET);
            }

            case ']': {
                return lexer_advance_current(lexer, TOKEN_R_BRACKET);
            }

            case ',': {
                return lexer_advance_current(lexer, TOKEN_COMMA);
            } break;

            case ';': {
                return lexer_advance_current(lexer, TOKEN_SEMICOLON);
            } break;

            case '.': {
                return lexer_advance_current(lexer, TOKEN_DOT);
            } break;

            case '@': {
                return lexer_advance_current(lexer, TOKEN_AT);
            } break;
            
            case '\0': break;

            default: {
                printf("Unexpected character '%c'", lexer->c);
                exit(1);
            }
        }
    }

    return init_token(0, TOKEN_EOF);
}

bool streql(const char* first, const char* second) {
    return strcmp(first, second) == 0;
}
