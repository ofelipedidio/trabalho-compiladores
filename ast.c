#include "ast.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>

ast_node_value_t empty_value() {
    return (ast_node_value_t) {.empty = (empty_t) { .temp = 0 }};
}

ast_node_value_t noop_value(char* text) {
    return (ast_node_value_t) {.lit_ident = text};
}

ast_t *ast_new(ast_node_type_t type, ast_node_value_t value) {
    ast_t *ast = (ast_t*) malloc(sizeof(ast_t));
    if (ast == NULL) {
        fprintf(stderr, "Failed to allocate memory for a new AST\n");
        exit(EXIT_FAILURE);
    }

    ast->type = type;
    ast->value = value;
    ast->n = 0;
    ast->children = (ast_t**) calloc(0, sizeof(ast_t*));

    if (ast->children == NULL) {
        fprintf(stderr, "Failed to allocate memory for a new child pointer array\n");
        exit(EXIT_FAILURE);
    }

    return ast;
}

ast_t *ast_new_noop(char *message) {
    return ast_new(noop, noop_value(message));
}

void ast_add_child(ast_t *parent, ast_t *child) {
    ast_t **children = (ast_t**) reallocarray(parent->children, parent->n+1, sizeof(ast_t*));
    if (children == NULL) {
        fprintf(stderr, "Failed to allocate memory for a new child pointer\n");
        exit(EXIT_FAILURE);
    }
    parent->children = children;
    parent->children[parent->n++] = child;
}

void ast_free(ast_t *ast) {
    if (ast == NULL) {
        fprintf(stderr, "Tried to free a null AST\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < ast->n; i++) {
        ast_free(ast->children[i]);
    }
    free(ast->children);
    free(ast);
}

char *ast_node_type_name(ast_node_type_t type) {
    switch (type) {
        case noop:
            return "noop";
        case func_declaration:
            return "func_declaration";
        case statement_decl:
            return "statement_decl";
        case statement_attr:
            return "statement_attr";
        case statement_call:
            return "statement_call";
        case statement_return:
            return "statement_return";
        case statement_if:
            return "statement_if";
        case statement_while:
            return "statement_while";

        case expr_not:
            return "expr_not";
        case expr_inv:
            return "expr_inv";
        case expr_mult:
            return "expr_mult";
        case expr_div:
            return "expr_div";
        case expr_mod:
            return "expr_mod";
        case expr_add:
            return "expr_add";
        case expr_sub:
            return "expr_sub";
        case expr_lt:
            return "expr_lt";
        case expr_gt:
            return "expr_gt";
        case expr_le:
            return "expr_le";
        case expr_ge:
            return "expr_ge";
        case expr_eq:
            return "expr_eq";
        case expr_ne:
            return "expr_ne";
        case expr_and:
            return "expr_and";
        case expr_or:
            return "expr_or";

        case lit_int_type:
            return "lit_int_type";
        case lit_float_type:
            return "lit_float_type";
        case lit_bool_type:
            return "lit_bool_type";
        case lit_ident_type:
            return "lit_ident_type";
        
        default:
            return "<unidentified>";
    }
}

void ast_node_value_label(ast_node_type_t type, ast_node_value_t *value, char buf[]) {
    switch (type) {
        case lit_int_type:
            sprintf(buf, "%lld", value->lit_int);
            return;
        case lit_float_type:
            sprintf(buf, "%f", value->lit_float);
            return;
        case lit_bool_type:
            sprintf(buf, "%s", value->lit_bool == 0 ? "false" : "true");
            return;
        case lit_ident_type:
            sprintf(buf, "%s", value->lit_ident);
            return;
        case func_declaration:
            sprintf(buf, "%s", value->lit_ident);
            return;
        case statement_call:
            sprintf(buf, "%s", value->lit_ident);
            return;
        case noop:
            sprintf(buf, "%s", value->lit_ident);
            return;
        default:
            buf[0] = '\0';
            return;
    }
}
