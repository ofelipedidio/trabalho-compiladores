#ifndef TYPE
#define TYPE

#include "ast.h"

typedef enum {
    sem_type_int,
    sem_type_float,
    sem_type_bool,
} sem_type_t;

sem_type_t infer_type(sem_type_t left, sem_type_t right);

#define sem_type_to_string(expr) ((expr) == sem_type_int ? "int" : ((expr) == sem_type_float ? "float" : "bool"))

// void handle_variable_attribution(ast_t *self, ast_t *identifier, ast_t *expr);

#endif // !TYPE
