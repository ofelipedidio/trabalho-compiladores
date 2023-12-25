#include "code_gen.h"
#include "list.h"
#include "structs.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

uint64_t last_id = 0;

uint64_t iloc_next_id() {
    return last_id++;
}

iloc_program_t *iloc_program_new() {
    iloc_program_t *iloc_program = (iloc_program_t*) malloc(sizeof(iloc_program_t));
    if (iloc_program == NULL) {
        fprintf(stderr, "Failed to allocate memory for iloc_program_t (errno = %d) [at file \"" __FILE__ "\", line %d]\n", errno, __LINE__-2);
        return NULL;
    }
    iloc_instruction_t* iloc_instructions = (iloc_instruction_t*) malloc(0*sizeof(iloc_instruction_t));
    if (iloc_instructions == NULL) {
        free(iloc_program);
        fprintf(stderr, "Failed to allocate memory for iloc_instruction_t* (errno = %d) [at file \"" __FILE__ "\", line %d]\n", errno, __LINE__-2);
        return NULL;
    }
    iloc_program->instructions = iloc_instructions;
    iloc_program->length = 0;
    return iloc_program;
}

iloc_instruction_t iloc_instruction_new(iloc_instruction_type_t type, uint64_t r1, uint64_t r2, uint64_t r3) {
    iloc_instruction_t instruction;
    instruction.instruction = type;
    instruction.r1 = r1;
    instruction.r2 = r2;
    instruction.r3 = r3;
    return instruction;
}

void iloc_program_push(iloc_program_t *program, iloc_instruction_t instruction) {
    iloc_instruction_t *new_instructions = realloc(program->instructions, (program->length+1)*sizeof(iloc_instruction_t));
    program->instructions = new_instructions;
    program->instructions[program->length] = instruction;
    program->length++;
}

void iloc_program_append(iloc_program_t *dest, iloc_program_t *src) {
    iloc_instruction_t *new_instructions = realloc(dest->instructions, (dest->length+src->length)*sizeof(iloc_instruction_t));
    dest->instructions = new_instructions;
    memcpy(&dest->instructions[dest->length], src->instructions, src->length * sizeof(iloc_instruction_t));
    dest->length += src->length;
}

void iloc_program_free(iloc_program_t *program) {
    free(program->instructions);
    free(program);
}

void iloc_instruction_to_string(iloc_instruction_t *instruction) {
    switch (instruction->instruction) {
        case nop:
            fprintf(stdout, "nop\n"); // does nothing
            break;
        case add:
            fprintf(stdout, "add r%ld, r%ld => r%ld\n", instruction->r1, instruction->r2, instruction->r3); // r3 = r1 + r2
            break;
        case sub:
            fprintf(stdout, "sub r%ld, r%ld => r%ld\n", instruction->r1, instruction->r2, instruction->r3); // r3 = r1 - r2
            break;
        case mult:
            fprintf(stdout, "mult r%ld, r%ld => r%ld\n", instruction->r1, instruction->r2, instruction->r3); // r3 = r1 * r2
            break;
        case _div:
            fprintf(stdout, "div r%ld, r%ld => r%ld\n", instruction->r1, instruction->r2, instruction->r3); // r3 = r1 / r2
            break;
        case add_i:
            fprintf(stdout, "addI r%ld, %ld => r%ld\n", instruction->r1, instruction->r2, instruction->r3); // r3 = r1 + c2
            break;
        case sub_i:
            fprintf(stdout, "subI r%ld, %ld => r%ld\n", instruction->r1, instruction->r2, instruction->r3); // r3 = r1 - c2
            break;
        case rsub_i:
            fprintf(stdout, "rsubI r%ld, %ld => r%ld\n", instruction->r1, instruction->r2, instruction->r3); // r3 = c2 - r1
            break;
        case mult_i:
            fprintf(stdout, "multI r%ld, %ld => r%ld\n", instruction->r1, instruction->r2, instruction->r3); // r3 = r1 * c2
            break;
        case div_i:
            fprintf(stdout, "divI r%ld, %ld => r%ld\n", instruction->r1, instruction->r2, instruction->r3); // r3 = r1 / c2
            break;
        case rdiv_i:
            fprintf(stdout, "rdivI r%ld, %ld => r%ld\n", instruction->r1, instruction->r2, instruction->r3); // r3 = c2 / r1
            break;
        case lshift:
            fprintf(stdout, "lshift r%ld, r%ld => r%ld\n", instruction->r1, instruction->r2, instruction->r3); // r3 = r1 << r2
            break;
        case lshift_i:
            fprintf(stdout, "lshiftI r%ld, %ld => r%ld\n", instruction->r1, instruction->r2, instruction->r3); // r3 = r1 << c2
            break;
        case rshift:
            fprintf(stdout, "rshift r%ld, r%ld => r%ld\n", instruction->r1, instruction->r2, instruction->r3); // r3 = r1 >> r2
            break;
        case rshift_i:
            fprintf(stdout, "rshiftI r%ld, %ld => r%ld\n", instruction->r1, instruction->r2, instruction->r3); // r3 = r1 >> c2
            break;
        case load:
            fprintf(stdout, "load r%ld => r%ld\n", instruction->r1, instruction->r2); // r2 = Memoria(r1)
            break;
        case load_ai:
            fprintf(stdout, "loadAI r%ld, %ld => r%ld\n", instruction->r1, instruction->r2, instruction->r3); // r3 = Memoria(r1 + c2)
            break;
        case load_a0:
            fprintf(stdout, "loadA0 r%ld, r%ld => r%ld\n", instruction->r1, instruction->r2, instruction->r3); // r3 = Memoria(r1 + r2)
            break;
        case cload:
            fprintf(stdout, "cload r%ld => r%ld\n", instruction->r1, instruction->r2); // caractere load
            break;
        case cload_ai:
            fprintf(stdout, "cloadAI r%ld, %ld => r%ld\n", instruction->r1, instruction->r2, instruction->r3); // caractere loadAI
            break;
        case cload_a0:
            fprintf(stdout, "cloadA0 r%ld, r%ld => r%ld\n", instruction->r1, instruction->r2, instruction->r3); // caractere loadA0
            break;
        case load_i:
            fprintf(stdout, "loadI %ld => r%ld\n", instruction->r1, instruction->r2); // r2 = c1
            break;
        case store:
            fprintf(stdout, "store r%ld => r%ld\n", instruction->r1, instruction->r2); // Memoria(r2) = r1
            break;
        case store_ai:
            fprintf(stdout, "storeAI r%ld => r%ld, %ld\n", instruction->r1, instruction->r2, instruction->r3); // Memoria(r2 + c3) = r1
            break;
        case store_ao:
            fprintf(stdout, "storeAO r%ld => r%ld, r%ld\n", instruction->r1, instruction->r2, instruction->r3); // Memoria(r2 + r3) = r1
            break;
        case cstore:
            fprintf(stdout, "cstore r%ld => r%ld\n", instruction->r1, instruction->r2); // caractere store
            break;
        case cstore_ai:
            fprintf(stdout, "cstoreAI r%ld => r%ld, %ld\n", instruction->r1, instruction->r2, instruction->r3); // caractere storeAI
            break;
        case cstore_ao:
            fprintf(stdout, "cstoreAO r%ld => r%ld, r%ld\n", instruction->r1, instruction->r2, instruction->r3); // caractere storeAO
            break;
        case i2i:
            fprintf(stdout, "i2i r%ld => r%ld\n", instruction->r1, instruction->r2); // r2 = r1 para inteiros
            break;
        case c2c:
            fprintf(stdout, "c2c r%ld => r%ld\n", instruction->r1, instruction->r2); // r2 = r1 para caracteres
            break;
        case c2i:
            fprintf(stdout, "c2i r%ld => r%ld\n", instruction->r1, instruction->r2); // converte um caractere para um inteiro
            break;
        case i2c:
            fprintf(stdout, "i2c r%ld => r%ld\n", instruction->r1, instruction->r2); // converte um inteiro para caractere
            break;
        case cmp_lt:
            fprintf(stdout, "cmp_LT r%ld, r%ld -> r%ld\n", instruction->r1, instruction->r2, instruction->r3); // r3 = true se r1 < r2, senão r3 = false
            break;
        case cmp_le:
            fprintf(stdout, "cmp_LE r%ld, r%ld -> r%ld\n", instruction->r1, instruction->r2, instruction->r3); // r3 = true se r1 <= r2, senão r3 = false
            break;
        case cmp_eq:
            fprintf(stdout, "cmp_EQ r%ld, r%ld -> r%ld\n", instruction->r1, instruction->r2, instruction->r3); // r3 = true se r1 = r2, senão r3 = false
            break;
        case cmp_ge:
            fprintf(stdout, "cmp_GE r%ld, r%ld -> r%ld\n", instruction->r1, instruction->r2, instruction->r3); // r3 = true se r1 >= r2, senão r3 = false
            break;
        case cmp_gt:
            fprintf(stdout, "cmp_GT r%ld, r%ld -> r%ld\n", instruction->r1, instruction->r2, instruction->r3); // r3 = true se r1 > r2, senão r3 = false
            break;
        case cmp_ne:
            fprintf(stdout, "cmp_NE r%ld, r%ld -> r%ld\n", instruction->r1, instruction->r2, instruction->r3); // r3 = true se r1 != r2, senão r3 = false
            break;
        case cbr:
            fprintf(stdout, "cbr r%ld -> L%ld, L%ld\n", instruction->r1, instruction->r2, instruction->r3); // PC = endereço(l2) se r1 = true, senão PC = endereço(l3)
            break;
        case jump_i:
            fprintf(stdout, "jumpI -> L%ld\n", instruction->r1); // PC = endereço(l1)
            break;
        case jump:
            fprintf(stdout, "jump -> r%ld\n", instruction->r1); // PC = r1
            break;
        case label:
            fprintf(stdout, "L%ld:\n", instruction->r1); // PC = r1
            break;
        default:
            fprintf(stderr, "Could not print a instruction\n");
            break;
    }
}

void iloc_program_to_string(iloc_program_t *program) {
    for (uint64_t i = 0; i < program->length; i++) {
        iloc_instruction_to_string(&program->instructions[i]);
    }
}

#define AST_INITIAL_LENGTH 2

ast_t *ast_new(ast_label_t label) {
    ast_t *ast = (ast_t*) malloc(sizeof(ast_t));
    if (ast == NULL) {
        fprintf(stderr, "ERROR: Failed to allocate memory for ast_t (errno = %d) [at file \"" __FILE__ "\", line %d]\n", errno, __LINE__-2);
        exit(EXIT_FAILURE);
    }
    ast_t** children = (ast_t**) malloc(AST_INITIAL_LENGTH*sizeof(ast_t*));
    if (children == NULL) {
        free(ast);
        fprintf(stderr, "ERROR: Failed to allocate memory for ast_t* (errno = %d) [at file \"" __FILE__ "\", line %d]\n", errno, __LINE__-2);
        exit(EXIT_FAILURE);
    }
    ast->length = 0;
    ast->capacity = AST_INITIAL_LENGTH;
    ast->label = label;
    ast->children = children;
    ast->lexeme = NULL;
    ast->type = type_undefined;
    ast->program = iloc_program_new();
    return ast;
}

void ast_push(ast_t *parent, ast_t *child) {
    while (parent->length+1 >= parent->capacity) {
        // new_capacity = round_up(3/2 * capacity);
        uint64_t new_capacity = (parent->capacity * 3 + 1) / 2;
        ast_t **new_children = realloc(parent->children, new_capacity * sizeof(void*));
        if (new_children == NULL) {
            fprintf(stderr, "ERROR: Failed to reallocate memory for void* (errno = %d) [at file \"" __FILE__ "\", line %d]\n", errno, __LINE__-2);
            exit(EXIT_FAILURE);
        }
        parent->children = new_children;
        parent->capacity = new_capacity;
    }
    parent->children[parent->length] = child;
    parent->length++;
}

scope_t *current_scope = NULL;

void reduce_push_scope() {
    current_scope = scope_new(current_scope);
}

void reduce_pop_scope() {
    scope_t *temp_scope = current_scope;
    current_scope = current_scope->parent;
    scope_free(temp_scope);
}

ast_t *reduce_program(ast_t *global_list) {
    return global_list;
}

ast_t *reduce_global_list_empty() {
    return ast_new(ast_global_list);
}

ast_t *reduce_global_list_variable(ast_t *global_list, type_t type, list_t *names) {
    // Iterates over names on the list and takes ownership of the items
    list_iterate(names, i) {
        lexeme_t *lexeme = list_get_as(names, i, lexeme_t);
        ast_t *node = ast_new(ast_var_decl);
        node->type = type;
        node->lexeme = lexeme;
        ast_push(global_list, node);
    }
    // Frees the list, but not the items
    list_free(names);
    return global_list;
}

ast_t *reduce_global_list_function(ast_t *global_list, ast_t *function_header, ast_t *commands) {
    ast_t *node = ast_new(ast_func_decl);
    ast_push(node, function_header);
    ast_push(node, commands);
    ast_push(global_list, node);
    return global_list;
}

ast_t *reduce_function_header(list_t *parameters, type_t type, lexeme_t *name) {
    ast_t *header = ast_new(ast_func_header);
    header->type = type;
    header->lexeme = name;
    list_iterate(parameters, i) {
        ast_t *argument = list_get_as(parameters, i, ast_t);
        ast_push(header, argument);
    }
    return header;
}

ast_t *reduce_variable(type_t type, lexeme_t *name) {
    ast_t *variable = ast_new(ast_var_decl);
    variable->type = type;
    variable->lexeme = name;
    return variable;
}

ast_t *reduce_command_empty() {
    return ast_new(ast_command_list);
}

ast_t *reduce_command_variable(ast_t *commands, type_t type, list_t *names) {
    list_iterate(names, i) {
        ast_t *variable = ast_new(ast_var_decl);
        variable->type = type;
        variable->lexeme = list_get_as(names, i, lexeme_t);
        ast_push(commands, variable);
    }
    return commands;
}

ast_t *reduce_command_assignment(ast_t *commands, lexeme_t *name, ast_t *expr) {
    ast_t *assignment = ast_new(ast_assignment);
    assignment->type = expr->type;
    ast_push(assignment, expr);
    ast_push(commands, assignment);
    return commands;
}

ast_t *reduce_command_call(ast_t *commands, lexeme_t *name, list_t *arguments) {
    ast_t *call = ast_new(ast_call);
    // TODO - Didio: get type
    // call->type = type;
    call->lexeme = name;
    list_iterate(arguments, i) {
        ast_t *node = list_get_as(arguments, i, ast_t);
        ast_push(call, node);
    }
    ast_push(commands, call);
    return commands;
}

ast_t *reduce_command_return(ast_t *commands, ast_t *expr) {
    ast_t *return_ = ast_new(ast_return);
    return_->type = expr->type;
    ast_push(return_, expr);
    ast_push(commands, return_);
    return commands;
}

ast_t *reduce_command_if_else(ast_t *commands, ast_t *cond, ast_t *then_block, ast_t *else_block) {
    ast_t *if_ = ast_new(ast_if);
    if_->type = cond->type;
    ast_push(if_, cond);
    ast_push(if_, then_block);
    ast_push(if_, else_block);
    ast_push(commands, if_);
    return commands;
}

ast_t *reduce_command_if(ast_t *commands, ast_t *cond, ast_t *then_block) {
    ast_t *if_ = ast_new(ast_if);
    if_->type = cond->type;
    ast_push(if_, cond);
    ast_push(if_, then_block);
    ast_push(if_, NULL);
    ast_push(commands, if_);
    return commands;
}

ast_t *reduce_command_while(ast_t *commands, ast_t *cond, ast_t *block) {
    ast_t *while_ = ast_new(ast_while);
    while_->type = cond->type;
    ast_push(while_, cond);
    ast_push(while_, block);
    ast_push(commands, while_);
    return commands;
}

ast_t *reduce_command_block(ast_t *commands, ast_t *block) {
    ast_push(commands, block);
    return commands;
}

ast_t *reduce_expr_or(ast_t *left, ast_t *right) {
    ast_t *new_expr = ast_new(ast_expr_or);
    ast_push(new_expr, left);
    ast_push(new_expr, right);
    return new_expr;
}

ast_t *reduce_expr_and(ast_t *left, ast_t *right) {
    ast_t *new_expr = ast_new(ast_expr_and);
    ast_push(new_expr, left);
    ast_push(new_expr, right);
    return new_expr;
}

ast_t *reduce_expr_eq(ast_t *left, ast_t *right) {
    ast_t *new_expr = ast_new(ast_expr_eq);
    ast_push(new_expr, left);
    ast_push(new_expr, right);
    return new_expr;
}

ast_t *reduce_expr_ne(ast_t *left, ast_t *right) {
    ast_t *new_expr = ast_new(ast_expr_ne);
    ast_push(new_expr, left);
    ast_push(new_expr, right);
    return new_expr;
}

ast_t *reduce_expr_lt(ast_t *left, ast_t *right) {
    ast_t *new_expr = ast_new(ast_expr_lt);
    ast_push(new_expr, left);
    ast_push(new_expr, right);
    return new_expr;
}

ast_t *reduce_expr_gt(ast_t *left, ast_t *right) {
    ast_t *new_expr = ast_new(ast_expr_gt);
    ast_push(new_expr, left);
    ast_push(new_expr, right);
    return new_expr;
}

ast_t *reduce_expr_le(ast_t *left, ast_t *right) {
    ast_t *new_expr = ast_new(ast_expr_le);
    ast_push(new_expr, left);
    ast_push(new_expr, right);
    return new_expr;
}

ast_t *reduce_expr_ge(ast_t *left, ast_t *right) {
    ast_t *new_expr = ast_new(ast_expr_ge);
    ast_push(new_expr, left);
    ast_push(new_expr, right);
    return new_expr;
}

ast_t *reduce_expr_add(ast_t *left, ast_t *right) {
    ast_t *new_expr = ast_new(ast_expr_add);
    ast_push(new_expr, left);
    ast_push(new_expr, right);
    return new_expr;
}

ast_t *reduce_expr_sub(ast_t *left, ast_t *right) {
    ast_t *new_expr = ast_new(ast_expr_sub);
    ast_push(new_expr, left);
    ast_push(new_expr, right);
    return new_expr;
}

ast_t *reduce_expr_mul(ast_t *left, ast_t *right) {
    ast_t *new_expr = ast_new(ast_expr_mul);
    ast_push(new_expr, left);
    ast_push(new_expr, right);
    return new_expr;
}

ast_t *reduce_expr_div(ast_t *left, ast_t *right) {
    ast_t *new_expr = ast_new(ast_expr_div);
    ast_push(new_expr, left);
    ast_push(new_expr, right);
    return new_expr;
}

ast_t *reduce_expr_mod(ast_t *left, ast_t *right) {
    ast_t *new_expr = ast_new(ast_expr_mod);
    ast_push(new_expr, left);
    ast_push(new_expr, right);
    return new_expr;
}

ast_t *reduce_expr_inv(ast_t *expr) {
    ast_t *new_expr = ast_new(ast_expr_inv);
    ast_push(new_expr, expr);
    return new_expr;
}

ast_t *reduce_expr_not(ast_t *expr) {
    ast_t *new_expr = ast_new(ast_expr_not);
    ast_push(new_expr, expr);
    return new_expr;
}

ast_t *reduce_expr_literal(lexeme_t *literal) {
    ast_t *expr = ast_new(ast_val_lit);
    expr->lexeme = literal;
    return expr;
}

ast_t *reduce_expr_call(lexeme_t *literal, list_t *arguments) {
    ast_t *call = ast_new(ast_call);
    // TODO - Didio: get type
    // call->type = type;
    call->lexeme = literal;
    list_iterate(arguments, i) {
        ast_t *node = list_get_as(arguments, i, ast_t);
        ast_push(call, node);
    }
    ast_push(commands, call);
    return commands;
}


