#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"

typedef struct expr_t epxr_t;
typedef struct binary_op_t binary_op_t;
typedef struct unary_op_t unary_op_t;

typedef struct expr_t {
    enum {
        EXPR_INVALID,
        EXPR_LIT_STR,
        EXPR_LIT_INT,
        EXPR_BINARY_OP,
        EXPR_UNARY_OP,
    } kind;
    union {
        const char* lit_str;
        int lit_int;
        binary_op_t* binary_op;
        unary_op_t* unary_op;
    } as;
} expr_t;

void print_tree(expr_t* expr, int level);

typedef struct binary_op_t {
    token_t op;
    expr_t rhs;
    expr_t lhs;
} binary_op_t;

binary_op_t* init_binary_op_t(token_t op, expr_t rhs, expr_t lhs);

typedef struct unary_op_t {
    token_t op;
    expr_t expr;
} unary_op_t;

unary_op_t* init_unary_op_t(token_t op, expr_t expr);

typedef struct parser_t {
    lexer_t* lexer;
    token_t current_token;
} parser_t;

parser_t* init_parser(lexer_t* lexer);
void parser_eat(parser_t* parser, int token_type);
void parser_error(int expected_token, int gotten_token);
expr_t parser_factor(parser_t* parser);
expr_t parser_term(parser_t* parser);
expr_t parser_expr(parser_t* parser);
expr_t parser_parse(parser_t* parser);

#endif // !PARSER_H

