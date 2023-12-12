#ifndef AST
#define AST

#include "lexeme.h"
#include <inttypes.h>
#include <errno.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* ##############
 * # Structures #
 * ############## */

enum bin_op {
    op_mul,
    op_div,
    op_mod,
    op_add,
    op_sub,
    op_lt,
    op_gt,
    op_le,
    op_ge,
    op_eq,
    op_ne,
    op_and,
    op_or,
};

enum un_op {
    op_not,
    op_inv,
};

enum ast_type {
    ast_int,
    ast_float,
    ast_bool,
};

struct ast_program;
struct ast_global;
struct ast_function;
struct ast_parameters;
struct ast_variable;
struct ast_variable_names;
struct ast_command;
struct ast_attribution;
struct ast_call;
struct ast_arguments;
struct ast_return;
struct ast_if;
struct ast_while;
struct ast_block;
struct ast_expression;
struct ast_bin_op;
struct ast_un_op;
struct ast_literal;
struct ast_identifier;
struct ast_int;
struct ast_float;
struct ast_bool;

typedef struct ast_program        program_t;
typedef struct ast_global         global_t;
typedef struct ast_function       function_t;
typedef struct ast_parameters     parameters_t;
typedef struct ast_variable       variable_t;
typedef struct ast_variable_names variable_names_t;
typedef struct ast_command        command_t;
typedef struct ast_attribution    attribution_t;
typedef struct ast_call           call_t;
typedef struct ast_arguments      arguments_t;
typedef struct ast_return         return_t;
typedef struct ast_if             if_t;
typedef struct ast_while          while_t;
typedef struct ast_block          block_t;
typedef struct ast_expression     expression_t;
typedef struct ast_bin_op         bin_op_t;
typedef struct ast_un_op          un_op_t;
typedef struct ast_literal        literal_t;
typedef struct ast_int            ast_int_t;
typedef struct ast_float          ast_float_t;
typedef struct ast_bool           ast_bool_t;
typedef struct ast_identifier     identifier_t;
typedef enum   ast_type           ast_type_t;
typedef enum   bin_op             bin_op;
typedef enum   un_op              un_op;

struct ast_program {
    global_t **globals;
    uint64_t len;
};

struct ast_global {
    function_t *function;
    variable_t *variable;
};

struct ast_function {
    identifier_t *name;
    parameters_t *parameters;
    ast_type_t type;
    block_t *body;
};

struct ast_parameters {
    variable_t **variables;
    uint64_t len;
};

struct ast_variable {
    ast_type_t type;
    variable_names_t *names;
};

struct ast_variable_names {
    identifier_t **names;
    uint64_t len;
};

struct ast_command {
    variable_t *variable;
    attribution_t *attribution;
    call_t *call;
    return_t *_return;
    if_t *_if;
    while_t *_while;
    block_t *block;
};

struct ast_attribution {
    identifier_t *variable_name;
    expression_t *expression;
};

struct ast_call {
    identifier_t *function_name;
    arguments_t *arguments;
};

struct ast_arguments {
    expression_t **arguments;
    uint64_t len;
};

struct ast_return {
    expression_t *expression;
};

struct ast_if {
    expression_t *condition;
    block_t *then_block;
    block_t *else_block;
};

struct ast_while {
    expression_t *condition;
    block_t *block;
};

struct ast_block {
    command_t **commands;
    uint64_t len;
};

struct ast_expression {
    bin_op_t *_bin_op;
    un_op_t *_un_op;
    call_t *call;
    literal_t *literal;
    identifier_t *identifier;
};

struct ast_bin_op {
    bin_op op;
    expression_t *left;
    expression_t *right;
};

struct ast_un_op {
    un_op op;
    expression_t *expression;
};

struct ast_literal {
    ast_int_t *_int;
    ast_float_t *_float;
    ast_bool_t *_bool;
};

struct ast_int {
    int value;
    lexeme_t lexeme;
};

struct ast_float {
    float value;
    lexeme_t lexeme;
};

struct ast_bool {
    int value;
    lexeme_t lexeme;
};

struct ast_identifier {
    char *text;
    uint64_t len;
    lexeme_t lexeme;
};

/* ################
 * # Constructors #
 * ################ */
program_t *ast_program_new();
global_t *ast_global_new_function(function_t *function);
global_t *ast_global_new_variable(variable_t *variable);
function_t *ast_function_new(identifier_t *name, ast_type_t type, parameters_t *parameters, block_t *body);
parameters_t *ast_parameters_new();
variable_t *ast_variable_new(ast_type_t type, variable_names_t *names);
variable_names_t *ast_variable_names_new();
command_t *ast_command_new_variable(variable_t *variable);
command_t *ast_command_new_attribution(attribution_t *attribution);
command_t *ast_command_new_call(call_t *call);
command_t *ast_command_new_return(return_t *_return);
command_t *ast_command_new_if(if_t *_if);
command_t *ast_command_new_while(while_t *_while);
command_t *ast_command_new_block(block_t *block);
attribution_t *ast_attribution_new(identifier_t *variable_name, expression_t *expression);
call_t *ast_call_new(identifier_t *function_name, arguments_t *arguments);
arguments_t *ast_arguments_new();
return_t *ast_return_new(expression_t *expression);
if_t *ast_if_new(expression_t *condition, block_t *then_block, block_t *else_block);
while_t *ast_while_new(expression_t *condition, block_t *block);
block_t *ast_block_new();
expression_t *ast_expression_new_bin_op(bin_op_t *_bin_op);
expression_t *ast_expression_new_un_op(un_op_t *_un_op);
expression_t *ast_expression_new_call(call_t *call);
expression_t *ast_expression_new_identifier(identifier_t *identifier);
expression_t *ast_expression_new_literal(literal_t *literal);
bin_op_t *ast_bin_op_new(bin_op op, expression_t *left, expression_t *right);
un_op_t *ast_un_op_new(un_op op, expression_t *expression);
literal_t *ast_literal_new_int(ast_int_t *_int);
literal_t *ast_literal_new_float(ast_float_t *_float);
literal_t *ast_literal_new_bool(ast_bool_t *_bool);
ast_int_t *ast_int_new(int value, lexeme_t lexeme);
ast_float_t *ast_float_new(float value, lexeme_t lexeme);
ast_bool_t *ast_bool_new_true(lexeme_t lexeme);
ast_bool_t *ast_bool_new_false(lexeme_t lexeme);
identifier_t *ast_identifier_new(char *text, lexeme_t lexeme);

/* ###############
 * # Destructors #
 * ############### */
void ast_program_free(program_t *program);
void ast_global_free(global_t *global);
void ast_function_free(function_t *function);
void ast_parameters_free(parameters_t *parameters);
void ast_variable_free(variable_t *variable);
void ast_variable_names_free(variable_names_t *names);
void ast_command_free(command_t *command);
void ast_attribution_free(attribution_t *attribution);
void ast_call_free(call_t *call);
void ast_arguments_free(arguments_t *arguments);
void ast_return_free(return_t *_return);
void ast_if_free(if_t *_if);
void ast_while_free(while_t *_while);
void ast_block_free(block_t *block);
void ast_expression_free(expression_t *expression);
void ast_bin_op_free(bin_op_t *bin_op);
void ast_un_op_free(un_op_t *un_op);
void ast_literal_free(literal_t *literal);
void ast_int_free(ast_int_t *_int);
void ast_float_free(ast_float_t *_float);
void ast_bool_free(ast_bool_t *_bool);
void ast_identifier_free(identifier_t *identifier);

/* ###########
 * # Methods #
 * ########### */

void ast_program_add_child(program_t *program, global_t *global);
void ast_variable_names_add_child(variable_names_t *variable_names, identifier_t *name);
void ast_parameters_add_child(parameters_t *parameters, variable_t *variable);
void ast_arguments_add_child(arguments_t *arguments, expression_t *argument);
void ast_block_add_child(block_t *block, command_t *command);

void *ast_program_export(program_t *program);
void *ast_global_export(global_t *global);
void *ast_function_export(function_t *function);
void *ast_parameters_export(parameters_t *parameters);
void *ast_variable_export(variable_t *variable);
void *ast_variable_names_export(variable_names_t *names);
void *ast_command_export(command_t *command);
void *ast_attribution_export(attribution_t *attribution);
void *ast_call_export(call_t *call);
void *ast_arguments_export(arguments_t *arguments);
void *ast_return_export(return_t *_return);
void *ast_if_export(if_t *_if);
void *ast_while_export(while_t *_while);
void *ast_block_export(block_t *block);
void *ast_expression_export(expression_t *expression);
void *ast_bin_op_export(bin_op_t *bin_op);
void *ast_un_op_export(un_op_t *un_op);
void *ast_literal_export(literal_t *literal);
void *ast_int_export(ast_int_t *_int);
void *ast_float_export(ast_float_t *_float);
void *ast_bool_export(ast_bool_t *_bool);
void *ast_identifier_export(identifier_t *identifier);

#endif // !AST
