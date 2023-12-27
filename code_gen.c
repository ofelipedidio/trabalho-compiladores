#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "code_gen.h"
#include "list.h"
#include "structs.h"

/**************\
* Global state *
\**************/
uint64_t last_id = 0;
scope_t *current_scope = NULL;
scope_t *global_scope = NULL;

/******************************\
* Intermediate Code Generation *
\******************************/
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

void iloc_push(iloc_program_t *program, iloc_instruction_type_t type, uint64_t r1, uint64_t r2, uint64_t r3) {
    iloc_instruction_t *new_instructions = realloc(program->instructions, (program->length+1)*sizeof(iloc_instruction_t));
    program->instructions = new_instructions;
    program->instructions[program->length].instruction = type;
    program->instructions[program->length].r1 = r1;
    program->instructions[program->length].r2 = r2;
    program->instructions[program->length].r3 = r3;
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

iloc_register_t id_to_reg(uint64_t id) {
    switch (id) {
        case 0:
            return rfp;
        case 1:
            return rsp;
        case 2:
            return rbss;
        case 3:
            return rsp;
        default:
            return rbss;
    }
}

uint64_t reg_to_id(iloc_register_t reg) {
    switch (reg) {
        case rfp:
            return 0;
        case rsp:
            return 1;
        case rbss:
            return 2;
        case rpc:
            return 3;
    }
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

/********************\
* Syntactic Analysis *
\********************/
#define AST_INITIAL_LENGTH 3

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
    uint64_t new_capacity = parent->capacity;
    while (parent->length+1 >= new_capacity) {
        // new_capacity = round_up(3/2 * capacity);
        new_capacity = (new_capacity * 3 + 1) / 2;
    }
    if (new_capacity > parent->capacity) {
        ast_t **new_children = realloc(parent->children, new_capacity * sizeof(void*));
        if (new_children == NULL) {
            fprintf(stderr, "ERROR: Failed to reallocate memory for ast_t* (errno = %d) [at file \"" __FILE__ "\", line %d]\n", errno, __LINE__-2);
            exit(EXIT_FAILURE);
        }
        parent->children = new_children;
        parent->capacity = new_capacity;
    }
    parent->children[parent->length] = child;
    parent->length++;
}

/********************\
* Reduction Handlers *
\********************/
void reduce_push_scope() {
    current_scope = scope_new(current_scope);
    if (current_scope->parent == NULL) {
        global_scope = current_scope;
    }
}

void reduce_pop_scope() {
    scope_t *temp_scope = current_scope;
    current_scope = current_scope->parent;
    scope_free(temp_scope);
    if (current_scope == NULL) {
        global_scope = NULL;
    }
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
        int var_res = register_variable(current_scope, type, lexeme);
        if (var_res != 0) {
            fprintf(stderr, "- Contexto: na declaracao de variavel global\n");
            fprintf(stderr, "- Contexto: na linha %ld, coluna %ld\n", lexeme->lex_ident_t.line, lexeme->lex_ident_t.column);
            if (var_res == 1) {
                fprintf(stderr, "- Contexto: previamente declarado como variavel\n");
            } else if (var_res == 2) {
                fprintf(stderr, "- Contexto: previamente declarado como funcao\n");
            }
            exit(ERR_DECLARED);
        }
        
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
    node->type = function_header->type;
    ast_push(node, function_header);
    ast_push(node, commands);
    ast_push(global_list, node);
    return global_list;
}

ast_t *reduce_function_header(list_t *parameters, type_t type, lexeme_t *name) {
    int var_res = register_function(current_scope->parent, type, name);
    if (var_res != 0) {
        fprintf(stderr, "- Contexto: na declaracao de funcao\n");
        fprintf(stderr, "- Contexto: na linha %ld, coluna %ld\n", name->lex_ident_t.line, name->lex_ident_t.column);
        if (var_res == 1) {
            fprintf(stderr, "- Contexto: previamente declarado como variavel\n");
        } else if (var_res == 2) {
            fprintf(stderr, "- Contexto: previamente declarado como funcao\n");
        }
        exit(ERR_DECLARED);
    }
    
    ast_t *header = ast_new(ast_func_header);
    header->type = type;
    header->lexeme = name;
    list_iterate(parameters, i) {
        ast_t *argument = list_get_as(parameters, i, ast_t);
        int var_res = register_variable(current_scope, argument->type, argument->lexeme);
        if (var_res != 0) {
            fprintf(stderr, "- Contexto: na declaracao de parametro da funcao \"%s\"\n", name->lex_ident_t.value);
            fprintf(stderr, "- Contexto: na linha %ld, coluna %ld\n", argument->lexeme->lex_ident_t.line, argument->lexeme->lex_ident_t.column);
            if (var_res == 1) {
                fprintf(stderr, "- Contexto: previamente declarado como variavel\n");
            } else if (var_res == 2) {
                fprintf(stderr, "- Contexto: previamente declarado como funcao\n");
            }
            exit(ERR_DECLARED);
        }
        ast_push(header, argument);
    }
    list_free(parameters);
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
        lexeme_t *lexeme = list_get_as(names, i, lexeme_t);
        int var_res = register_variable(current_scope, type, lexeme);
        if (var_res != 0) {
            fprintf(stderr, "- Contexto: dentro da funcao \"%s\"\n", 
                list_get_as(global_scope->entries, global_scope->entries->length-1, name_entry_t)->lexeme->lex_ident_t.value);
            fprintf(stderr, "- Contexto: na linha %ld, coluna %ld\n", lexeme->lex_ident_t.line, lexeme->lex_ident_t.column);
            if (var_res == 1) {
                fprintf(stderr, "- Contexto: previamente declarado como variavel\n");
            } else if (var_res == 2) {
                fprintf(stderr, "- Contexto: previamente declarado como funcao\n");
            }
            exit(ERR_DECLARED);
        }
        ast_t *variable = ast_new(ast_var_decl);
        variable->type = type;
        variable->lexeme = lexeme;
        ast_push(commands, variable);
    }
    list_free(names);
    return commands;
}

ast_t *reduce_command_assignment(ast_t *commands, lexeme_t *name, ast_t *expr) {
    name_entry_t *entry = scope_find(current_scope, name->lex_ident_t.value);
    if (entry == NULL) {
        fprintf(stderr, "ERRO: a variavel \"%s\" foi utilizada antes de ser declarada\n", name->lex_ident_t.value);
        fprintf(stderr, "- Contexto: na linha %ld, coluna %ld\n", name->lex_ident_t.line, name->lex_ident_t.column);
        fprintf(stderr, "- Contexto: em uma atribuicao na funcao \"%s\"\n",
                list_get_as(global_scope->entries, global_scope->entries->length-1, name_entry_t)->lexeme->lex_ident_t.value);
        exit(ERR_UNDECLARED);
    }
    if (entry->nature != nat_identifier) {
        fprintf(stderr, "ERRO: a funcao \"%s\" foi utilizada como variavel\n", name->lex_ident_t.value);
        fprintf(stderr, "- Contexto: na linha %ld, coluna %ld\n", name->lex_ident_t.line, name->lex_ident_t.column);
        fprintf(stderr, "- Contexto: em uma atribuicao na funcao \"%s\"\n",
                list_get_as(global_scope->entries, global_scope->entries->length-1, name_entry_t)->lexeme->lex_ident_t.value);
        exit(ERR_FUNCTION);
    }
    ast_t *assignment = ast_new(ast_assignment);
    assignment->type = expr->type;
    ast_push(assignment, expr);
    ast_push(commands, assignment);
    return commands;
}

ast_t *reduce_command_call(ast_t *commands, lexeme_t *name, list_t *arguments) {
    ast_t *call = ast_new(ast_call);
    name_entry_t *entry = scope_find(current_scope, name->lex_ident_t.value);
    if (entry == NULL) {
        fprintf(stderr, "ERRO: a funcao \"%s\" foi utilizada antes de ser declarada\n", name->lex_ident_t.value);
        fprintf(stderr, "- Contexto: na linha %ld, coluna %ld\n", name->lex_ident_t.line, name->lex_ident_t.column);
        fprintf(stderr, "- Contexto: em uma chamada de funcao na funcao \"%s\"\n",
                list_get_as(global_scope->entries, global_scope->entries->length-1, name_entry_t)->lexeme->lex_ident_t.value);
        exit(ERR_UNDECLARED);
    }
    if (entry->nature != nat_function) {
        fprintf(stderr, "ERRO: a variavel \"%s\" foi utilizada como funcao\n", name->lex_ident_t.value);
        fprintf(stderr, "- Contexto: na linha %ld, coluna %ld\n", name->lex_ident_t.line, name->lex_ident_t.column);
        fprintf(stderr, "- Contexto: em uma chamada de funcao na funcao \"%s\"\n",
                list_get_as(global_scope->entries, global_scope->entries->length-1, name_entry_t)->lexeme->lex_ident_t.value);
        exit(ERR_VARIABLE);
    }
    call->type = entry->type;
    call->lexeme = name;
    list_iterate(arguments, i) {
        ast_t *node = list_get_as(arguments, i, ast_t);
        ast_push(call, node);
    }
    list_free(arguments);
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
    new_expr->type = type_infer(left->type, right->type);
    ast_push(new_expr, left);
    ast_push(new_expr, right);
    return new_expr;
}

ast_t *reduce_expr_and(ast_t *left, ast_t *right) {
    ast_t *new_expr = ast_new(ast_expr_and);
    new_expr->type = type_infer(left->type, right->type);
    ast_push(new_expr, left);
    ast_push(new_expr, right);
    return new_expr;
}

ast_t *reduce_expr_eq(ast_t *left, ast_t *right) {
    ast_t *new_expr = ast_new(ast_expr_eq);
    new_expr->type = type_infer(left->type, right->type);
    ast_push(new_expr, left);
    ast_push(new_expr, right);
    return new_expr;
}

ast_t *reduce_expr_ne(ast_t *left, ast_t *right) {
    ast_t *new_expr = ast_new(ast_expr_ne);
    new_expr->type = type_infer(left->type, right->type);
    ast_push(new_expr, left);
    ast_push(new_expr, right);
    return new_expr;
}

ast_t *reduce_expr_lt(ast_t *left, ast_t *right) {
    ast_t *new_expr = ast_new(ast_expr_lt);
    new_expr->type = type_infer(left->type, right->type);
    ast_push(new_expr, left);
    ast_push(new_expr, right);
    return new_expr;
}

ast_t *reduce_expr_gt(ast_t *left, ast_t *right) {
    ast_t *new_expr = ast_new(ast_expr_gt);
    new_expr->type = type_infer(left->type, right->type);
    ast_push(new_expr, left);
    ast_push(new_expr, right);
    return new_expr;
}

ast_t *reduce_expr_le(ast_t *left, ast_t *right) {
    ast_t *new_expr = ast_new(ast_expr_le);
    new_expr->type = type_infer(left->type, right->type);
    ast_push(new_expr, left);
    ast_push(new_expr, right);
    return new_expr;
}

ast_t *reduce_expr_ge(ast_t *left, ast_t *right) {
    ast_t *new_expr = ast_new(ast_expr_ge);
    new_expr->type = type_infer(left->type, right->type);
    ast_push(new_expr, left);
    ast_push(new_expr, right);
    return new_expr;
}

ast_t *reduce_expr_add(ast_t *left, ast_t *right) {
    ast_t *new_expr = ast_new(ast_expr_add);
    new_expr->type = type_infer(left->type, right->type);
    ast_push(new_expr, left);
    ast_push(new_expr, right);
    return new_expr;
}

ast_t *reduce_expr_sub(ast_t *left, ast_t *right) {
    ast_t *new_expr = ast_new(ast_expr_sub);
    new_expr->type = type_infer(left->type, right->type);
    ast_push(new_expr, left);
    ast_push(new_expr, right);
    return new_expr;
}

ast_t *reduce_expr_mul(ast_t *left, ast_t *right) {
    ast_t *new_expr = ast_new(ast_expr_mul);
    new_expr->type = type_infer(left->type, right->type);
    ast_push(new_expr, left);
    ast_push(new_expr, right);
    return new_expr;
}

ast_t *reduce_expr_div(ast_t *left, ast_t *right) {
    ast_t *new_expr = ast_new(ast_expr_div);
    new_expr->type = type_infer(left->type, right->type);
    ast_push(new_expr, left);
    ast_push(new_expr, right);
    return new_expr;
}

ast_t *reduce_expr_mod(ast_t *left, ast_t *right) {
    ast_t *new_expr = ast_new(ast_expr_mod);
    new_expr->type = type_infer(left->type, right->type);
    ast_push(new_expr, left);
    ast_push(new_expr, right);
    return new_expr;
}

ast_t *reduce_expr_inv(ast_t *expr) {
    ast_t *new_expr = ast_new(ast_expr_inv);
    new_expr->type = expr->type;
    ast_push(new_expr, expr);
    return new_expr;
}

ast_t *reduce_expr_not(ast_t *expr) {
    ast_t *new_expr = ast_new(ast_expr_not);
    new_expr->type = expr->type;
    ast_push(new_expr, expr);
    return new_expr;
}

ast_t *reduce_expr_ident(lexeme_t *literal) {
    ast_t *expr = ast_new(ast_ident);
    name_entry_t *entry = scope_find(current_scope, literal->lex_ident_t.value);
    if (entry == NULL) {
        fprintf(stderr, "ERRO: a variavel \"%s\" foi utilizada antes de ser declarada\n", literal->lex_ident_t.value);
        fprintf(stderr, "- Contexto: na linha %ld, coluna %ld\n", literal->lex_ident_t.line, literal->lex_ident_t.column);
        fprintf(stderr, "- Contexto: em uma expressao na funcao \"%s\"\n",
                list_get_as(global_scope->entries, global_scope->entries->length-1, name_entry_t)->lexeme->lex_ident_t.value);
        exit(ERR_UNDECLARED);
    }
    if (entry->nature != nat_identifier) {
        fprintf(stderr, "ERRO: a funcao \"%s\" foi utilizada como variavel\n", literal->lex_ident_t.value);
        fprintf(stderr, "- Contexto: na linha %ld, coluna %ld\n", literal->lex_ident_t.line, literal->lex_ident_t.column);
        fprintf(stderr, "- Contexto: em uma expressao na funcao \"%s\"\n",
                list_get_as(global_scope->entries, global_scope->entries->length-1, name_entry_t)->lexeme->lex_ident_t.value);
        exit(ERR_FUNCTION);
    }
    expr->type = entry->type;
    expr->lexeme = literal;
    return expr;
}

ast_t *reduce_expr_int(lexeme_t *literal) {
    ast_t *expr = ast_new(ast_val_int);
    expr->type = type_int;
    expr->lexeme = literal;
    return expr;
}

ast_t *reduce_expr_float(lexeme_t *literal) {
    ast_t *expr = ast_new(ast_val_float);
    expr->type = type_float;
    expr->lexeme = literal;
    return expr;
}

ast_t *reduce_expr_bool(lexeme_t *literal) {
    ast_t *expr = ast_new(ast_val_bool);
    expr->type = type_bool;
    expr->lexeme = literal;
    return expr;
}

ast_t *reduce_expr_call(lexeme_t *literal, list_t *arguments) {
    ast_t *call = ast_new(ast_call);
    name_entry_t *entry = scope_find(current_scope, literal->lex_ident_t.value);
    if (entry == NULL) {
        fprintf(stderr, "ERRO: a funcao \"%s\" foi utilizada antes de ser declarada\n", literal->lex_ident_t.value);
        fprintf(stderr, "- Contexto: na linha %ld, coluna %ld\n", literal->lex_ident_t.line, literal->lex_ident_t.column);
        fprintf(stderr, "- Contexto: em uma expressao na funcao \"%s\"\n",
                list_get_as(global_scope->entries, global_scope->entries->length-1, name_entry_t)->lexeme->lex_ident_t.value);
        exit(ERR_UNDECLARED);
    }
    if (entry->nature != nat_function) {
        fprintf(stderr, "ERRO: a variavel \"%s\" foi utilizada como funcao\n", literal->lex_ident_t.value);
        fprintf(stderr, "- Contexto: na linha %ld, coluna %ld\n", literal->lex_ident_t.line, literal->lex_ident_t.column);
        fprintf(stderr, "- Contexto: em uma expressao na funcao \"%s\"\n",
                list_get_as(global_scope->entries, global_scope->entries->length-1, name_entry_t)->lexeme->lex_ident_t.value);
        exit(ERR_VARIABLE);
    }
    call->type = entry->type;
    call->lexeme = literal;
    list_iterate(arguments, i) {
        ast_t *node = list_get_as(arguments, i, ast_t);
        ast_push(call, node);
    }
    list_free(arguments);
    return call;
}

/********\
* Lexeme *
\********/
lexeme_t *lexeme_new(lexeme_type_t type, uint64_t line, uint64_t column) {
    lexeme_t *lexeme = (lexeme_t*) malloc(sizeof(lexeme_t));
    if (lexeme == NULL) {
        fprintf(stderr, "ERROR: Failed to allocate memory for lexeme_t (errno = %d) [at file \"" __FILE__ "\", line %d]\n", errno, __LINE__-2);
        exit(EXIT_FAILURE);
    }
    lexeme->lex_ident_t.type = type;
    lexeme->lex_ident_t.line = line;
    lexeme->lex_ident_t.column = column;
    return lexeme;
}

lexeme_t *lexeme_clone(lexeme_t *lexeme) {
    lexeme_t *new_lexeme = (lexeme_t*) malloc(sizeof(lexeme_t));
    if (new_lexeme == NULL) {
        fprintf(stderr, "ERROR: Failed to allocate memory for lexeme_t (errno = %d) [at file \"" __FILE__ "\", line %d]\n", errno, __LINE__-2);
        exit(EXIT_FAILURE);
    }
    switch (lexeme->lex_ident_t.type) {
        case lex_ident:
            new_lexeme->lex_ident_t.type = lexeme->lex_ident_t.type;
            new_lexeme->lex_ident_t.line = lexeme->lex_ident_t.line;
            new_lexeme->lex_ident_t.column = lexeme->lex_ident_t.column;
            new_lexeme->lex_ident_t.value = lexeme->lex_ident_t.value;
            break;
        case lex_int:
            new_lexeme->lex_int_t.type = lexeme->lex_int_t.type;
            new_lexeme->lex_int_t.line = lexeme->lex_int_t.line;
            new_lexeme->lex_int_t.column = lexeme->lex_int_t.column;
            new_lexeme->lex_int_t.value = lexeme->lex_int_t.value;
            break;
        case lex_float:
            new_lexeme->lex_float_t.type = lexeme->lex_float_t.type;
            new_lexeme->lex_float_t.line = lexeme->lex_float_t.line;
            new_lexeme->lex_float_t.column = lexeme->lex_float_t.column;
            new_lexeme->lex_float_t.value = lexeme->lex_float_t.value;
            break;
        case lex_bool:
            new_lexeme->lex_bool_t.type = lexeme->lex_bool_t.type;
            new_lexeme->lex_bool_t.line = lexeme->lex_bool_t.line;
            new_lexeme->lex_bool_t.column = lexeme->lex_bool_t.column;
            new_lexeme->lex_bool_t.value = lexeme->lex_bool_t.value;
            break;
    }
    return new_lexeme;
}

/*******************\
* Semantic Analysis *
\*******************/
type_t type_infer(type_t left, type_t right) {
    if (left == right) {
        return left;
    } else if (left == type_float || right == type_float) {
        return type_float;
    } else {
        return type_int;
    }
}

scope_t *scope_new(scope_t *parent) {
    scope_t *scope = (scope_t*) malloc(sizeof(scope_t));
    if (scope == NULL) {
        fprintf(stderr, "ERROR: Failed to allocate memory for scope_t (errno = %d) [at file \"" __FILE__ "\", line %d]\n", errno, __LINE__-2);
        exit(EXIT_FAILURE);
    }
    scope->parent = parent;
    scope->size = 0;
    scope->entries = empty_list();
    return scope;
}

void name_entry_free(name_entry_t *entry) {
    free(entry->lexeme);
    free(entry);
}

void scope_free(scope_t *scope) {
    list_iterate(scope->entries, i) {
        name_entry_t *entry = list_get_as(scope->entries, i, name_entry_t);
        name_entry_free(entry);
    }
    list_free(scope->entries);
    free(scope);
}

int register_variable(scope_t *scope, type_t type, lexeme_t *lexeme) {
    name_entry_t *found_entry = scope_find(scope, lexeme->lex_ident_t.value);
    if (found_entry != NULL) {
        fprintf(stderr, "ERRO: a variavel \"%s\" for redefinida\n", lexeme->lex_ident_t.value);
        if (found_entry->nature == nat_identifier) {
            return 1;
        } else if (found_entry->nature == nat_function) {
            return 2;
        }
        return -1;
    }
    
    name_entry_t *name_entry = (name_entry_t*) malloc(sizeof(name_entry_t));
    if (name_entry == NULL) {
        fprintf(stderr, "ERROR: Failed to allocate memory for name_entry_t (errno = %d) [at file \"" __FILE__ "\", line %d]\n", errno, __LINE__-2);
        exit(EXIT_FAILURE);
    }
    name_entry->nature = nat_identifier;
    name_entry->type = type;
    name_entry->lexeme = lexeme_clone(lexeme);
    name_entry->line = lexeme->lex_ident_t.line;
    name_entry->column = lexeme->lex_ident_t.column;
    name_entry->offset = scope->size;
    if (scope->parent == NULL) {
        name_entry->base_register = rbss;
    } else {
        name_entry->base_register = rfp;
    }
    scope->size += sizeof_type(type);
    list_push(scope->entries, name_entry);
    return 0;
}

int register_function(scope_t *scope, type_t type, lexeme_t *lexeme) {
    name_entry_t *found_entry = scope_find(scope, lexeme->lex_ident_t.value);
    if (found_entry != NULL) {
        fprintf(stderr, "ERRO: a funcao \"%s\" for redefinida\n", lexeme->lex_ident_t.value);
        if (found_entry->nature == nat_identifier) {
            return 1;
        } else if (found_entry->nature == nat_function) {
            return 2;
        }
        return -1;
    }
    name_entry_t *name_entry = (name_entry_t*) malloc(sizeof(name_entry_t));
    if (name_entry == NULL) {
        fprintf(stderr, "ERROR: Failed to allocate memory for name_entry_t (errno = %d) [at file \"" __FILE__ "\", line %d]\n", errno, __LINE__-2);
        exit(EXIT_FAILURE);
    }
    name_entry->nature = nat_function;
    name_entry->type = type;
    name_entry->lexeme = lexeme_clone(lexeme);
    name_entry->line = lexeme->lex_ident_t.line;
    name_entry->column = lexeme->lex_ident_t.column;
    name_entry->offset = scope->size;
    scope->size += sizeof_type(type);
    list_push(scope->entries, name_entry);
    return 0;
}

uint64_t sizeof_type(type_t type) {
    switch (type) {
        case type_undefined:
            return 0;
        case type_int:
            return 4;
        case type_float:
            return 8;
        case type_bool:
            return 4;
    }
}

name_entry_t *scope_find(scope_t *scope, char *name) {
    while (scope != NULL) {
        list_iterate(scope->entries, i) {
            name_entry_t *entry = list_get_as(scope->entries, i, name_entry_t);
            if (entry->nature == nat_literal) {
                continue;
            }
            if (strcmp(entry->lexeme->lex_ident_t.value, name) == 0) {
                return entry;
            }
        }
        scope = scope->parent;
    }
    return NULL;
}

