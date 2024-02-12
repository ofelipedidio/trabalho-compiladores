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
uint64_t last_id = 1;
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
        case push:
            fprintf(stdout, "push r%ld\n", instruction->r1); // r3 = r1 + r2
            break;
        case pop:
            fprintf(stdout, "pop r%ld\n", instruction->r1); // r3 = r1 + r2
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
        case rsub_i:
            fprintf(stdout, "rsubI r%ld, %ld => r%ld\n", instruction->r1, instruction->r2, instruction->r3); // r3 = c2 - r1
            break;
        case load_ai_r:
            switch (id_to_reg(instruction->r1)) {
                case rfp:
                    fprintf(stdout, "loadAI rfp, %ld => r%ld\n", instruction->r2, instruction->r3); // r3 = Memoria(r1 + c2)
                    break;
                case rsp:
                    fprintf(stdout, "loadAI sp, %ld => r%ld\n",  instruction->r2, instruction->r3); // r3 = Memoria(r1 + c2)
                    break;
                case rbss:
                    fprintf(stdout, "loadAI rbss, %ld => r%ld\n", instruction->r2, instruction->r3); // r3 = Memoria(r1 + c2)
                    break;
                case rpc:
                    fprintf(stdout, "loadAI rpc, %ld => r%ld\n", instruction->r2, instruction->r3); // r3 = Memoria(r1 + c2)
                    break;
            }
            break;
        case load_i:
            fprintf(stdout, "loadI %ld => r%ld\n", instruction->r1, instruction->r2); // r2 = c1
            break;
        case store_ai_r:
            switch (id_to_reg(instruction->r2)) {
                case rfp:
                    fprintf(stdout, "storeAI r%ld => rfp, %ld\n", instruction->r1, instruction->r3); // Memoria(r2 + c3) = r1
                    break;
                case rsp:
                    fprintf(stdout, "storeAI r%ld => rsp, %ld\n", instruction->r1, instruction->r3); // Memoria(r2 + c3) = r1
                    break;
                case rbss:
                    fprintf(stdout, "storeAI r%ld => rbss, %ld\n", instruction->r1, instruction->r3); // Memoria(r2 + c3) = r1
                    break;
                case rpc:
                    fprintf(stdout, "storeAI r%ld => rpc, %ld\n", instruction->r1, instruction->r3); // Memoria(r2 + c3) = r1
                    break;
            }
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
    nlist_init(char*, ast->_program);
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
    parent->children[parent->length] = child; parent->length++;
}

/********************\
 * Reduction Handlers *
 \********************/
char *current_function;
void reduce_push_scope() {
    if (current_scope == NULL) {
        current_scope = scope_new(current_scope, strdup("global_scope"));
    } else if (current_scope->parent == NULL) {
        current_scope = scope_new(current_scope, strdup(current_function));
    } else {
        current_scope = scope_new(current_scope, strdup("inner_scope"));
    }
    if (current_scope->parent == NULL) {
        global_scope = current_scope;
    }
}

void reduce_pop_scope() {
    if (current_scope == global_scope) {
        return;
    }
    print_scope(stderr, current_scope);
    scope_t *temp_scope = current_scope;
    current_scope = current_scope->parent;
    scope_free(temp_scope);
    if (current_scope == NULL) {
        global_scope = NULL;
    }
}

char str_buffer[16*1024];
#define asm_push(list, ...) sprintf(str_buffer, __VA_ARGS__); nlist_insert(char*, list, strdup(str_buffer))
#define asm_append(list, other) for (size_t i = 0; i < other.length; i++) { nlist_insert(char*, list, strdup(other.items[i])); }

ast_t *reduce_program(ast_t *global_list) {
    name_entry_t *entry = scope_find(current_scope, "main");
    if (global_list->value == 0) {
        fprintf(stderr, "ERRO: O programa compilado nao contem a funcao \"main\"\n");
        exit(ERR_ENTRY);
    }

    ast_t *program = ast_new(ast_program);
    asm_push(program->_program, ".text\n");

    list_iterate(global_scope->entries, i) {
        if (list_get_as(global_scope->entries, i, name_entry_t)->nature != nat_identifier) {
            continue;
        }
        char *txt = list_get_as(global_scope->entries, i, name_entry_t)->lexeme->lex_ident_t.value;
        // fprintf(stderr, "%s\n", txt);
        asm_push(program->_program, ".globl %s\n", txt);
        asm_push(program->_program, ".bss\n");
        asm_push(program->_program, ".align 4\n");
        asm_push(program->_program, ".type %s, @object\n", txt);
        asm_push(program->_program, ".size %s, 4\n", txt);
        asm_push(program->_program, "%s:\n", txt);
        asm_push(program->_program, ".zero 4\n");
    }

    asm_push(program->_program, ".text\n");
    asm_push(program->_program, ".globl main\n");
    asm_push(program->_program, ".type main, @function\n");

    asm_append(program->_program, global_list->_program);

    asm_push(program->_program, ".section .note.GNU-stack,\"\",@progbits\n");


    return program;
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
    // ILOC
    name_entry_t *entry = scope_find(current_scope, function_header->lexeme->lex_ident_t.value);
    asm_push(node->_program, "%s:\n", function_header->lexeme->lex_ident_t.value);
    asm_push(node->_program, "endbr64\n");
    asm_push(node->_program, "pushq %%rbp\n");
    asm_push(node->_program, "movq %%rsp, %%rbp\n");

    asm_append(node->_program, commands->_program);
    asm_push(node->_program, "movl $1, %%eax\n");
    asm_push(node->_program, "popq %%rbp\n");
    asm_push(node->_program, "ret\n");

    asm_append(global_list->_program, node->_program);

    if (strcmp(function_header->lexeme->lex_ident_t.value, "main") == 0) {
        global_list->value = entry->function_label;
    }
    // Return
    return global_list;
}

ast_t *reduce_function_header(list_t *parameters, type_t type, lexeme_t *name) {
    current_function = name->lex_ident_t.value;
    reduce_push_scope();
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
    // ILOC
    asm_append(assignment->_program, expr->_program);
    asm_push(assignment->_program, "// assignment (%s)\n", entry->lexeme->lex_ident_t.value);
    switch (entry->base_register) {
        case rbss:
            asm_push(assignment->_program, "movl %%eax, %s(%%rip)\n", entry->lexeme->lex_ident_t.value);
            break;
        case rfp:
            asm_push(expr->_program, "movl -%lu(%%rbp), %%eax\n", entry->offset);
            break;
        default:
            fprintf(stderr, "Register error #1\n");
            exit(EXIT_FAILURE);
            break;
    }
    asm_append(commands->_program, assignment->_program);
    // Return
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
    // TODO - Didio: write iloc code to handle calls
    return commands;
}

ast_t *reduce_command_return(ast_t *commands, ast_t *expr) {
    ast_t *return_ = ast_new(ast_return);
    return_->type = expr->type;
    ast_push(return_, expr);
    ast_push(commands, return_);
    // ILOC
    asm_push(return_->_program, "// Return\n");
    asm_append(return_->_program, expr->_program);
    asm_push(return_->_program, "popq %%rbp\n");
    asm_push(return_->_program, "ret\n");
    asm_append(commands->_program, return_->_program);
    return commands;
}

ast_t *reduce_command_if_else(ast_t *commands, ast_t *cond, ast_t *then_block, ast_t *else_block) {
    ast_t *if_ = ast_new(ast_if);
    if_->type = cond->type;
    ast_push(if_, cond);
    ast_push(if_, then_block);
    ast_push(if_, else_block);
    ast_push(commands, if_);
    // Compute condition
    uint64_t label_else = iloc_next_id();
    uint64_t label_done = iloc_next_id();

    asm_push(if_->_program, "// if (expr)\n");
    asm_append(if_->_program, cond->_program);
    asm_push(if_->_program, "// if (cmp)\n");
    asm_push(if_->_program, "and %%eax, %%eax\n");
    asm_push(if_->_program, "jz L%lu\n", label_else);
    asm_push(if_->_program, "// if (then)\n");
    asm_append(if_->_program, then_block->_program);
    asm_push(if_->_program, "jmp L%lu\n", label_done);
    asm_push(if_->_program, "// if (else)\n");
    asm_push(if_->_program, "L%lu:\n", label_else);
    asm_append(if_->_program, else_block->_program);
    asm_push(if_->_program, "// if (done)\n");
    asm_push(if_->_program, "L%lu:\n", label_done);

    asm_append(commands->_program, if_->_program);
    // Return
    return commands;
}

ast_t *reduce_command_if(ast_t *commands, ast_t *cond, ast_t *then_block) {
    ast_t *if_ = ast_new(ast_if);
    if_->type = cond->type;
    ast_push(if_, cond);
    ast_push(if_, then_block);
    ast_push(if_, NULL);
    ast_push(commands, if_);
    // ILOC
    uint64_t label_done = iloc_next_id();

    asm_push(if_->_program, "// if (expr)\n");
    asm_append(if_->_program, cond->_program);
    asm_push(if_->_program, "// if (cmp)\n");
    asm_push(if_->_program, "and %%eax, %%eax\n");
    asm_push(if_->_program, "jz L%lu\n", label_done);
    asm_push(if_->_program, "// if (then)\n");
    asm_append(if_->_program, then_block->_program);
    asm_push(if_->_program, "// if (done)\n");
    asm_push(if_->_program, "L%lu:\n", label_done);

    asm_append(commands->_program, if_->_program);
    // Return
    return commands;
}

ast_t *reduce_command_while(ast_t *commands, ast_t *cond, ast_t *block) {
    ast_t *while_ = ast_new(ast_while);
    while_->type = cond->type;
    ast_push(while_, cond);
    ast_push(while_, block);
    ast_push(commands, while_);
    // ILOC
    uint64_t label_start = iloc_next_id();
    uint64_t label_done = iloc_next_id();

    asm_push(while_->_program, "// while (expr)\n");
    asm_push(while_->_program, "L%lu:\n", label_start);
    asm_append(while_->_program, cond->_program);
    asm_push(while_->_program, "// while (cmp)\n");
    asm_push(while_->_program, "and %%eax, %%eax\n");
    asm_push(while_->_program, "jz L%lu\n", label_done);
    asm_push(while_->_program, "// while (block)\n");
    asm_append(while_->_program, block->_program);
    asm_push(while_->_program, "jmp L%lu\n", label_start);
    asm_push(while_->_program, "// while (done)\n");
    asm_push(while_->_program, "L%lu:\n", label_done);

    asm_append(commands->_program, while_->_program);
    // Return
    return commands;
}

ast_t *reduce_command_block(ast_t *commands, ast_t *block) {
    ast_push(commands, block);
    asm_push(commands->_program, "// block\n");
    asm_append(commands->_program, block->_program);
    return commands;
}

ast_t *reduce_expr_or(ast_t *left, ast_t *right) {
    ast_t *new_expr = ast_new(ast_expr_or);
    new_expr->type = type_infer(left->type, right->type);
    ast_push(new_expr, left);
    ast_push(new_expr, right);
    // ILOC
    uint64_t label_right = iloc_next_id();
    uint64_t label_false = iloc_next_id();
    uint64_t label_done = iloc_next_id();

    // Left
    asm_push(new_expr->_program, "// or (left)\n");
    asm_append(new_expr->_program, left->_program);
    asm_push(new_expr->_program, "// or (cmp)\n");
    asm_push(new_expr->_program, "and %%eax, %%eax\n");
    asm_push(new_expr->_program, "jz L%lu\n", label_right);
    // True
    asm_push(new_expr->_program, "movl $1, %%eax\n");
    asm_push(new_expr->_program, "jmp L%lu\n", label_done);
    // Right
    asm_push(new_expr->_program, "L%lu:\n", label_right);
    asm_push(new_expr->_program, "// or (right)\n");
    asm_append(new_expr->_program, right->_program);
    asm_push(new_expr->_program, "// or (cmp)\n");
    asm_push(new_expr->_program, "and %%eax, %%eax\n");
    asm_push(new_expr->_program, "jz L%lu\n", label_false);
    // True
    asm_push(new_expr->_program, "movl $1, %%eax\n");
    asm_push(new_expr->_program, "jmp L%lu\n", label_done);
    // False
    asm_push(new_expr->_program, "L%lu:\n", label_false);
    asm_push(new_expr->_program, "movl $0, %%eax\n");
    // Done
    asm_push(new_expr->_program, "// or (done)\n");
    asm_push(new_expr->_program, "L%lu:\n", label_done);

    // Return
    return new_expr;
}

ast_t *reduce_expr_and(ast_t *left, ast_t *right) {
    ast_t *new_expr = ast_new(ast_expr_and);
    new_expr->type = type_infer(left->type, right->type);
    ast_push(new_expr, left);
    ast_push(new_expr, right);
    // ILOC
    uint64_t label_false = iloc_next_id();
    uint64_t label_done = iloc_next_id();

    // Left
    asm_push(new_expr->_program, "// and (left)\n");
    asm_append(new_expr->_program, left->_program);
    asm_push(new_expr->_program, "// and (cmp)\n");
    asm_push(new_expr->_program, "and %%eax, %%eax\n");
    asm_push(new_expr->_program, "jz L%lu\n", label_false);
    // Right
    asm_push(new_expr->_program, "// and (right)\n");
    asm_append(new_expr->_program, right->_program);
    asm_push(new_expr->_program, "// and (cmp)\n");
    asm_push(new_expr->_program, "and %%eax, %%eax\n");
    asm_push(new_expr->_program, "jz L%lu\n", label_false);
    // True
    asm_push(new_expr->_program, "movl $1, %%eax\n");
    asm_push(new_expr->_program, "jmp L%lu\n", label_done);
    // False
    asm_push(new_expr->_program, "L%lu:\n", label_false);
    asm_push(new_expr->_program, "movl $0, %%eax\n");
    // Done
    asm_push(new_expr->_program, "// and (done)\n");
    asm_push(new_expr->_program, "L%lu:\n", label_done);

    // Return
    return new_expr;
}

ast_t *reduce_expr_eq(ast_t *left, ast_t *right) {
    ast_t *new_expr = ast_new(ast_expr_eq);
    new_expr->type = type_infer(left->type, right->type);
    ast_push(new_expr, left);
    ast_push(new_expr, right);
    // ILOC
    uint64_t label_false = iloc_next_id();
    uint64_t label_done = iloc_next_id();

    // Left
    asm_push(new_expr->_program, "// eq (left)\n");
    asm_append(new_expr->_program, left->_program);
    asm_push(new_expr->_program, "pushq %%rax\n");
    // Right
    asm_push(new_expr->_program, "// eq (right)\n");
    asm_append(new_expr->_program, right->_program);
    // Compare
    asm_push(new_expr->_program, "popq %%rbx\n");
    asm_push(new_expr->_program, "// eq (cmp)\n");
    asm_push(new_expr->_program, "cmp %%ebx, %%eax\n");
    asm_push(new_expr->_program, "jne L%lu\n", label_false);
    // True
    asm_push(new_expr->_program, "movl $1, %%eax\n");
    asm_push(new_expr->_program, "jmp L%lu\n", label_done);
    // False
    asm_push(new_expr->_program, "L%lu:\n", label_false);
    asm_push(new_expr->_program, "movl $0, %%eax\n");
    // Done
    asm_push(new_expr->_program, "// eq (done)\n");
    asm_push(new_expr->_program, "L%lu:\n", label_done);

    // Return
    return new_expr;
}

ast_t *reduce_expr_ne(ast_t *left, ast_t *right) {
    ast_t *new_expr = ast_new(ast_expr_ne);
    new_expr->type = type_infer(left->type, right->type);
    ast_push(new_expr, left);
    ast_push(new_expr, right);
    // ILOC
    uint64_t label_true = iloc_next_id();
    uint64_t label_done = iloc_next_id();

    // Left
    asm_push(new_expr->_program, "// ne (left)\n");
    asm_append(new_expr->_program, left->_program);
    asm_push(new_expr->_program, "pushq %%rax\n");
    // Right
    asm_push(new_expr->_program, "// ne (right)\n");
    asm_append(new_expr->_program, right->_program);
    // Compare
    asm_push(new_expr->_program, "popq %%rbx\n");
    asm_push(new_expr->_program, "// ne (cmp)\n");
    asm_push(new_expr->_program, "cmp %%ebx, %%eax\n");
    asm_push(new_expr->_program, "jne L%lu\n", label_true);
    // False
    asm_push(new_expr->_program, "movl $0, %%eax\n");
    asm_push(new_expr->_program, "jmp L%lu\n", label_done);
    // True
    asm_push(new_expr->_program, "L%lu:\n", label_true);
    asm_push(new_expr->_program, "movl $1, %%eax\n");
    // Done
    asm_push(new_expr->_program, "// ne (done)\n");
    asm_push(new_expr->_program, "L%lu:\n", label_done);

    // Return
    return new_expr;
}

ast_t *reduce_expr_lt(ast_t *left, ast_t *right) {
    ast_t *new_expr = ast_new(ast_expr_lt);
    new_expr->type = type_infer(left->type, right->type);
    ast_push(new_expr, left);
    ast_push(new_expr, right);
    // ILOC
    uint64_t label_true = iloc_next_id();
    uint64_t label_done = iloc_next_id();

    // Left
    asm_push(new_expr->_program, "// lt (left)\n");
    asm_append(new_expr->_program, left->_program);
    asm_push(new_expr->_program, "pushq %%rax\n");
    // Right
    asm_push(new_expr->_program, "// lt (right)\n");
    asm_append(new_expr->_program, right->_program);
    // Compare
    asm_push(new_expr->_program, "popq %%rbx\n");
    asm_push(new_expr->_program, "// lt (cmp)\n");
    asm_push(new_expr->_program, "cmp %%eax, %%ebx\n");
    asm_push(new_expr->_program, "jl L%lu\n", label_true);
    // False
    asm_push(new_expr->_program, "movl $0, %%eax\n");
    asm_push(new_expr->_program, "jmp L%lu\n", label_done);
    // True
    asm_push(new_expr->_program, "L%lu:\n", label_true);
    asm_push(new_expr->_program, "movl $1, %%eax\n");
    // Done
    asm_push(new_expr->_program, "// lt (done)\n");
    asm_push(new_expr->_program, "L%lu:\n", label_done);

    // Return
    return new_expr;
}

ast_t *reduce_expr_gt(ast_t *left, ast_t *right) {
    ast_t *new_expr = ast_new(ast_expr_gt);
    new_expr->type = type_infer(left->type, right->type);
    ast_push(new_expr, left);
    ast_push(new_expr, right);
    // ILOC
    uint64_t label_true = iloc_next_id();
    uint64_t label_done = iloc_next_id();

    // Left
    asm_push(new_expr->_program, "// gt (left)\n");
    asm_append(new_expr->_program, left->_program);
    asm_push(new_expr->_program, "pushq %%rax\n");
    // Right
    asm_push(new_expr->_program, "// gt (right)\n");
    asm_append(new_expr->_program, right->_program);
    // Compare
    asm_push(new_expr->_program, "popq %%rbx\n");
    asm_push(new_expr->_program, "// gt (cmp)\n");
    asm_push(new_expr->_program, "cmp %%eax, %%ebx\n");
    asm_push(new_expr->_program, "ja L%lu\n", label_true);
    // False
    asm_push(new_expr->_program, "movl $0, %%eax\n");
    asm_push(new_expr->_program, "jmp L%lu\n", label_done);
    // True
    asm_push(new_expr->_program, "L%lu:\n", label_true);
    asm_push(new_expr->_program, "movl $1, %%eax\n");
    // Done
    asm_push(new_expr->_program, "// gt (done)\n");
    asm_push(new_expr->_program, "L%lu:\n", label_done);

    // Return
    return new_expr;
}

ast_t *reduce_expr_le(ast_t *left, ast_t *right) {
    ast_t *new_expr = ast_new(ast_expr_le);
    new_expr->type = type_infer(left->type, right->type);
    ast_push(new_expr, left);
    ast_push(new_expr, right);
    // ILOC
    uint64_t label_true = iloc_next_id();
    uint64_t label_done = iloc_next_id();

    // Left
    asm_push(new_expr->_program, "// le (left)\n");
    asm_append(new_expr->_program, left->_program);
    asm_push(new_expr->_program, "pushq %%rax\n");
    // Right
    asm_push(new_expr->_program, "// le (right)\n");
    asm_append(new_expr->_program, right->_program);
    // Compare
    asm_push(new_expr->_program, "popq %%rbx\n");
    asm_push(new_expr->_program, "// le (cmp)\n");
    asm_push(new_expr->_program, "cmp %%eax, %%ebx\n");
    asm_push(new_expr->_program, "jle L%lu\n", label_true);
    // False
    asm_push(new_expr->_program, "movl $0, %%eax\n");
    asm_push(new_expr->_program, "jmp L%lu\n", label_done);
    // True
    asm_push(new_expr->_program, "L%lu:\n", label_true);
    asm_push(new_expr->_program, "movl $1, %%eax\n");
    // Done
    asm_push(new_expr->_program, "// le (done)\n");
    asm_push(new_expr->_program, "L%lu:\n", label_done);

    // Return
    return new_expr;
}

ast_t *reduce_expr_ge(ast_t *left, ast_t *right) {
    ast_t *new_expr = ast_new(ast_expr_ge);
    new_expr->type = type_infer(left->type, right->type);
    ast_push(new_expr, left);
    ast_push(new_expr, right);
    // ILOC
    uint64_t label_true = iloc_next_id();
    uint64_t label_done = iloc_next_id();

    // Left
    asm_push(new_expr->_program, "// ge (left)\n");
    asm_append(new_expr->_program, left->_program);
    asm_push(new_expr->_program, "pushq %%rax\n");
    // Right
    asm_push(new_expr->_program, "// ge (right)\n");
    asm_append(new_expr->_program, right->_program);
    // Compare
    asm_push(new_expr->_program, "popq %%rbx\n");
    asm_push(new_expr->_program, "// ge (cmp)\n");
    asm_push(new_expr->_program, "cmp %%eax, %%ebx\n");
    asm_push(new_expr->_program, "jge L%lu\n", label_true);
    // False
    asm_push(new_expr->_program, "movl $0, %%eax\n");
    asm_push(new_expr->_program, "jmp L%lu\n", label_done);
    // True
    asm_push(new_expr->_program, "L%lu:\n", label_true);
    asm_push(new_expr->_program, "movl $1, %%eax\n");
    // Done
    asm_push(new_expr->_program, "// ge (done)\n");
    asm_push(new_expr->_program, "L%lu:\n", label_done);

    // Return
    return new_expr;
}

ast_t *reduce_expr_add(ast_t *left, ast_t *right) {
    ast_t *new_expr = ast_new(ast_expr_add);
    new_expr->type = type_infer(left->type, right->type);
    uint64_t temp_val_1 = iloc_next_id();
    ast_push(new_expr, left);
    ast_push(new_expr, right);
    // ILOC
    // Left
    asm_push(new_expr->_program, "// add (left)\n");
    asm_append(new_expr->_program, left->_program);
    asm_push(new_expr->_program, "pushq %%rax\n");
    // Right
    asm_push(new_expr->_program, "// add (right)\n");
    asm_append(new_expr->_program, right->_program);
    // Opperation
    asm_push(new_expr->_program, "popq %%rbx\n");
    asm_push(new_expr->_program, "// add (op)\n");
    asm_push(new_expr->_program, "addl %%ebx, %%eax\n");

    // Return
    return new_expr;
}

ast_t *reduce_expr_sub(ast_t *left, ast_t *right) {
    ast_t *new_expr = ast_new(ast_expr_sub);
    new_expr->type = type_infer(left->type, right->type);
    uint64_t temp_val_1 = iloc_next_id();
    ast_push(new_expr, left);
    ast_push(new_expr, right);
    // ILOC
    // ILOC
    // Left
    asm_push(new_expr->_program, "// sub (left)\n");
    asm_append(new_expr->_program, left->_program);
    asm_push(new_expr->_program, "pushq %%rax\n");
    // Right
    asm_push(new_expr->_program, "// sub (right)\n");
    asm_append(new_expr->_program, right->_program);
    // Opperation
    asm_push(new_expr->_program, "movl %%eax, %%ebx\n");
    asm_push(new_expr->_program, "popq %%rax\n");
    asm_push(new_expr->_program, "// sub (op)\n");
    asm_push(new_expr->_program, "subl %%ebx, %%eax\n");

    // Return
    return new_expr;
}

ast_t *reduce_expr_mul(ast_t *left, ast_t *right) {
    ast_t *new_expr = ast_new(ast_expr_mul);
    new_expr->type = type_infer(left->type, right->type);
    ast_push(new_expr, left);
    ast_push(new_expr, right);
    // ILOC
    // Left
    asm_push(new_expr->_program, "// mul (left)\n");
    asm_append(new_expr->_program, left->_program);
    asm_push(new_expr->_program, "pushq %%rax\n");
    // Right
    asm_push(new_expr->_program, "// mul (right)\n");
    asm_append(new_expr->_program, right->_program);
    // Opperation
    asm_push(new_expr->_program, "popq %%rbx\n");
    asm_push(new_expr->_program, "// mul (op)\n");
    asm_push(new_expr->_program, "imull %%ebx, %%eax\n");

    // Return
    return new_expr;
}

ast_t *reduce_expr_div(ast_t *left, ast_t *right) {
    ast_t *new_expr = ast_new(ast_expr_div);
    new_expr->type = type_infer(left->type, right->type);
    ast_push(new_expr, left);
    ast_push(new_expr, right);
    // ILOC
    asm_push(new_expr->_program, "// div (left)\n");
    asm_append(new_expr->_program, left->_program);
    asm_push(new_expr->_program, "pushq %%rax\n");
    asm_push(new_expr->_program, "// div (right)\n");
    asm_append(new_expr->_program, right->_program);
    asm_push(new_expr->_program, "movl %%eax, %%ecx\n");
    asm_push(new_expr->_program, "popq %%rax\n");
    asm_push(new_expr->_program, "// div (op)\n");
    asm_push(new_expr->_program, "cltd\n");
    asm_push(new_expr->_program, "idivl %%ecx\n");

    // Return
    return new_expr;
}

ast_t *reduce_expr_mod(ast_t *left, ast_t *right) {
    ast_t *new_expr = ast_new(ast_expr_mod);
    new_expr->type = type_infer(left->type, right->type);
    ast_push(new_expr, left);
    ast_push(new_expr, right);
    // ILOC
    asm_push(new_expr->_program, "// mod (left)\n");
    asm_append(new_expr->_program, left->_program);
    asm_push(new_expr->_program, "pushq %%rax\n");
    asm_push(new_expr->_program, "// mod (right)\n");
    asm_append(new_expr->_program, right->_program);
    asm_push(new_expr->_program, "movl %%eax, %%ecx\n");
    asm_push(new_expr->_program, "popq %%rax\n");
    asm_push(new_expr->_program, "// mod (op)\n");
    asm_push(new_expr->_program, "cltd\n");
    asm_push(new_expr->_program, "idivl %%ecx\n");
    asm_push(new_expr->_program, "movl %%edx, %%eax\n");

    // Return
    return new_expr;
}

ast_t *reduce_expr_inv(ast_t *expr) {
    ast_t *new_expr = ast_new(ast_expr_inv);
    new_expr->type = expr->type;
    ast_push(new_expr, expr);
    // ILOC
    asm_push(new_expr->_program, "// inv (expr)\n");
    asm_append(new_expr->_program, expr->_program);
    asm_push(new_expr->_program, "// inv (op)\n");
    asm_push(new_expr->_program, "movl $0, %%ebx\n");
    asm_push(new_expr->_program, "sub %%ebx, %%eax\n");

    // Return
    return new_expr;
}

ast_t *reduce_expr_not(ast_t *expr) {
    ast_t *new_expr = ast_new(ast_expr_not);
    new_expr->type = expr->type;
    ast_push(new_expr, expr);
    // ILOC
    uint64_t label_false = iloc_next_id();
    uint64_t label_done = iloc_next_id();

    asm_push(new_expr->_program, "// not (expr)\n");
    asm_append(new_expr->_program, expr->_program);
    asm_push(new_expr->_program, "// not (op)\n");
    asm_push(new_expr->_program, "and %%eax, %%eax\n");
    asm_push(new_expr->_program, "jz L%lu\n", label_false);
    asm_push(new_expr->_program, "movl $1, %%eax\n");
    asm_push(new_expr->_program, "jmp L%lu\n", label_done);
    asm_push(new_expr->_program, "L%lu:\n", label_false);
    asm_push(new_expr->_program, "movl $0, %%eax\n");
    asm_push(new_expr->_program, "L%lu:\n", label_done);
    asm_push(new_expr->_program, "// not (done)\n");

    // Return
    return new_expr;
}

ast_t *reduce_expr_ident(lexeme_t *literal) {
    // Semantics
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
    // AST
    ast_t *expr = ast_new(ast_ident);
    expr->type = entry->type;
    expr->lexeme = literal;
    // ILOC
    asm_push(expr->_program, "// ident\n");
    switch (entry->base_register) {
        case rbss:
            asm_push(expr->_program, "movl %s(%%rip), %%eax\n", entry->lexeme->lex_ident_t.value);
            break;
        case rfp:
            asm_push(expr->_program, "movl -%lu(%%rbp), %%eax\n", entry->offset);
            break;
        default:
            fprintf(stderr, "Register error #1\n");
            exit(EXIT_FAILURE);
            break;
    }

    // Return
    return expr;
}

ast_t *reduce_expr_int(lexeme_t *literal) {
    // Ast
    ast_t *expr = ast_new(ast_val_int);
    expr->type = type_int;
    expr->lexeme = literal;
    // ILOC
    expr->value = iloc_next_id();
    asm_push(expr->_program, "// lit (int)\n");
    asm_push(expr->_program, "movl $%lu, %%eax\n", literal->lex_int_t.value);
    // Return
    return expr;
}

ast_t *reduce_expr_float(lexeme_t *literal) {
    // AST
    ast_t *expr = ast_new(ast_val_float);
    expr->type = type_float;
    expr->lexeme = literal;
    // ILOC
    // Return
    return expr;
}

ast_t *reduce_expr_bool(lexeme_t *literal) {
    // AST
    ast_t *expr = ast_new(ast_val_bool);
    expr->type = type_bool;
    expr->lexeme = literal;
    // ILOC
    expr->value = iloc_next_id();
    uint64_t bool_val = literal->lex_bool_t.value == 0 ? 0 : 1;
    expr->value = iloc_next_id();
    asm_push(expr->_program, "// lit (bool)\n");
    asm_push(expr->_program, "movl $%lu, %%eax\n", bool_val);
    // Return
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
    // TODO - Didio: write iloc code to handle calls
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

scope_t *scope_new(scope_t *parent, char *scope_name) {
    scope_t *scope = (scope_t*) malloc(sizeof(scope_t));
    if (scope == NULL) {
        fprintf(stderr, "ERROR: Failed to allocate memory for scope_t (errno = %d) [at file \"" __FILE__ "\", line %d]\n", errno, __LINE__-2);
        exit(EXIT_FAILURE);
    }
    scope->parent = parent;
    scope->entries = empty_list();
    scope->scope_name = scope_name;
    if (scope->parent == NULL || scope->parent->parent == NULL) {
        scope->size = 0;
        scope->total_size = 0;
    } else {
        scope->size = scope->parent->size;
        scope->total_size = scope->size;
    }
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
    free(scope->scope_name);
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
    name_entry->function_label = iloc_next_id();
    scope->size += sizeof_type(type);
    // Se for o escopo global, apenas adiciona o tamanho no total_size
    // Senao, adiciona o tamanho ao total_size de todos os escopos ate 
    // o escopo da funcao
    if (scope->parent == NULL) {
        scope->total_size += sizeof_type(type);
    } else {
        scope_t *s = scope;
        while (s->parent != NULL) {
            s->total_size += sizeof_type(type);
            s = s->parent;
        }
    }
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

/*******************\
 *      ETAPA 6      *
 \*******************/
typedef struct {
    uint64_t register_id;
    uint64_t start;
    uint64_t end;
} reg_alloc_entry_t;

typedef nlist_definition(reg_alloc_entry_t) rae_list_t;

typedef struct graph {
    uint64_t register_id;
    nlist_definition(struct graph*) next;
} graph_node_t;

typedef nlist_definition(graph_node_t) graph_t;

void handle_instruction(rae_list_t *list, iloc_instruction_t *instruction, uint64_t index) {
    // Handle r1
    switch (instruction->instruction) {
        case add:
        case sub:
        case mult:
        case _div:
        case rsub_i:
        case store_ai_r:
        case cmp_lt:
        case cmp_le:
        case cmp_eq:
        case cmp_ge:
        case cmp_gt:
        case cmp_ne:
        case cbr:
        case push:
        case pop:
            // Has r1
            {
                reg_alloc_entry_t *found;
                nlist_find(reg_alloc_entry_t, *list, item, item.register_id == instruction->r1, found);
                if (found == NULL) {
                    reg_alloc_entry_t new_item;
                    new_item.register_id = instruction->r1;
                    new_item.start = index;
                    new_item.end = index;
                    nlist_insert(reg_alloc_entry_t, *list, new_item);
                } else {
                    found->end = index;
                }
            }
            break;

        case load_i:
        case jump_i:
        case label:
        case load_ai_r:
            // Does not have r1
            break;

        default:
            // Error
            fprintf(stderr, "Instruction %d error #1\n", instruction->instruction);
            exit(EXIT_FAILURE);
            break;
    }

    // Handle r2
    switch (instruction->instruction) {
        case add:
        case sub:
        case mult:
        case _div:
        case load_i:
        case cmp_lt:
        case cmp_le:
        case cmp_eq:
        case cmp_ge:
        case cmp_gt:
        case cmp_ne:
            // Has r2
            {
                reg_alloc_entry_t *found;
                nlist_find(reg_alloc_entry_t, *list, item, item.register_id == instruction->r2, found);
                if (found == NULL) {
                    reg_alloc_entry_t new_item;
                    new_item.register_id = instruction->r2;
                    new_item.start = index;
                    new_item.end = index;
                    nlist_insert(reg_alloc_entry_t, *list, new_item);
                } else {
                    found->end = index;
                }
            }
            break;

        case cbr:
        case jump_i:
        case label:
        case rsub_i:
        case store_ai_r:
        case load_ai_r:
        case push:
        case pop:
            // Does not have r2
            break;

        default:
            // Error
            fprintf(stderr, "Instruction %d error #2\n", instruction->instruction);
            exit(EXIT_FAILURE);
            break;
    }

    // Handle r3
    switch (instruction->instruction) {
        case add:
        case sub:
        case mult:
        case _div:
        case rsub_i:
        case load_ai_r:
        case cmp_lt:
        case cmp_le:
        case cmp_eq:
        case cmp_ge:
        case cmp_gt:
        case cmp_ne:
            // Has r3
            {
                reg_alloc_entry_t *found;
                nlist_find(reg_alloc_entry_t, *list, item, item.register_id == instruction->r3, found);
                if (found == NULL) {
                    reg_alloc_entry_t new_item;
                    new_item.register_id = instruction->r3;
                    new_item.start = index;
                    new_item.end = index;
                    nlist_insert(reg_alloc_entry_t, *list, new_item);
                } else {
                    found->end = index;
                }
            }
            break;

        case store_ai_r:
        case load_i:
        case cbr:
        case jump_i:
        case label:
        case push:
        case pop:
            // Does not have r3
            break;

        default:
            // Error
            fprintf(stderr, "Instruction %d error #3\n", instruction->instruction);
            exit(EXIT_FAILURE);
            break;
    }
}

void allocated_registers(iloc_program_t *program) {
    iloc_program_t *new_program = iloc_program_new();
    rae_list_t list;
    nlist_init(reg_alloc_entry_t, list);

    int64_t ebx = -1;
    int64_t ecx = -1;
    int64_t edx = -1;

    reg_alloc_entry_t *found;
    for (uint64_t i = 0; i < program->length; i++) {
        handle_instruction(&list, &program->instructions[i], i);
    }

    for (size_t i = 0; i < list.length; i++) {
        reg_alloc_entry_t rae = nlist_get_unsafe(list, i);
        fprintf(stderr, "%lu [%lu..=%lu]\n", rae.register_id, rae.start, rae.end);
    }

    graph_t graph;
    nlist_init(graph_node_t, graph);

    for (size_t i = 0; i < list.length; i++) {
        reg_alloc_entry_t entry = nlist_get_unsafe(list, i);

        graph_node_t node;
        nlist_init(graph_node_t*, node.next);
        node.register_id = entry.register_id;
        nlist_insert(graph_node_t, graph, node);

        for (size_t j = 0; j < i; j++) {
            reg_alloc_entry_t other = nlist_get_unsafe(list, j);

            if ((entry.start <= other.start && other.start <= entry.end)
                    || (other.start <= entry.start && entry.start <= other.end)) {
                nlist_insert(graph_node_t*, nlist_get_unsafe(graph, i).next, &nlist_get_unsafe(graph, j));
                nlist_insert(graph_node_t*, nlist_get_unsafe(graph, j).next, &nlist_get_unsafe(graph, i));
            }
        }
    }

    for (size_t i = 0; i < graph.length; i++) {
        graph_node_t node = nlist_get_unsafe(graph, i);

        fprintf(stdout, "%lu [ ", node.register_id);

        for (size_t j = 0; j < node.next.length; j++) {
            graph_node_t *next = nlist_get_unsafe(node.next, j);
            fprintf(stdout, "%lu, ", next->register_id);
        }

        fprintf(stdout, "]\n");
    }
}
