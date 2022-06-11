#include "include/parser.h"
#include "stdlib.h"
#include <assert.h>

void print_ws(int level) {
    for (int i = 0; i < level; i++) printf(" ");
}

void print_tree(expr_t* expr, int level) {
    switch (expr->kind) {
        case EXPR_LIT_STR: {
            printf("Not implemented yet\n");
        } break;
        case EXPR_LIT_INT: {
            print_ws(level+1);
            printf("integer literal -> %d\n", expr->as.lit_int);
        } break;
        case EXPR_BINARY_OP: {
            binary_op_t* binary_op = expr->as.binary_op;
            print_ws(level+1);
            printf("token -> %s\n", token_to_str(binary_op->token));
            print_ws(level+1);
            print_tree(&binary_op->rhs, level+1);
            print_ws(level+1);
            print_tree(&binary_op->lhs, level+1);
        } break;
        default: printf("NULL\n"); break;
    }
}

binary_op_t* init_binary_op_t(token_t token, expr_t rhs, expr_t lhs) {
    binary_op_t* binary_op = calloc(1, sizeof(binary_op_t));
    binary_op->token = token;
    binary_op->rhs = rhs;
    binary_op->lhs = lhs;

    return binary_op;
}

parser_t* init_parser(lexer_t* lexer) {
    parser_t* parser = calloc(1, sizeof(struct parser_t));
    parser->lexer = lexer;
    parser->current_token = lexer_next_token(lexer);
    return parser;
}

void parser_eat(parser_t* parser, int token_type) {
    if (parser->current_token.type == token_type) parser->current_token = lexer_next_token(parser->lexer);
    else parser_error(token_type, parser->current_token.type);
}

void parser_error(int expected_token, int gotten_token) {
    printf("expected %s got %s\n", token_type_to_str(expected_token), token_type_to_str(gotten_token));
}

expr_t parser_factor(parser_t* parser) {
    token_t token = parser->current_token;
    if (token.type == TOKEN_INTEGER_LITERAL) {
        parser_eat(parser, TOKEN_INTEGER_LITERAL);
        return (expr_t) {.as = {.lit_int = atoi(token.value)}, .kind = EXPR_LIT_INT};
    }

    else if (token.type == TOKEN_L_PAREN) {
        parser_eat(parser, TOKEN_L_PAREN);
        expr_t node = parser_expr(parser);
        parser_eat(parser, TOKEN_R_PAREN);
        return node;
    }

    assert(0 && "parser_factor error");
}

expr_t parser_term(parser_t* parser) {
    expr_t node = parser_factor(parser);

    while (parser->current_token.type == TOKEN_ASTERISK || parser->current_token.type == TOKEN_SLASH) {
        token_t token = parser->current_token;
        if (token.type == TOKEN_ASTERISK) parser_eat(parser, TOKEN_ASTERISK);
        else if (token.type == TOKEN_SLASH) parser_eat(parser, TOKEN_SLASH);
        
        expr_t new_node = (expr_t){.as = {.binary_op = init_binary_op_t(token, parser_factor(parser), node)},
            .kind = EXPR_BINARY_OP};
        node = new_node;
    }

    return node;
}

expr_t parser_expr(parser_t* parser) {
    expr_t node = parser_term(parser);

    while (parser->current_token.type == TOKEN_PLUS || parser->current_token.type == TOKEN_MINUS) {
        token_t token = parser->current_token;
        if (token.type == TOKEN_PLUS) parser_eat(parser, TOKEN_PLUS);
        else if (token.type == TOKEN_MINUS) parser_eat(parser, TOKEN_MINUS);
        
        expr_t new_node = (expr_t){.as = {.binary_op = init_binary_op_t(token, parser_term(parser), node)},
            .kind = EXPR_BINARY_OP};
        node = new_node;
    }
    return node;
}

expr_t parser_parse(parser_t* parser) {
    return parser_expr(parser);
}
