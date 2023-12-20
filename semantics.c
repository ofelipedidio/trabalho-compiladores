#include "semantics.h"
#include "lexeme.h"
#include <string.h>

uint64_t ast_type_size(ast_type_t type) {
    switch (type) {
        case ast_int:
            return 4;
        case ast_float:
            return 8;
        case ast_bool:
            return 4;
        case ast_type_undefined:
            return 0;
    }
}


sym_list_t* sym_list_init() {
    // An empty list is represented as a NULL ptr
    return NULL;
}

sym_list_t* sym_list_insert(sym_list_t *list, sym_key_t key, sym_val_t value) {
    // Create the new node
    sym_list_t *new_node = (sym_list_t*) malloc(sizeof(sym_list_t));

    // Initialize variables
    new_node->key = strdup(key);
    new_node->value = value;
    new_node->prev = NULL;
    new_node->next = list;

    // Update the second element of the list
    if (list != NULL) {
        list->prev = new_node;
    }

    // Return the new list
    return new_node;
}

sym_list_t* sym_list_remove(sym_list_t *list, sym_key_t key) {
    // Remember the first node for later
    sym_list_t *first_node = list;

    // Iterate through the list until either:
    // 1. The end of the list is reached (key not found)
    // 2. The key is found
    while (list != NULL && strcmp(list->key, key) != 0) {
        list = list->next;
    }

    // If the key was not found, do nothing to the list and return it
    if (list == NULL) {
        return first_node;
    }

    // If the key was found, remove the node from the list
    if (list->prev == NULL) {
        // The node is the first element of the list
        first_node = list->next;
        list->next->prev = NULL;
    } else if (list->next == NULL) {
        // The node is the last element of the list
        list->prev->next = NULL;
    } else {
        // The node is in the middle of the list
        list->next->prev = list->prev;
        list->prev->next = list->next;
    }

    // Free the memory of the node
    sym_list_node_free(list);

    // Return the changed list
    return first_node;
}

sym_val_t *sym_list_find_key(sym_list_t *list, sym_key_t key) {
    // Iterate through the list until either:
    // 1. The end of the list is reached (key not found)
    // 2. The key is found
    while (list != NULL && strcmp(list->key, key) != 0) {
        list = list->next;
    }

    // Return the result of the search
    if (list != NULL) {
        return &list->value;
    } else {
        return NULL;
    }
}

void sym_list_free(sym_list_t *list) {
    sym_list_t *next;

    // Iterate through the list and free each node
    while (list != NULL) {
        next = list->next;
        sym_list_node_free(list);
        list = next;
    }
}

void sym_list_node_free(sym_list_t *node) {
    free(node->key);
    lexeme_free(node->value.lex);
    free(node);
}

void sym_list_print(sym_list_t *list) {
    int first = 1;
    while (list != NULL) {
        fprintf(stderr, "%s", first ? "--- " : "    ");
        sym_value_print(&list->value);
        list = list->next;
        first = 0;
    }
}

void sym_value_print(sym_val_t *value) {
    if (value == NULL) {
        fprintf(stderr, "(nul)\n");
        return;
    }

    fprintf(
            stderr,
            "(key=\"%s\", value=<%s, lin=%lld, col=%lld, typ=%s, nat=%s>)\n", 
            value->lex.value.ident,
            value->lex.value.ident,
            value->line,
            value->column,
            ast_type_to_string(value->type),
            value->nature == sem_nature_id ? "id" : 
            value->nature == sem_nature_func ? "func" : "other"
           );
}


sym_tab_t* sym_tab_init() {
    // An empty stack is represented as a NULL pointer
    return NULL;
}

sym_tab_t* sym_tab_push(sym_tab_t *stack) {
    sym_tab_t *new_table = (sym_tab_t*) malloc(sizeof(sym_tab_t));
    new_table->parent = stack;
    new_table->list = sym_list_init();
    if (stack->parent == NULL) {
        new_table->scope_size = 0;
    } else if (stack->parent->parent == NULL) {
        new_table->scope_size = 0;
    } else {
        new_table->scope_size = stack->scope_size;
    }

    return new_table;
}

sym_tab_t* sym_tab_pop(sym_tab_t *table) {
    sym_tab_t *parent = table->parent;
    sym_tab_free_table(table);
    return parent;
}

sym_val_t* sym_tab_find(sym_tab_t *stack, sym_key_t key) {
    sym_val_t *item = NULL;

    while (stack != NULL && item == NULL) {
        item = sym_list_find_key(stack->list, key);
        stack = stack->parent;
    }

    return item;
}

void sym_tab_free_table(sym_tab_t *table) {
    sym_list_free(table->list);
    free(table);
}

void sym_tab_print(sym_tab_t *stack) {
    fprintf(stderr, "[STACK TOP]\n");
    while (stack != NULL) {
        //fprintf(stderr, "---\n");
        sym_list_print(stack->list);
        stack = stack->parent;
    }
    fprintf(stderr, "[STACK BOTTOM]\n");
}


ast_type_t infer_type(ast_type_t left, ast_type_t right) {
    if (left == right) {
        return left;
    } else if (left == ast_float || right == ast_float) {
        return ast_float;
    } else {
        return ast_int;
    }
}

void register_symbol_global_variable(sym_tab_t *top_scope, sym_tab_t *current_scope, identifier_t *identifier, sem_nature_t nature, ast_type_t type) {
    sym_val_t temp_val;
    sym_val_t *temp;

    if ((temp = sym_tab_find(top_scope, identifier->text)) != NULL) {
        printf("Erro semantico: variavel \"%s\" foi declarada multiplas vezes.\n", identifier->text);
        printf("- Contexto: declaracao global na linha %lld, coluna %lld\n", identifier->lexeme.line, identifier->lexeme.column);
        sym_tab_t *global_scope = current_scope;
        while (global_scope->parent != NULL) {
            global_scope = global_scope->parent;
        }
        printf("- Contexto: dentro da funcao \"%s\"\n", global_scope->list->key);
        exit(ERR_DECLARED);
    }

    uint64_t type_size = ast_type_size(type);
    temp_val.line = identifier->lexeme.line;
    temp_val.column = identifier->lexeme.column;
    temp_val.nature = nature;
    temp_val.type = type;
    temp_val.lex = lex_clone(identifier->lexeme);
    temp_val.offset = current_scope->scope_size;
    temp_val.location = sem_global;
    current_scope->list = sym_list_insert(
            current_scope->list,
            identifier->text,
            temp_val
            ); 
    current_scope->scope_size += type_size;
}

void register_symbol_function(sym_tab_t *top_scope, sym_tab_t *current_scope, identifier_t *identifier, sem_nature_t nature, ast_type_t type) {
    sym_val_t temp_val;
    sym_val_t *temp;

    if ((temp = sym_tab_find(top_scope, identifier->text)) != NULL) {
        printf("Erro semantico: variavel \"%s\" foi declarada multiplas vezes.\n", identifier->text);
        printf("- Contexto: linha %lld, coluna %lld\n", identifier->lexeme.line, identifier->lexeme.column);
        sym_tab_t *global_scope = current_scope;
        while (global_scope->parent != NULL) {
            global_scope = global_scope->parent;
        }
        printf("- Contexto: na declaracao da funcao \"%s\"\n", identifier->text);
        exit(ERR_DECLARED);
    }

    temp_val.line = identifier->lexeme.line;
    temp_val.column = identifier->lexeme.column;
    temp_val.nature = nature;
    temp_val.type = type;
    temp_val.lex = lex_clone(identifier->lexeme);
    temp_val.offset = current_scope->scope_size;
    temp_val.location = sem_global;
    current_scope->list = sym_list_insert(
            current_scope->list,
            identifier->text,
            temp_val
            ); 
}

void register_symbol_parameter(sym_tab_t *top_scope, sym_tab_t *current_scope, identifier_t *identifier, sem_nature_t nature, ast_type_t type, int *delayed_param_error, identifier_t **error_identifier) {
    sym_val_t temp_val;
    sym_val_t *temp;

    if ((temp = sym_tab_find(top_scope, identifier->text)) != NULL) {
        *delayed_param_error = 1;
        *error_identifier = identifier;
        return;
    }

    uint64_t type_size = ast_type_size(type);
    temp_val.line = identifier->lexeme.line;
    temp_val.column = identifier->lexeme.column;
    temp_val.nature = nature;
    temp_val.type = type;
    temp_val.lex = lex_clone(identifier->lexeme);
    temp_val.offset = current_scope->scope_size;
    temp_val.location = sem_local;
    current_scope->list = sym_list_insert(
            current_scope->list,
            identifier->text,
            temp_val
            ); 
    current_scope->scope_size += type_size;
}

void register_symbol_local_variable(sym_tab_t *top_scope, sym_tab_t *current_scope, identifier_t *identifier, sem_nature_t nature, ast_type_t type) {
    sym_val_t temp_val;
    sym_val_t *temp;

    if ((temp = sym_tab_find(top_scope, identifier->text)) != NULL) {
        printf("Erro semantico: variavel \"%s\" foi declarada multiplas vezes.\n", identifier->text);
        printf("- Contexto: declaracao local na linha %lld, coluna %lld\n", identifier->lexeme.line, identifier->lexeme.column);
        sym_tab_t *global_scope = current_scope;
        while (global_scope->parent != NULL) {
            global_scope = global_scope->parent;
        }
        printf("- Contexto: dentro da funcao \"%s\"\n", global_scope->list->key);
        exit(ERR_DECLARED);
    }

    uint64_t type_size = ast_type_size(type);
    temp_val.line = identifier->lexeme.line;
    temp_val.column = identifier->lexeme.column;
    temp_val.nature = nature;
    temp_val.type = type;
    temp_val.lex = lex_clone(identifier->lexeme);
    temp_val.offset = current_scope->scope_size;
    temp_val.location = sem_local;
    current_scope->list = sym_list_insert(
            current_scope->list,
            identifier->text,
            temp_val
            ); 
    current_scope->scope_size += type_size;
}

int get_variable(sym_tab_t *current_scope, char *name, sem_var_location_t *location, uint64_t *offset) {
    sym_val_t *value = sym_tab_find(current_scope, name);
    if (value == NULL) {
        return 0;
    }
    *location = value->location;
    *offset = value->offset;
    return 1;
}

