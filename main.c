/* Porto Alegre, Novembro de 2023
 * INF01147 - Compiladores
 *
 * Grupo B
 * Felipe Souza Didio - 00323392
 * Pedro Company Beck - 00324055
 *
 */

#define GENERATE_COMMENTS

#include <stdio.h>
#include <stdlib.h>
#include "code_gen.h"
#include "list.h"
#include "structs.h"
#include "print.h"

extern int yyparse(void);
extern int yylex_destroy(void);

char *program_name;

void *arvore = NULL;

void temp() {
    // TODO: data segment (from semantics' scope)
    // .align 4
    // .type	<varname>, @object
    // .size	<varname>, 4
    // <varname>:
    // .zero	4

    // TODO: register allocation
    ast_t *tree = ((ast_t*) arvore);
    allocated_registers(tree->program);

    // TODO: text segment (from arvore.program)
    for (uint64_t i = 0; i < tree->program->length; i++) {
        iloc_instruction_t instruction = tree->program->instructions[i];
#ifdef GENERATE_COMMENTS
        if (instruction.instruction != label) {
            fprintf(stdout, "// ");
            iloc_instruction_to_string(&instruction);
        }
#endif

        switch (instruction.instruction) {
        case add:
          // [add r1, r2 => r3]
          // movl <r1>, %eax
          fprintf(stdout, "movl %%%s, %%eax\n", x86_reg_to_string(int_to_x86(instruction.r1)));
          // addl <r2>, %eax
          fprintf(stdout, "addl %%%s, %%eax\n", x86_reg_to_string(int_to_x86(instruction.r2)));
          // movl %eax, <r3>
          fprintf(stdout, "movl %%eax, %%%s\n", x86_reg_to_string(int_to_x86(instruction.r3)));
          break;

        case sub:
          // [sub r1, r2 => r3]
          // movl <r1>, %eax
          fprintf(stdout, "movl %%%s, %%eax\n", x86_reg_to_string(int_to_x86(instruction.r1)));
          // subl <r2>, %eax
          fprintf(stdout, "subl %%%s, %%eax\n", x86_reg_to_string(int_to_x86(instruction.r2)));
          // movl %eax, <r3>
          fprintf(stdout, "movl %%eax, %%%s\n", x86_reg_to_string(int_to_x86(instruction.r3)));
          break;

        case mult:
          // [mult r1, r2 => r3]
          // movl <r1>, %eax
          fprintf(stdout, "movl %%%s, %%eax\n" , x86_reg_to_string(int_to_x86(instruction.r1)));
          // imull <r2>, %eax
          fprintf(stdout, "imull %%%s, %%eax\n", x86_reg_to_string(int_to_x86(instruction.r2)));
          // movl %eax, <r3>
          fprintf(stdout, "movl %%eax, %%%s\n" , x86_reg_to_string(int_to_x86(instruction.r3)));
          break;

        case _div:
          // [div r1, r2 => r3]
          // push %edx
          fprintf(stdout, "push %%edx\n");
          // push %ecx
          fprintf(stdout, "push %%ecx\n");
          // movl	<r1>, %eax
          fprintf(stdout, "movl %%%s, %%eax\n" , x86_reg_to_string(int_to_x86(instruction.r1)));
          // movl	<r2>, %ecx
          fprintf(stdout, "movl %%%s, %%ecx\n" , x86_reg_to_string(int_to_x86(instruction.r2)));
          // cltd
          fprintf(stdout, "cltd\n");
          // idivl %ecx
          fprintf(stdout, "idivl %%ecx\n");
          // movl	%eax, <r3>
          fprintf(stdout, "movl %%eax, %%%s\n" , x86_reg_to_string(int_to_x86(instruction.r3)));
          // pop %ecx
          fprintf(stdout, "pop %%ecx\n");
          // pop %edx
          fprintf(stdout, "pop %%edx\n");
          break;

        case rsub_i:
          // [rsubI r1, c2 => r3]
          // TODO : VER MAPEAMENTO DAS CONSTANTES (valores com c1, c2, etc)
          // movl $<c2>, %eax
          // subl <r1>, %eax
          fprintf(stdout, "subl %%%s, %%eax\n" , x86_reg_to_string(int_to_x86(instruction.r1)));
          // movl %eax, <r3>
          fprintf(stdout, "movl %%eax, %%%s\n" , x86_reg_to_string(int_to_x86(instruction.r3)));
          break;

        case load_i:
          // [loadI c1 => r2]
          // movl $<c1>, <r2>
          fprintf(stdout, "movl $%lu, %%%s\n", instruction.r1, x86_reg_to_string(int_to_x86(instruction.r2)));
          break;

        case load_ai_r:
          // [loadAI reg(r1), c2 => r3]
          // movl c2(<r1>), %eax
          fprintf(stdout, "movl %lu(%%%s), %%%s\n", 
                  instruction.r2,
                  x86_reg_to_string(int_to_x86(instruction.r1)),
                  x86_reg_to_string(int_to_x86(instruction.r3)));
          break;

        case store_ai_r:
          // [storeAI r1 => reg(r2), c3]
          // movl <r1>, c3(<r2>)
          fprintf(stdout, "movl %%%s, %lu(%%%s)\n", 
                  x86_reg_to_string(int_to_x86(instruction.r1)),
                  instruction.r3,
                  x86_reg_to_string(int_to_x86(instruction.r2)));
          break;

        case cmp_lt:
          // cmp_lt
          //   movl    <r1>, %eax
          fprintf(stdout, "movl %s, %%eax\n" , x86_reg_to_string(int_to_x86(instruction.r1)));
          //   cmpl    <r2>, %eax
          fprintf(stdout, "cmpl %s, %%eax\n" , x86_reg_to_string(int_to_x86(instruction.r2)));
          // TODO : VER MAPEAMENTO DAS LABELS
          //   jl    L2
          //   movl    $0, <r3>
          fprintf(stdout, "movl $0, %s\n", x86_reg_to_string(int_to_x86(instruction.r3)));
          //   jmp    L3
          // L2:
          //   movl    $1, <r3>
          fprintf(stdout, "movl $1, %s\n", x86_reg_to_string(int_to_x86(instruction.r3)));
          // L3:
          break;

        case cmp_le:
          // cmp_le
          //   movl    <r1>, %eax
          fprintf(stdout, "movl %s, %%eax\n" , x86_reg_to_string(int_to_x86(instruction.r1)));
          //   cmpl    <r2>, %eax
          fprintf(stdout, "cmpl %s, %%eax\n" , x86_reg_to_string(int_to_x86(instruction.r2)));
          //   jle    L2
          //   movl    $0, <r3>
          fprintf(stdout, "movl $0, %s\n", x86_reg_to_string(int_to_x86(instruction.r3)));
          //   jmp    L3
          // L2:
          //   movl    $1, <r3>
          fprintf(stdout, "movl $1, %s\n", x86_reg_to_string(int_to_x86(instruction.r3)));
          // L3:
          break;

        case cmp_eq:
          // cmp_eq
          //   movl    <r1>, %eax
          fprintf(stdout, "movl %s, %%eax\n" , x86_reg_to_string(int_to_x86(instruction.r1)));
          //   cmpl    <r2>, %eax
          fprintf(stdout, "cmpl %s, %%eax\n" , x86_reg_to_string(int_to_x86(instruction.r2)));
          //   je    L2
          //   movl    $0, <r3>
          fprintf(stdout, "movl $0, %s\n", x86_reg_to_string(int_to_x86(instruction.r3)));
          //   jmp    L3
          // L2:
          //   movl    $1, <r3>
          fprintf(stdout, "movl $1, %s\n", x86_reg_to_string(int_to_x86(instruction.r3)));
          // L3:
          break;

        case cmp_ge:
          // cmp_ge
          //   movl    <r1>, %eax
          fprintf(stdout, "movl %s, %%eax\n" , x86_reg_to_string(int_to_x86(instruction.r1)));
          //   cmpl    <r2>, %eax
          fprintf(stdout, "cmpl %s, %%eax\n" , x86_reg_to_string(int_to_x86(instruction.r2)));
          //   jge    L2
          //   movl    $0, <r3>
          fprintf(stdout, "movl $0, %s\n", x86_reg_to_string(int_to_x86(instruction.r3)));
          //   jmp    L3
          // L2:
          //   movl    $1, <r3>
          fprintf(stdout, "movl $1, %s\n", x86_reg_to_string(int_to_x86(instruction.r3)));
          // L3:
          break;

        case cmp_gt:
          // cmp_gt
          //   movl    <r1>, %eax
          fprintf(stdout, "movl %s, %%eax\n" , x86_reg_to_string(int_to_x86(instruction.r1)));
          //   cmpl    <r2>, %eax
          fprintf(stdout, "cmpl %s, %%eax\n" , x86_reg_to_string(int_to_x86(instruction.r2)));
          //   jg    L2
          //   movl    $0, <r3>
          fprintf(stdout, "movl $0, %s\n", x86_reg_to_string(int_to_x86(instruction.r3)));
          //   jmp    L3
          // L2:
          //   movl    $1, <r3>
          fprintf(stdout, "movl $1, %s\n", x86_reg_to_string(int_to_x86(instruction.r3)));
          // L3:
          break;

        case cmp_ne:
          // cmp_ne
          //   movl    <r1>, %eax
          fprintf(stdout, "movl %s, %%eax\n" , x86_reg_to_string(int_to_x86(instruction.r1)));
          //   cmpl    <r2>, %eax
          fprintf(stdout, "cmpl %s, %%eax\n" , x86_reg_to_string(int_to_x86(instruction.r2)));
          //   jne    L2
          //   movl    $0, <r3>
          fprintf(stdout, "movl $0, %s\n", x86_reg_to_string(int_to_x86(instruction.r3)));
          //   jmp    L3
          // L2:
          //   movl    $1, <r3>
          fprintf(stdout, "movl $1, %s\n", x86_reg_to_string(int_to_x86(instruction.r3)));
          // L3:
          break;

        case cbr:
          // cbr
          // movl <r1>, %eax
          fprintf(stdout, "movl %%%s, %%eax\n" , x86_reg_to_string(int_to_x86(instruction.r1)));
          fprintf(stdout, "test %%eax, %%eax\n");
          // jz L3
          fprintf(stdout, "jz L%lu\n" , instruction.r2);
          // jmp L2
          fprintf(stdout, "jmp L%lu\n" , instruction.r3);
          break;

        case jump_i:
          // jmp <l1>
          fprintf(stdout, "jmp L%lu\n", instruction.r1);
          break;

        case label:
          // l1:
          fprintf(stdout, "L%lu:\n", instruction.r1);
          break;

        case push:
          // l1:
          fprintf(stdout, "push %%%s\n", x86_reg_to_string(int_to_x86(instruction.r1)));
          break;

        case pop:
          // l1:
          fprintf(stdout, "pop %%%s\n", x86_reg_to_string(int_to_x86(instruction.r1)));
          break;

        default:
          fprintf(stderr, "[1]    42069 segmentation fault (core dumped)  %s\n", program_name);
          // TODO - Didio: Removed the next print
          fprintf(stderr, "Nao foi segfault, foi a instrucao %d\n", instruction.instruction); // DEBUG ONLY
          exit(EXIT_FAILURE);
          break;
        }
    }
}

int main (int argc, char **argv) {
    program_name = argv[0];
    int ret = yyparse(); 
    // fprintf(stderr, "Reached main with code %d (arvore = %p)\n", ret, arvore);
    yylex_destroy();
    if (ret != 0) {
        return ret;
    }

    ast_t *program = (ast_t*) arvore;
    if (program != NULL) {
        fprintf(stdout, "= ILOC =========================================================================\n");
        // ast_program_export(program);
        // ast_program_free(program);
        for (uint64_t i = 0; i < program->program->length; i++) {
            fprintf(stdout, "%lu ", i);
            iloc_instruction_to_string(&program->program->instructions[i]);
        }
        // print_ast(stderr, program);
        fprintf(stdout, "================================================================================\n\n\n= Assembly =====================================================================\n");
        temp();
        fprintf(stdout, "================================================================================\n");
    }

    return 0;
}

