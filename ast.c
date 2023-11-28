#include "ast.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


/* ################
 * # Constructors #
 * ################ */
ast_t *ast_make_node(ast_type_t type) {
    switch (type) {
        case lit_int_type:
        case lit_float_type:
        case lit_bool_type:
        case ident:
            fprintf(stderr, "Tried to initialize a node with a literal/identifier\n");
            exit(EXIT_FAILURE);
            break;
        default:
            break;
    }

    ast_t *ast = (ast_t*) malloc(sizeof(ast_t));
    if (ast == NULL) {
            fprintf(stderr, "Could not allocate memory for ast_t (node)");
            exit(EXIT_FAILURE);
            return NULL;
    }

    ast->type = type;
    ast->body.node.n = 0;
    ast->body.node.children = (ast_t**) malloc(0*sizeof(ast_t*));
    
    if (ast->body.node.children == NULL) {
        free(ast);
        fprintf(stderr, "Could not allocate memory for ast_t children array");
        exit(EXIT_FAILURE);
        return NULL;
    }

    return ast;
}

ast_t *ast_make_leaf(ast_type_t type, lexeme_t lexeme) {
    switch (type) {
        case lit_int_type:
        case lit_float_type:
        case lit_bool_type:
        case ident:
            break;
        default:
            fprintf(stderr, "Tried to initialize a leaf with something other than literal/identifier\n");
            exit(EXIT_FAILURE);
            break;
    }

    ast_t *ast = (ast_t*) malloc(sizeof(ast_t));
    if (ast == NULL) {
            fprintf(stderr, "Could not allocate memory for ast_t (leaf)");
            exit(EXIT_FAILURE);
            return NULL;
    }

    ast->type = type;
    ast->body.leaf = lexeme;

    return ast;
}

/* ###############
 * # Destructors #
 * ############### */
void ast_free(ast_t *ast) {
    switch (ast->type) {
        case lit_int_type:
        case lit_float_type:
        case lit_bool_type:
        case ident:
            lexeme_free(ast->body.leaf);
            break;
        default:
            for (int i = 0; i < ast->body.node.n; i++) {
                ast_free(ast->body.node.children[i]);
            }
            free(ast->body.node.children);
            break;
    }
    free(ast);
}

/* ###########
 * # Methods #
 * ########### */
void ast_append(ast_t *ast, ast_t *child) {
    switch (ast->type) {
        case lit_int_type:
        case lit_float_type:
        case lit_bool_type:
        case ident:
            free(child);
            fprintf(stderr, "Tried to append to a leaf node of the ast\n");
            exit(EXIT_FAILURE);
            return;
        default:
            break;
    }

    if (child->type == noop) {
        ast_free(child);
        return;
    }

    ast_t **new_children = (ast_t**) realloc(
            ast->body.node.children, (ast->body.node.n+1)*sizeof(ast_t*));
    if (new_children == NULL) {
        free(child);
        fprintf(stderr, "Could not reallocate memory for new with child\n");
        exit(EXIT_FAILURE);
        return;
    }

    new_children[ast->body.node.n] = child;
    ast->body.node.children = new_children;
    ast->body.node.n += 1;
}

char *ast_node_type_name(ast_type_t type) {
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
        case ident:
            return "lit_ident_type";
        
        default:
            return "<unidentified>";
    }
}
