typedef enum {
    sym_nature_lit,
    sym_nature_id,
    sym_nature_func,
} sym_nature_t;

typedef enum {
    lit_int_type,
    lit_float_type,
    lit_bool_type,
} sym_type_t;

typedef struct {
    long long int line;
    sym_nature_t nature;
    sym_type_t type;
    lexeme_t lex;
} sym_val_t;

typedef struct node_sym_t {
    char *key;
    sym_val_t value;
    struct node_sym_t *prev;
    struct node_sym_t *next;
} sym_t;

typedef struct node_tab_t {
    sym_t *list;
    struct node_tab_t *father;
} sym_tab_t;

sym_t* sym_init_list();
sym_t* sym_insert_node(sym_t *sym_list, sym_t sym);
sym_t* sym_remove_node(sym_t *sym_list, char *key);
sym_t* sym_find_key(sym_t *sym_list, char *key);
sym_t* sym_free_list(sym_t *list_pt);

sym_tab_t* sym_tab_init();
sym_tab_t* sym_tab_push(sym_tab_t *table);
sym_tab_t* sym_tab_pop();
sym_t* sym_tab_find(sym_t);
