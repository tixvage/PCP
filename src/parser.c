#include "include/parser.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

void print_ws(int level) {
    for (int i = 0; i < level; i++) printf(" ");
}

void print_tree(expr_t* expr, int level) {
    switch (expr->kind) {
        case EXPR_LIT_STR: {
            printf("not implemented yet\n");
        } break;
        case EXPR_LIT_INT: {
            print_ws(level+3);
            printf("integer literal -> %d\n", expr->as.lit_int);
        } break;
        case EXPR_BINARY_OP: {
            binary_op_t* binary_op = expr->as.binary_op;
            print_ws(level+3);
            printf("op -> %s\n", token_to_str(binary_op->op));
            print_ws(level+3);
            print_tree(&binary_op->lhs, level+3);
            print_ws(level+3);
            print_tree(&binary_op->rhs, level+3);
        } break;
        case EXPR_UNARY_OP: {
            unary_op_t* unary_op = expr->as.unary_op;
            print_ws(level+3);
            printf("op -> %s\n", token_to_str(unary_op->op));
            print_ws(level+3);
            print_tree(&unary_op->expr, level+3);
            
        } break;
        case EXPR_COMPOUND: {
            compound_t* compound = expr->as.compound;
            for (size_t i = 0; i < compound->childs.size; i++) {
                print_ws(level+3);
                printf("compound:\n");
                print_tree(&compound->childs.items[i], level+3);
            }
        } break;
        case EXPR_ASSIGN: {
            assign_t* assign = expr->as.assign;
            print_ws(level+3);
            switch (assign->type) {
                case CONST: {
                    printf("const assign:\n");
                } break;
                case ASSIGN: {
                    printf("assign:\n");
                } break;
                case REASSIGN: {
                    printf("reassign:\n");
                } break;
                default: {
                    printf("wtf assign?!?:\n");
                } break;
            }
            print_ws(level+3);
            print_tree(&assign->left, level+3);
            print_ws(level+3);
            print_tree(&assign->right, level+3);
        } break;
        case EXPR_VAR: {
            var_t* var = expr->as.var;
            print_ws(level+3);
            printf("name -> %s\n", var->value);
        } break;
        case EXPR_FN: {
            function_t* fn = expr->as.function;
            print_ws(level+3);
            printf("return type -> %s\n", fn->return_value);
            print_tree(&fn->compound, level+3);
        } break;
        case EXPR_NO_OP: {
            print_ws(level+3);
            printf("end of block\n");
        } break;
        default: printf("NULL\n"); break;
    }
}

void destroy_tree(expr_t* expr) {
    switch (expr->kind) {
        case EXPR_INVALID: {
            assert(0 && "invalid expr wtf");
        } break;
        case EXPR_LIT_STR: {
            assert(0 && "not implemented yet");
        } break;
        case EXPR_LIT_INT: break;
        case EXPR_BINARY_OP: {
            binary_op_t* bop = expr->as.binary_op;
            destroy_tree(&bop->lhs);
            destroy_tree(&bop->rhs);
            free(bop);
        } break;
        case EXPR_UNARY_OP: {
            unary_op_t* uop = expr->as.unary_op;
            destroy_tree(&uop->expr);
            free(uop);
        } break;
        case EXPR_COMPOUND: {
            compound_t* c = expr->as.compound;
            for (size_t i = 0; i < c->childs.size; i++) {
                destroy_tree(&c->childs.items[i]);
            }
            free(c->childs.items);
            free(c);
        } break;
        case EXPR_ASSIGN: {
            assign_t* a = expr->as.assign;
            destroy_tree(&a->left);
            destroy_tree(&a->right);
            free(a);
        } break;
        case EXPR_VAR: {
            var_t* var = expr->as.var;
            free(var->value);
            free(var);
        } break;
        case EXPR_FN: {
            function_t* fn = expr->as.function;
            free(fn->return_value);
            destroy_tree(&fn->compound);
            free(fn);
        } break;
        case EXPR_NO_OP: break;
    }
}

binary_op_t* init_binary_op_t(token_t op, expr_t rhs, expr_t lhs) {
    binary_op_t* binary_op = calloc(1, sizeof(binary_op_t));
    binary_op->op = op;
    binary_op->rhs = rhs;
    binary_op->lhs = lhs;

    return binary_op;
}

unary_op_t* init_unary_op_t(token_t op, expr_t expr) {
    unary_op_t* unary_op = calloc(1, sizeof(unary_op_t));
    unary_op->expr = expr;
    unary_op->op = op;

    return unary_op;
}

compound_list_t init_list(void) {
    compound_list_t list = (compound_list_t){.items = NULL, .size = 0};
    return list;
}
void list_push(compound_list_t* list, expr_t item) {
    list->size += 1;
    if (!list->items) list->items = calloc(1, sizeof(expr_t));
    else list->items = realloc(list->items, (list->size * sizeof(expr_t)));

    list->items[list->size-1] = item;
}

compound_t* init_compound_t(void) {
    compound_t* compound = calloc(1, sizeof(compound_t));
    compound->childs = init_list();

    return compound;
}

assign_t* init_assign_t(expr_t left, expr_t right, token_t op, int type) {
    assign_t* assign = calloc(1, sizeof(assign_t));
    assign->left = left;
    assign->right = right;
    assign->op = op;
    assign->type = type;
    return assign;
}

var_t* init_var_t(token_t token) {
    var_t* var = calloc(1, sizeof(var_t));
    var->token = token;
    char* value = calloc(strlen(token.value) + 1, sizeof(char));
    strcpy(value, token.value);
    value[strlen(token.value)] = '\0';
    var->value = value;

    return var;
}

function_t* init_function_t(const char* return_value, expr_t compound) {
    function_t* function = calloc(1, sizeof(function_t));
    char* value = calloc(strlen(return_value) + 1, sizeof(char));
    strcpy(value, return_value);
    value[strlen(return_value)] = '\0';
    function->return_value = value;
    function->compound = compound;

    return function;
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
    else if (token.type == TOKEN_PLUS) {
        parser_eat(parser, TOKEN_PLUS);
        return (expr_t) {.as = {.unary_op = init_unary_op_t(token, parser_factor(parser))}, .kind = EXPR_UNARY_OP};
    }
    else if (token.type == TOKEN_MINUS) {
        parser_eat(parser, TOKEN_MINUS);
        return (expr_t) {.as = {.unary_op = init_unary_op_t(token, parser_factor(parser))}, .kind = EXPR_UNARY_OP};
    }
    else if (token.type == TOKEN_L_PAREN) {
        parser_eat(parser, TOKEN_L_PAREN);
        expr_t node = parser_expr(parser);
        parser_eat(parser, TOKEN_R_PAREN);
        return node;
    }

    return parser_variable(parser);
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

expr_t parser_variable(parser_t* parser) {
    //TODO: multiple return values?
    expr_t node = (expr_t){.as = {.var = init_var_t(parser->current_token)}, .kind = EXPR_VAR};
    parser_eat(parser, TOKEN_IDENTIFIER);

    return node;
}

expr_t parser_identifier_statement(parser_t* parser) {
    expr_t left = parser_variable(parser);
    token_t op = parser->current_token;
    if (op.type == TOKEN_COLON_EQUAL) {
        parser_eat(parser, TOKEN_COLON_EQUAL);
        expr_t right = parser_expr(parser);
        expr_t node = (expr_t){.as = {.assign = init_assign_t(left, right, op, ASSIGN)}, .kind = EXPR_ASSIGN};
        return node;
    }
    else if (op.type == TOKEN_COLON_COLON) {
        parser_eat(parser, TOKEN_COLON_COLON);
        if (parser->current_token.type == TOKEN_KEYWORD_FN) {
            expr_t right = parser_function_statement(parser);
            //TODO: compound must be a function
            return (expr_t){.as = {.assign = init_assign_t(left, right, op, CONST)}, .kind = EXPR_ASSIGN};
        }
        expr_t right = parser_expr(parser);
        expr_t node = (expr_t){.as = {.assign = init_assign_t(left, right, op, CONST)}, .kind = EXPR_ASSIGN};
        return node;
    }
    else if (op.type == TOKEN_EQUAL) {
        parser_eat(parser, TOKEN_EQUAL);
        expr_t right = parser_expr(parser);
        expr_t node = (expr_t){.as = {.assign = init_assign_t(left, right, op, REASSIGN)}, .kind = EXPR_ASSIGN};
        return node;
    }

    assert(0 && "Expected \"::\", \":=\" or \"=\" but got something else");
}

expr_t parser_statement(parser_t* parser) {
    if (parser->current_token.type == TOKEN_L_BRACE) return parser_compound_statement(parser);
    else if (parser->current_token.type == TOKEN_IDENTIFIER) return parser_identifier_statement(parser);
    return (expr_t){.as = {.no_op = (no_op_t){}}, .kind = EXPR_NO_OP};
}

expr_t parser_function_statement(parser_t* parser) {
    parser_eat(parser, TOKEN_KEYWORD_FN);
    parser_eat(parser, TOKEN_L_PAREN);
    //TODO: parse function arguments
    parser_eat(parser, TOKEN_R_PAREN);
    parser_eat(parser, TOKEN_ARROW);
    expr_t return_value = parser_variable(parser);
    expr_t compound = parser_compound_statement(parser);
    expr_t node = (expr_t){.as = {.function = init_function_t(return_value.as.var->value, compound)}, .kind = EXPR_FN};
    destroy_tree(&return_value);
    return node;
}

compound_list_t parser_statement_list(parser_t* parser) {
    expr_t node = parser_statement(parser);
    compound_list_t list = init_list();
    list_push(&list, node);
    
    while (parser->current_token.type == TOKEN_SEMICOLON) {
        parser_eat(parser, TOKEN_SEMICOLON);
        list_push(&list, parser_statement(parser));
    }

    return list;
}

expr_t parser_compound_statement(parser_t* parser) {
    parser_eat(parser, TOKEN_L_BRACE);
    compound_list_t nodes = parser_statement_list(parser);
    parser_eat(parser, TOKEN_R_BRACE);

    compound_t* root = init_compound_t();
    root->childs = nodes;
    
    expr_t node = (expr_t){.as = {.compound = root}, .kind = EXPR_COMPOUND};
    return node;
}

expr_t parser_program(parser_t* parser) {
    expr_t node = parser_compound_statement(parser);
    parser_eat(parser, TOKEN_SEMICOLON);

    return node;
}

expr_t parser_parse(parser_t* parser) {
    return parser_identifier_statement(parser);
}
