#ifndef TOKEN_H
#define TOKEN_H

typedef struct token
{
    const char* value;
    enum
    {
        TOKEN_ID,
        TOKEN_LPAREN,
        TOKEN_RPAREN,
        TOKEN_LBRACE,
        TOKEN_RBRACE,
        TOKEN_ARROW_RIGHT,
        TOKEN_ASSIGN_CONST, // ::
        TOKEN_ASSIGN_TYPE, // :
        TOKEN_ASSIGN_VALUE, // :=
        TOKEN_ASSIGN, // =
        TOKEN_QUOTE,
        TOKEN_STRING,
        TOKEN_COMMA,
        TOKEN_DOT,
        TOKEN_AT,
        TOKEN_SEMI,
        TOKEN_INT,
        TOKEN_EOF,
    } type;
} token_t;

token_t init_token(const char* value, int type);
const char* token_type_to_str(token_t token);

#endif // !TOKEN_H
