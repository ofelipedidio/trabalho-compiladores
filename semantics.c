#include "semantics.h"

sem_type_t infer_type(sem_type_t left, sem_type_t right) {
    if (left == right) {
        return left;
    } else if (left == sem_type_float || right == sem_type_float) {
        return sem_type_float;
    } else {
        return sem_type_int;
    }
}


