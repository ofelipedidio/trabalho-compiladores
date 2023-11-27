#ifndef AST
#define AST

typedef enum ast_value_type {
    lit_int_t,
    lit_float_t,
    lit_bool_t,
    text_t,
    empty_t,
} ast_value_type_t;

typedef union ast_node_value {
    struct {
        ast_value_type_t type;
        long long int value;
    } lit_int;
    struct {
        ast_value_type_t type;
        double value;
    } lit_float;
    struct {
        ast_value_type_t type;
        int value;
    } lit_bool;
    struct {
        ast_value_type_t type;
        char *value;
    } text;
    struct {
        ast_value_type_t type;
    } empty;
} ast_value_t;

ast_value_t value_new_int(long long int value);
ast_value_t value_new_float(double value);
ast_value_t value_new_bool(int value);
ast_value_t value_new_text(char *value);
ast_value_t value_new_empty();

void value_free(ast_value_t value);

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
    ast_value_t value;
    int n;
    struct ast **children;
} ast_t;

ast_t *ast_new(ast_node_type_t type, ast_value_t value);

ast_t *ast_new_empty(ast_node_type_t type);

ast_t *ast_new_noop(char* message);

void ast_add_child(ast_t *parent, ast_t *child);

void ast_free(ast_t *ast);

char *ast_node_type_name(ast_node_type_t type);

void ast_value_label(ast_value_t value, char buf[]);

#endif // !AST
