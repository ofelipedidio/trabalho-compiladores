#include "symbols.h"
#include "lexeme.h"
#include <string.h>

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

sym_list_t* sym_list_find_key(sym_list_t *list, sym_key_t key)
{
    // Iterate through the list until either:
    // 1. The end of the list is reached (key not found)
    // 2. The key is found
    while (list != NULL && strcmp(list->key, key) != 0) {
        list = list->next;
    }

    // Return the result of the search
    return list;
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
        fprintf(
                stderr,
                "%s(key=\"%s\", value=<%s, lin=%lld, col=%lld, typ=%s, nat=%s>)\n", 
                (first ? "--- " : "    "),
                list->key,
                list->value.lex.value.ident,
                list->value.line,
                list->value.column,
                list->value.type == sem_type_int ? "int" :
                    list->value.type == sem_type_float ? "float" : "bool",
                list->value.nature == sym_nature_id ? "id" : 
                    list->value.nature == sym_nature_func ? "func" : "other"
                );
        list = list->next;
        first = 0;
    }
}

sym_tab_t* sym_tab_init() {
    // An empty stack is represented as a NULL pointer
    return NULL;
}

sym_tab_t* sym_tab_push(sym_tab_t *stack) {
    sym_tab_t *new_table = (sym_tab_t*) malloc(sizeof(sym_tab_t));
    new_table->parent = stack;
    new_table->list = sym_list_init();

    return new_table;
}

sym_tab_t* sym_tab_pop(sym_tab_t *table) {
    sym_tab_t *parent = table->parent;
    sym_tab_free_table(table);
    return parent;
}

sym_list_t* sym_tab_find(sym_tab_t *stack, sym_key_t key) {
    sym_list_t *item = NULL;

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

