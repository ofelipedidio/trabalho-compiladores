#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "list.h"

#define LIST_INITIAL_CAPACITY 10

list_t empty_list() {
    list_t list;
    void** contents = (void**) malloc(LIST_INITIAL_CAPACITY*sizeof(void*));
    if (contents == NULL) {
        fprintf(stderr, "Failed to allocate memory for void* (errno = %d) [at file \"" __FILE__ "\", line %d]\n", errno, __LINE__-3);
        exit(1);
    }
    list.contents = contents;
    list.capacity = LIST_INITIAL_CAPACITY;
    list.length = 0;
    return list;
}

void list_push(list_t *list, void *item) {
    // Ensure the item can fit on the list
    while (list->length+1 >= list->capacity) {
        // new_capacity = round_up(3/2 * capacity);
        uint64_t new_capacity = (list->capacity * 3 + 1) / 2;
        void **new_contents = realloc(list->contents, new_capacity * sizeof(void*));
        if (new_contents == NULL) {
            fprintf(stderr, "ERROR: Failed to reallocate memory for void* (errno = %d) [at file \"" __FILE__ "\", line %d]\n", errno, __LINE__-2);
            exit(EXIT_FAILURE);
        }
        list->contents = new_contents;
        list->capacity = new_capacity;
    }
    list->contents[list->length] = item;
    list->length++;
}

void *list_get(list_t *list, uint64_t index) {
    if (index >= list->length) {
        return NULL;
    }
    return list->contents[index];
}

void list_free(list_t *list) {
    free(list->contents);
    free(list);
}
