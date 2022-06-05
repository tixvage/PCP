#ifndef TOKEN_H
#define TOKEN_H

#define TOKEN_STRING_CASE(token) case TOKEN_##token: return ""#token""; break

typedef struct token_t {
    const char* value;
    enum {
        TOKEN_INVALID,
        TOKEN_IDENTIFIER,
        TOKEN_STRING_LITERAL,
        TOKEN_CHAR_LITERAL,
        TOKEN_EOF,
        TOKEN_BUILTIN,
        TOKEN_BANG,
        TOKEN_EQUAL,
        TOKEN_EQUAL_EQUAL,
        TOKEN_BANG_EQUAL,
        TOKEN_L_PAREN,
        TOKEN_R_PAREN,
        TOKEN_SEMICOLON,
        TOKEN_DOT,
        TOKEN_AT,
        TOKEN_L_BRACE,
        TOKEN_R_BRACE,
        TOKEN_L_BRACKET,
        TOKEN_R_BRACKET,
        TOKEN_PLUS,
        TOKEN_PLUS_PLUS,
        TOKEN_PLUS_EQUAL,
        TOKEN_MINUS,
        TOKEN_MINUS_EQUAL,
        TOKEN_MINUS_PERCENT,
        TOKEN_ASTERISK,
        TOKEN_ASTERISK_EQUAL,
        TOKEN_ARROW,
        TOKEN_COLON,
        TOKEN_COLON_EQUAL,
        TOKEN_COLON_COLON,
        TOKEN_SLASH,
        TOKEN_SLASH_EQUAL,
        TOKEN_COMMA,
        TOKEN_QUESTION_MARK,
        TOKEN_ANGLE_BRACKET_LEFT,
        TOKEN_ANGLE_BRACKET_LEFT_EQUAL,
        TOKEN_ANGLE_BRACKET_RIGHT,
        TOKEN_ANGLE_BRACKET_RIGHT_EQUAL,
        TOKEN_INTEGER_LITERAL,
        TOKEN_FLOAT_LITERAL,
        TOKEN_KEYWORD_AND,
        TOKEN_KEYWORD_ASM,
        TOKEN_KEYWORD_BREAK,
        TOKEN_KEYWORD_CONTINUE,
        TOKEN_KEYWORD_DEFER,
        TOKEN_KEYWORD_ELSE,
        TOKEN_KEYWORD_ENUM,
        TOKEN_KEYWORD_EXPORT,
        TOKEN_KEYWORD_EXTERN,
        TOKEN_KEYWORD_FN,
        TOKEN_KEYWORD_FOR,
        TOKEN_KEYWORD_IF,
        TOKEN_KEYWORD_INLINE,
        TOKEN_KEYWORD_OR,
        TOKEN_KEYWORD_RETURN,
        TOKEN_KEYWORD_STRUCT,
        TOKEN_KEYWORD_SWITCH,
        TOKEN_KEYWORD_WHILE,
    } type;
} token_t;

token_t init_token(const char* value, int type);
const char* token_type_to_str(token_t token);

#endif // !TOKEN_H
