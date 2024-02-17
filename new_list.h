#ifndef LIST
#define LIST

#include <stdlib.h>

#define nlist_definition(T) \
    struct { \
        T *items; \
        size_t length; \
        size_t capacity; \
    }

#define nlist_init(list) \
    (list).capacity = 1; \
    (list).items = (typeof((list).items)) calloc((list).capacity, sizeof(typeof(list.items[0]))); \
    (list).length = 0;

#define nlist_push(list, item) \
    while ((list).length + 1 >= (list).capacity) { \
        size_t new_capacity = (list).capacity * 2; \
        typeof((list).items) new_array = (typeof((list).items)) realloc((list).items, new_capacity * sizeof(typeof((list).items[0]))); \
        (list).items = new_array; \
        (list).capacity = new_capacity; \
    } \
    (list).items[(list).length] = (item); \
    (list).length++;

#define nlist_get(list, index) \
    (((index) < (list).length) ? &(list).items[(index)] : NULL)

#define nlist_get_unsafe(list, index) \
    ((list).items[(index)])

#define nlist_free(list, destructor) \
    for (size_t i = 0; i < list.length; i++) { \
        destructor \
    } \
    free((list).items);

#endif

