#ifndef AST
#define AST

#include "lexeme.h"

/* ##############
 * # Structures #
 * ############## */
typedef enum {
    // Internal (node)
    noop, // Completely removed from ast
    call_argument, // Acts like a leaf linked list struct(leaf, next)

    // Global (node)
    func_declaration, // First child is internal

    // Statements (node)
    statement_decl,
    statement_attr,
    statement_call, // First child is internal
    statement_return,
    statement_if,
    statement_while,

    // Expressions (node)
    expr_not,
    expr_inv,
    expr_mult,
    expr_div,
    expr_mod,
    expr_add,
    expr_sub,
    expr_lt,
    expr_gt,
    expr_le,
    expr_ge,
    expr_eq,
    expr_ne,
    expr_and,
    expr_or,

    // Literals (leaf)
    lit_int_type,
    lit_float_type,
    lit_bool_type,
    ident,
} ast_type_t;

struct ast;

typedef union {
    struct {
        int n;
        struct ast **children;
    } node;
    lexeme_t leaf;
} ast_body_t;

typedef struct ast {
    ast_type_t type;
    ast_body_t body;
} ast_t;

/* ################
 * # Constructors #
 * ################ */
ast_t *ast_make_node(ast_type_t type);
ast_t *ast_make_leaf(ast_type_t type, lexeme_t lexeme);

/* ###############
 * # Destructors #
 * ############### */
void ast_free(ast_t *ast);

/* ###########
 * # Methods #
 * ########### */
void ast_append(ast_t *ast, ast_t *child);

char *ast_node_type_name(ast_type_t type);

#endif // !AST
