#ifndef SEMANTICS
#define SEMANTICS

#include "ast.h"
#include "lexeme.h"

#define ERR_UNDECLARED 10 //2.2
#define ERR_DECLARED 11 //2.2
#define ERR_VARIABLE 20 //2.3
#define ERR_FUNCTION 21 //2.3

typedef enum {
    sem_type_int,
    sem_type_float,
    sem_type_bool,
} sem_type_t;

typedef enum {
    sym_nature_lit,
    sym_nature_id,
    sym_nature_func,
} sym_nature_t;

typedef char* sym_key_t;

typedef struct {
    long long int line;
    long long int column;
    sym_nature_t nature;
    sem_type_t type;
    lexeme_t lex;
} sym_val_t;

typedef struct sym_list {
    sym_key_t key;
    sym_val_t value;
    struct sym_list *prev;
    struct sym_list *next;
} sym_list_t;

typedef struct node_tab_t {
    sym_list_t *list;
    struct node_tab_t *parent;
} sym_tab_t;

/*
 * This function initializes the list and returns a pointer to it.
 */
sym_list_t* sym_list_init();

/*
 * This function inserts a new node at the beginning of the list 
 * and returns the new pointer to the list.
 */
sym_list_t* sym_list_insert(sym_list_t *list, sym_key_t key, sym_val_t value);

/*
 * This function removes a node from the list by key 
 * and returns the pointer to the list.
 */
sym_list_t* sym_list_remove(sym_list_t *list, sym_key_t key);

/*
 * This function searches for a node by key. If the node if found,
 * the function returns a pointer to it, otherwise it returns NULL.
 * The list is not modified by this function
 */
sym_val_t* sym_list_find_key(sym_list_t *list, sym_key_t key);

/*
 * This functions frees all nodes from this list.
 */
void sym_list_free(sym_list_t *list);

/*
 * This functions frees a node from the list. The relationship between 
 * this node and the rest of the nodes in its list (prev and next) is ignored.
 * Calling this function on a node that is attached to a list WILL break it.
 */
void sym_list_node_free(sym_list_t *node);

/*
 * This function prints the entire list 
 */
void sym_list_print(sym_list_t *list);


/*
 * This function initializes the stack and returns a pointer to it.
 */
sym_tab_t* sym_tab_init();

/*
 * This function pushes a new table into the stack and returns a pointer to it.
 */
sym_tab_t* sym_tab_push(sym_tab_t *stack);

/*
 * This function pops the top of the stack and returns a pointer to the new top.
 */
sym_tab_t* sym_tab_pop(sym_tab_t *stack);

/*
 * This function searches for an item in the whole stack. If the item is found,
 * it returns the item, otherwise it returns NULL.
 */
sym_val_t* sym_tab_find(sym_tab_t *stack, sym_key_t key);

/*
 * Frees all the memory taken from a stack table. The relationship between 
 * tables in the stack is ignored. Calling this function on a node that is
 * attached to a stack WILL break it.
 */
void sym_tab_free_table(sym_tab_t *table);

/*
 * This function prints the entire stack
 */
void sym_tab_print(sym_tab_t *stack);


sem_type_t from_ast_type(ast_type_t type);

sem_type_t infer_type(sem_type_t left, sem_type_t right);

#define sem_type_to_string(expr) ((expr) == sem_type_int ? "int" : ((expr) == sem_type_float ? "float" : "bool"))

// void handle_variable_attribution(ast_t *self, ast_t *identifier, ast_t *expr);

void register_symbol(sym_tab_t *current_scope, identifier_t *identifier, sym_nature_t nature, sem_type_t type);

#endif // !SEMANTICS
