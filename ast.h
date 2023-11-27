#ifndef AST
#define AST

typedef struct empty_s { int temp; } empty_t; 

typedef union ast_node_value {
    long long int lit_int;
    double lit_float;
    int lit_bool;
    char *lit_ident;
    empty_t empty;
} ast_node_value_t;

ast_node_value_t empty_value();

typedef enum ast_node_type {
    noop,
    func_declaration,

    statement_decl,
    statement_attr,
    statement_call,
    statement_return,
    statement_if,
    statement_while,

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

    lit_int_type,
    lit_float_type,
    lit_bool_type,
    lit_ident_type,
} ast_node_type_t;

typedef struct ast {
    ast_node_type_t type;
    ast_node_value_t value;
    int n;
    struct ast **children;
} ast_t;

ast_t *ast_new(ast_node_type_t type, ast_node_value_t value);

ast_t *ast_new_noop(char* message);

void ast_add_child(ast_t *parent, ast_t *child);

void ast_free(ast_t *ast);

char *ast_node_type_name(ast_node_type_t type);

void ast_node_value_label(ast_node_type_t type, ast_node_value_t *value, char buf[]);

#endif // !AST
