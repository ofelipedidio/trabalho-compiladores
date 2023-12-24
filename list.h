#pragma once

#include <inttypes.h>

typedef struct {
    void ** contents;
    uint64_t length;
    uint64_t capacity;
} list_t;

list_t empty_list();

void list_push(list_t *list, void *item);

void *list_get(list_t *list, uint64_t index);

#define list_get_as(list, index, type) ((type*) list_get(list, index))

#define list_iterate(list, i) for (uint64_t i = 0; i < (list).length; i++)
