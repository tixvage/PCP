#include "include/token.h"
#include <stdlib.h>

token_t init_token(const char* value, int type) {
    token_t token = { 0 };
    token.value = value;
    token.type = type;

    return token;
}

const char* token_to_str(token_t token) {
    return token_type_to_str(token.type);
}

const char* token_type_to_str(int token_type) {
    switch (token_type) {
        TOKEN_STRING_CASE(INVALID);
        TOKEN_STRING_CASE(IDENTIFIER);
        TOKEN_STRING_CASE(STRING_LITERAL);
        TOKEN_STRING_CASE(CHAR_LITERAL);
        TOKEN_STRING_CASE(INTEGER_LITERAL);
        TOKEN_STRING_CASE(FLOAT_LITERAL);
        TOKEN_STRING_CASE(EOF);
        TOKEN_STRING_CASE(BUILTIN);
        TOKEN_STRING_CASE(BANG);
        TOKEN_STRING_CASE(EQUAL);
        TOKEN_STRING_CASE(EQUAL_EQUAL);
        TOKEN_STRING_CASE(BANG_EQUAL);
        TOKEN_STRING_CASE(L_PAREN);
        TOKEN_STRING_CASE(R_PAREN);
        TOKEN_STRING_CASE(SEMICOLON);
        TOKEN_STRING_CASE(DOT);
        TOKEN_STRING_CASE(AT);
        TOKEN_STRING_CASE(L_BRACE);
        TOKEN_STRING_CASE(R_BRACE);
        TOKEN_STRING_CASE(L_BRACKET);
        TOKEN_STRING_CASE(R_BRACKET);
        TOKEN_STRING_CASE(PLUS);
        TOKEN_STRING_CASE(PLUS_EQUAL);
        TOKEN_STRING_CASE(MINUS);
        TOKEN_STRING_CASE(MINUS_EQUAL);
        TOKEN_STRING_CASE(MINUS_PERCENT);
        TOKEN_STRING_CASE(ASTERISK);
        TOKEN_STRING_CASE(ASTERISK_EQUAL);
        TOKEN_STRING_CASE(ARROW);
        TOKEN_STRING_CASE(COLON);
        TOKEN_STRING_CASE(COLON_EQUAL);
        TOKEN_STRING_CASE(COLON_COLON);
        TOKEN_STRING_CASE(SLASH);
        TOKEN_STRING_CASE(SLASH_EQUAL);
        TOKEN_STRING_CASE(COMMA);
        TOKEN_STRING_CASE(QUESTION_MARK);
        TOKEN_STRING_CASE(ANGLE_BRACKET_LEFT);
        TOKEN_STRING_CASE(ANGLE_BRACKET_LEFT_EQUAL);
        TOKEN_STRING_CASE(ANGLE_BRACKET_RIGHT);
        TOKEN_STRING_CASE(ANGLE_BRACKET_RIGHT_EQUAL);
        TOKEN_STRING_CASE(KEYWORD_AND);
        TOKEN_STRING_CASE(KEYWORD_ASM);
        TOKEN_STRING_CASE(KEYWORD_BREAK);
        TOKEN_STRING_CASE(KEYWORD_CONTINUE);
        TOKEN_STRING_CASE(KEYWORD_DEFER);
        TOKEN_STRING_CASE(KEYWORD_ELSE);
        TOKEN_STRING_CASE(KEYWORD_ENUM);
        TOKEN_STRING_CASE(KEYWORD_EXPORT);
        TOKEN_STRING_CASE(KEYWORD_EXTERN);
        TOKEN_STRING_CASE(KEYWORD_FN);
        TOKEN_STRING_CASE(KEYWORD_FOR);
        TOKEN_STRING_CASE(KEYWORD_IF);
        TOKEN_STRING_CASE(KEYWORD_INLINE);
        TOKEN_STRING_CASE(KEYWORD_OR);
        TOKEN_STRING_CASE(KEYWORD_RETURN);
        TOKEN_STRING_CASE(KEYWORD_STRUCT);
        TOKEN_STRING_CASE(KEYWORD_SWITCH);
        TOKEN_STRING_CASE(KEYWORD_WHILE);
        default: return "INVALID";
    }
}
