#pragma once

#include <inttypes.h>

typedef struct {
    void ** contents;
    uint64_t length;
    uint64_t capacity;
} list_t;

list_t *empty_list();

void list_push(list_t *list, void *item);

void *list_get(list_t *list, uint64_t index);

void list_free(list_t *list);

#define list_get_as(list, index, type) ((type*) list_get(list, index))

#define list_iterate(list, i) for (uint64_t i = 0; i < (list)->length; i++)





// Imported list library from another project of mine
#include <stdlib.h>

#define nlist_definition(T) \
    struct { \
        T *items; \
        size_t length; \
        size_t capacity; \
    }

#define nlist_init(T, list) \
    (list).capacity = 1; \
    (list).items = (T*) calloc((list).capacity, sizeof(T)); \
    (list).length = 0;

#define nlist_insert(T, list, item) \
    while ((list).length + 1 >= (list).capacity) { \
        size_t new_capacity = (list).capacity * 2; \
        T *new_array = (T*) reallocarray((list).items, new_capacity, sizeof(T)); \
        (list).items = new_array; \
        (list).capacity = new_capacity; \
    } \
    (list).items[(list).length] = (item); \
    (list).length++;

#define nlist_get(list, index) \
    (((index) < (list).length) ? &(list).items[(index)] : NULL)

#define nlist_get_unsafe(list, index) \
    ((list).items[(index)])

#define nlist_free(list) \
    free((list).items);

#define nlist_find(T, list, item, cond, res) \
    res = NULL; \
    for (size_t i = 0; i < (list).length; i++) { \
        T item = nlist_get_unsafe(list, i); \
        if (cond) { \
            res = &nlist_get_unsafe(list, i); \
            break; \
        } \
    }

