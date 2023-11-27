#include "ast.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

ast_value_t value_new_int(long long int value) {
    ast_value_t v;
    v.lit_int.type = lit_int_t;
    v.lit_int.value = value;
    return v;
}

ast_value_t value_new_float(double value) {
    ast_value_t v;
    v.lit_float.type = lit_float_t;
    v.lit_float.value = value;
    return v;
}

ast_value_t value_new_bool(int value) {
    ast_value_t v;
    v.lit_bool.type = lit_bool_t;
    v.lit_bool.value = value;
    return v;
}

ast_value_t value_new_text(char *value) {
    ast_value_t v;
    v.text.type = text_t;
    v.text.value = value;
    return v;
}

ast_value_t value_new_empty() {
    ast_value_t v;
    v.empty.type = empty_t;
    return v;
}

void value_free(ast_value_t value) {
    switch (value.empty.type) {
        case text_t:
            free(value.text.value);
            break;
        default:
            break;
    }
}

ast_t *ast_new(ast_node_type_t type, ast_value_t value) {
    ast_t *ast = (ast_t*) malloc(sizeof(ast_t));
    if (ast == NULL) {
        free(ast);
        fprintf(stderr, "Failed to allocate memory for a new AST\n");
        exit(EXIT_FAILURE);
    }

    ast->type = type;
    ast->value = value;
    ast->n = 0;
    ast->children = (ast_t**) calloc(0, sizeof(ast_t*));

    if (ast->children == NULL) {
        free(ast->children);
        free(ast);
        fprintf(stderr, "Failed to allocate memory for a new child pointer array\n");
        exit(EXIT_FAILURE);
    }

    return ast;
}

ast_t *ast_new_empty(ast_node_type_t type) {
    return ast_new(type, value_new_empty());
}

ast_t *ast_new_noop(char *message) {
    return ast_new(noop, value_new_text(strdup(message)));
}

void ast_add_child(ast_t *parent, ast_t *child) {
    if (child->type == noop) {
        ast_free(child);
        return;
    }

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
    value_free(ast->value);
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

void ast_value_label(ast_value_t value, char buf[]) {
    switch(value.empty.type) {
        case lit_int_t:
            sprintf(buf, "%lld", value.lit_int.value);
            break;
        case lit_float_t:
            sprintf(buf, "%f", value.lit_float.value);
            break;
        case lit_bool_t:
            sprintf(buf, "%s", value.lit_bool.value ? "true" : "false");
            break;
        case text_t:
            sprintf(buf, "%s", value.text.value);
            break;
        case empty_t:
            sprintf(buf, "%lld", value.lit_int.value);
            break;
    }
}
