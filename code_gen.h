#pragma once

#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include "structs.h"

iloc_program_t *iloc_program_new();

iloc_instruction_t iloc_instruction_new(iloc_instruction_type_t type, uint64_t r1, uint64_t r2, uint64_t r3);

void iloc_program_push(iloc_program_t *program, iloc_instruction_t instruction);

void iloc_program_append(iloc_program_t *dest, iloc_program_t *src);

void iloc_program_free(iloc_program_t *program);

uint64_t iloc_next_id();

void iloc_instruction_to_string(iloc_instruction_t *instruction);

void iloc_program_to_string(iloc_program_t *program);

ast_t *ast_new(ast_label_t label);

void ast_push(ast_t *parent, ast_t *child);

scope_t *scope_new(scope_t *parent);

void scope_free(scope_t *scope);

/********************\
* Reduction Handlers *
\********************/
void reduce_push_scope();
void reduce_pop_scope();
ast_t *reduce_program(ast_t *global_list);
ast_t *reduce_global_list_empty();
ast_t *reduce_global_list_variable(ast_t *global_list, type_t type, list_t *names);
ast_t *reduce_global_list_function(ast_t *global_list, ast_t *function_header, ast_t *commands);
ast_t *reduce_function_header(list_t *parameters, type_t type, lexeme_t *name);
ast_t *reduce_variable(type_t type, lexeme_t *name);
ast_t *reduce_command_empty();
ast_t *reduce_command_variable(ast_t *commands, type_t type, list_t *names);
ast_t *reduce_command_assignment(ast_t *commands, lexeme_t *name, ast_t *expr);
ast_t *reduce_command_call(ast_t *commands, lexeme_t *name, list_t *arguments);
ast_t *reduce_command_return(ast_t *commands, ast_t *expr);
ast_t *reduce_command_if_else(ast_t *commands, ast_t *cond, ast_t *then_block, ast_t *else_block);
ast_t *reduce_command_if(ast_t *commands, ast_t *cond, ast_t *then_block);
ast_t *reduce_command_while(ast_t *commands, ast_t *cond, ast_t *block);
ast_t *reduce_command_block(ast_t *commands, ast_t *block);
ast_t *reduce_expr_or(ast_t *left, ast_t *right);
ast_t *reduce_expr_and(ast_t *left, ast_t *right);
ast_t *reduce_expr_eq(ast_t *left, ast_t *right);
ast_t *reduce_expr_ne(ast_t *left, ast_t *right);
ast_t *reduce_expr_lt(ast_t *left, ast_t *right);
ast_t *reduce_expr_gt(ast_t *left, ast_t *right);
ast_t *reduce_expr_le(ast_t *left, ast_t *right);
ast_t *reduce_expr_ge(ast_t *left, ast_t *right);
ast_t *reduce_expr_add(ast_t *left, ast_t *right);
ast_t *reduce_expr_sub(ast_t *left, ast_t *right);
ast_t *reduce_expr_mul(ast_t *left, ast_t *right);
ast_t *reduce_expr_div(ast_t *left, ast_t *right);
ast_t *reduce_expr_mod(ast_t *left, ast_t *right);
ast_t *reduce_expr_inv(ast_t *expr);
ast_t *reduce_expr_not(ast_t *expr);
ast_t *reduce_expr_literal(lexeme_t *literal);
ast_t *reduce_expr_call(lexeme_t *literal, list_t *arguments);

