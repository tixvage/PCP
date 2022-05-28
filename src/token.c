#include "include/token.h"
#include <stdlib.h>

token_t init_token(const char* value, int type)
{
    token_t token = { 0 };
    token.value = value;
    token.type = type;

    return token;
}

const char* token_type_to_str(token_t token)
{
    switch (token.type){
        case TOKEN_ID: return "ID"; break;
        case TOKEN_LPAREN: return "LPAREN"; break;
        case TOKEN_RPAREN: return "RPAREN"; break;
        case TOKEN_LBRACE: return "LBRACE"; break;
        case TOKEN_RBRACE: return "RBRACE"; break;
        case TOKEN_ARROW_RIGHT: return "ARROW_RIGHT"; break;
        case TOKEN_ASSIGN_CONST: return "ASSIGN_CONST"; break;
        case TOKEN_ASSIGN_TYPE: return "ASSIGN_TYPE"; break;
        case TOKEN_ASSIGN_VALUE: return "ASSIGN_VALUE"; break;
        case TOKEN_ASSIGN: return "ASSIGN"; break;
        case TOKEN_QUOTE: return "QUOTE"; break;
        case TOKEN_STRING: return "STRING"; break;
        case TOKEN_COMMA: return "COMMA"; break;
        case TOKEN_DOT: return "DOT"; break;
        case TOKEN_AT: return "AT"; break;
        case TOKEN_SEMI: return "SEMI"; break;
        case TOKEN_INT: return "INT"; break;
        case TOKEN_EOF: return "EOF"; break;
        default: return "UNDEFINED"; break;
    }
}
