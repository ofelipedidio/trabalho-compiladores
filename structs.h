#pragma once

#include <inttypes.h>
#include "list.h"

/*********\
* Generic *
\*********/
typedef enum {
    type_undefined,
    type_int,
    type_float,
    type_bool
} type_t;

/******************\
* Lexical Analysis *
\******************/
typedef enum {
    lex_ident,
    lex_int,
    lex_float,
    lex_bool,
} lexeme_type_t;

typedef union {
    struct {
        lexeme_type_t type;
        uint64_t line;
        uint64_t column;
        char *value;
    } lex_ident_t;
    struct {
        lexeme_type_t type;
        uint64_t line;
        uint64_t column;
        uint64_t value;
    } lex_int_t;
    struct {
        lexeme_type_t type;
        uint64_t line;
        uint64_t column;
        double value;
    } lex_float_t;
    struct {
        lexeme_type_t type;
        uint64_t line;
        uint64_t column;
        int value;
    } lex_bool_t;
} lexeme_t;

/*******************\
* Semantic Analysis *
\*******************/
typedef enum {
    nat_literal,
    nat_identifier,
    nat_function,
} nature_t;

typedef enum {
    rfp,  // Points to the base of the current activation record
    rsp,  // Points to the top of the stack
    rbss, // Points to the base of the data segment
    rpc,  // Program counter
} iloc_register_t;

typedef struct {
    nature_t nature;
    type_t type;
    lexeme_t *lexeme;
    uint64_t line;
    uint64_t column;
    uint64_t offset;
    uint64_t function_label;
    iloc_register_t base_register;
} name_entry_t;

typedef struct scope {
    struct scope *parent;
    list_t *entries;
    uint64_t size;
    uint64_t total_size;
    char *scope_name;
} scope_t;

/******************************\
* Intermediate Code Generation *
\******************************/
typedef enum {
    nop,          // nop                      // does nothing
    
    // Arithmetic
    add,          // add r1, r2 => r3         // r3 = r1 + r2
    sub,          // sub r1, r2 => r3         // r3 = r1 - r2
    mult,         // mult r1, r2 => r3        // r3 = r1 * r2
    _div,         // div r1, r2 => r3         // r3 = r1 / r2
    add_i,        // addI r1, c2 => r3        // r3 = r1 + c2
    sub_i,        // subI r1, c2 => r3        // r3 = r1 - c2
    rsub_i,       // rsubI r1, c2 => r3       // r3 = c2 - r1
    mult_i,       // multI r1, c2 => r3       // r3 = r1 * c2
    div_i,        // divI r1, c2 => r3        // r3 = r1 / c2
    rdiv_i,       // rdivI r1, c2 => r3       // r3 = c2 / r1
    
    // Shift
    lshift,       // lshift r1, r2 => r3      // r3 = r1 << r2
    lshift_i,     // lshiftI r1, c2 => r3     // r3 = r1 << c2
    rshift,       // rshift r1, r2 => r3      // r3 = r1 >> r2
    rshift_i,     // rshiftI r1, c2 => r3     // r3 = r1 >> c2
    
    // Load
    load,         // load r1 => r2            // r2 = Memoria(r1)
    load_ai,      // loadAI r1, c2 => r3      // r3 = Memoria(r1 + c2)
    load_ai_r,    // loadAI reg(r1), c2 => r3 // r3 = Memoria(reg(r1) + c2)
    load_a0,      // loadA0 r1, r2 => r3      // r3 = Memoria(r1 + r2)
    cload,        // cload r1 => r2           // caractere load
    cload_ai,     // cloadAI r1, c2 => r3     // caractere loadAI
    cload_a0,     // cloadA0 r1, r2 => r3     // caractere loadA0
    load_i,       // loadI c1 => r2           // r2 = c1
    
    // Store
    store,        // store r1 => r2           // Memoria(r2) = r1
    store_ai,     // storeAI r1 => r2, c3     // Memoria(r2 + c3) = r1
    store_ai_r,   // storeAI r1 => reg(r2), c3// Memoria(reg(r2) + c3) = r1
    store_ao,     // storeAO r1 => r2, r3     // Memoria(r2 + r3) = r1
    cstore,       // cstore r1 => r2          // caractere store
    cstore_ai,    // cstoreAI r1 => r2, c3    // caractere storeAI
    cstore_ao,    // cstoreAO r1 => r2, r3    // caractere storeAO
    
    // Copy
    i2i,          // i2i r1 => r2             // r2 = r1 para inteiros
    c2c,          // c2c r1 => r2             // r2 = r1 para caracteres
    c2i,          // c2i r1 => r2             // converte um caractere para um inteiro
    i2c,          // i2c r1 => r2             // converte um inteiro para caractere
    
    // Comparisons
    cmp_lt,       // cmp_LT r1, r2 -> r3      // r3 = true se r1 < r2, senão r3 = false
    cmp_le,       // cmp_LE r1, r2 -> r3      // r3 = true se r1 <= r2, senão r3 = false
    cmp_eq,       // cmp_EQ r1, r2 -> r3      // r3 = true se r1 = r2, senão r3 = false
    cmp_ge,       // cmp_GE r1, r2 -> r3      // r3 = true se r1 >= r2, senão r3 = false
    cmp_gt,       // cmp_GT r1, r2 -> r3      // r3 = true se r1 > r2, senão r3 = false
    cmp_ne,       // cmp_NE r1, r2 -> r3      // r3 = true se r1 != r2, senão r3 = false
    cbr,          // cbr r1 -> l2, l3         // PC = endereço(l2) se r1 = true, senão PC = endereço(l3)
    
    // Jump
    jump_i,       // jumpI -> l1              // PC = endereço(l1)
    jump,         // jump -> r1               // PC = r1
    
    label,        // L<r1>:
} iloc_instruction_type_t;

typedef struct {
    iloc_instruction_type_t instruction;
    uint64_t r1;
    uint64_t r2;
    uint64_t r3;
} iloc_instruction_t;

typedef struct {
    iloc_instruction_t *instructions;
    uint64_t length;
} iloc_program_t;

/********************\
* Syntactic Analysis *
\********************/
typedef enum {
    ast_program,
    ast_global_list,
    ast_var_decl,
    ast_func_decl,
    ast_func_header,
    ast_command_list,
    ast_assignment,
    ast_call,
    ast_return,
    ast_if,
    ast_while,
    ast_expr_or,
    ast_expr_and,
    ast_expr_eq,
    ast_expr_ne,
    ast_expr_lt,
    ast_expr_gt,
    ast_expr_le,
    ast_expr_ge,
    ast_expr_add,
    ast_expr_sub,
    ast_expr_mul,
    ast_expr_div,
    ast_expr_mod,
    ast_expr_inv,
    ast_expr_not,
    ast_ident,
    ast_val_int,
    ast_val_float,
    ast_val_bool,
} ast_label_t;

typedef struct ast {
    ast_label_t label;
    struct ast **children;
    uint64_t length;
    uint64_t capacity;
    lexeme_t *lexeme;
    type_t type;
    iloc_program_t *program;
    uint64_t value;
} ast_t;

typedef enum {
    ebx,
    ecx,
    edx,
} x86_register_t;

uint64_t x86_to_int(x86_register_t reg);

x86_register_t int_to_x86(int reg);

// [Ideia do algoritmo de alocacao de registradores]
// passar por tudo
// alocar ebx para r64
// if (program->instructions[3].r1 == 64) {
// program->instructions[3].r1 = x86_to_int(ebx);
// }
void allocated_registers(iloc_program_t *program);

