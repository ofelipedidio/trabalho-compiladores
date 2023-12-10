#include "symbols.h"

sym_t* sym_init_list()
{
    return NULL;
}

sym_t* sym_insert_node(sym_t *sym_list, sym_t sym)
{
    sym_t *new_pt;

    new_pt = (sym_t)malloc(sizeof(sym_t));
    new_pt->key = strdup(sym_t.key);
    new_pt->value = sym_t.value;

    if (sym_list) {
        // inserir sempre no inicio
        new_pt->next = sym_list;
        new_pt->prev = NULL;
        sym_list->prev = new_pt;
    } else {
        new_pt->next = NULL;
        new_pt->prev = NULL;
    }
    return new_pt;
}

sym_t* sym_remove_node(sym_t *sym_list, char *key)
{
    sym_t *aux_pt = sym_list;

    if (!sym_list) {
        return NULL;
    }

    while (strcmp(aux_pt->key, key) != 0 && aux_pt->next) {
        aux_pt = aux_pt->next;
    }

    if (strcmp(aux_pt->key, key) == 0) {
        if (aux_pt->prev == NULL) {
            sym_list = sym_list->next;
            sym_list->prev = NULL;
        }
        else if (aux_pt->next == NULL) {
            aux_pt->prev->next == NULL;
        }
        else {
            aux_pt->prev->next = aux_pt->next;
            aux_pt->next->prev = aux_pt->prev;
        }
        free(aux_pt);
    }
    return sym_list;
}

sym_t* sym_find_node(sym_t *sym_list, char *key)
{
    sym_t *aux_pt = sym_list;

    if (!sym_list) {
        return NULL;
    }

    while (strcmp(aux_pt->key, key) != 0 && aux_pt->next) {
        aux_pt = aux_pt->next;
    }

    if (strcmp(aux_pt->key, key) == 0) {
        return aux_pt;
    }
    return NULL;
}

sym_t* sym_free_list(sym_t *sym_list)
{
    sym_t *aux_pt = sym_list;
    while (sym_list) {
        aux_pt = sym_list;
        free(sym_list->key);
        sym_list = sym_list->next;
        free(aux_pt);
    }
    free(sym_list);
    return NULL;
}

sym_tab_t* sym_tab_init()
{
    return NULL;
}

sym_tab_t* sym_tab_push(sym_tab_t *table)
{
    sym_tab_t *new_pt;

    new_pt = (sym_tab_t*)malloc(sizeof(sym_tab_t));
    new_pt->list = sym_init_list();
    new_pt->father = table;
    return new_pt;
}

sym_tab_t* sym_tab_pop(sym_tab_t *table)
{
    sym_tab_t *aux_pt = table->father;
    sym_free_list(table->list);
    free(table);
    return aux_pt;
}

sym_t* sym_tab_find(sym_tab_t *table, char *key)
{
    sym_tab_t *aux_pt = table;
    sym_t *found_item = NULL;

    while (found_item == NULL && aux_pt) {
        found_item = sym_find_key(aux_pt->list, key);
        aux_pt = aux_pt->father;
    }
    return found_item;
}
