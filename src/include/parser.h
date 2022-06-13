#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"

typedef struct expr_t epxr_t;
typedef struct binary_op_t binary_op_t;
typedef struct unary_op_t unary_op_t;
typedef struct compound_t compound_t;
typedef struct assign_t assign_t;
typedef struct no_op_t {} no_op_t;
typedef struct var_t var_t;

typedef struct parser_t {
    lexer_t* lexer;
    token_t current_token;
} parser_t;

typedef struct expr_t {
    enum {
        EXPR_INVALID,
        EXPR_LIT_STR,
        EXPR_LIT_INT,
        EXPR_BINARY_OP,
        EXPR_UNARY_OP,
        EXPR_COMPOUND,
        EXPR_ASSIGN,
        EXPR_VAR,
        EXPR_NO_OP,
    } kind;
    union {
        const char* lit_str;
        int lit_int;
        binary_op_t* binary_op;
        unary_op_t* unary_op;
        compound_t* compound;
        assign_t* assign;
        var_t* var;
        no_op_t no_op;
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

typedef struct compound_list_t {
  expr_t* items;
  size_t size;
} compound_list_t;

compound_list_t init_list(void);
void list_push(compound_list_t* list, expr_t item);

typedef struct compound_t {
    compound_list_t childs;
} compound_t;

compound_t* init_compound_t(void);

typedef struct assign_t {
    expr_t left;
    expr_t right;
    token_t op;
} assign_t;

assign_t* init_assign_t(expr_t left, expr_t right, token_t op);

typedef struct var_t {
    token_t token;
    const char* value;
} var_t;

var_t* init_var_t(token_t token);

parser_t* init_parser(lexer_t* lexer);
void parser_eat(parser_t* parser, int token_type);
void parser_error(int expected_token, int gotten_token);
expr_t parser_factor(parser_t* parser);
expr_t parser_term(parser_t* parser);
expr_t parser_expr(parser_t* parser);
expr_t parser_variable(parser_t* parser);
expr_t parser_assignment_statement(parser_t* parser);
expr_t parser_statement(parser_t* parser);
compound_list_t parser_statement_list(parser_t* parser);
expr_t parser_compound_statement(parser_t* parser);
expr_t parser_program(parser_t* parser);
expr_t parser_parse(parser_t* parser);

#endif // !PARSER_H

