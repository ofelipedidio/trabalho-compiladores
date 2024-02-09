/* Porto Alegre, Novembro de 2023
 * INF01147 - Compiladores
 *
 * Grupo B
 * Felipe Souza Didio - 00323392
 * Pedro Company Beck - 00324055
 *
 */

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
        switch (instruction.instruction) {
            case add:
                // [add r1, r2 => r3]
                // movl <r1>, %eax
                // addl <r2>, %eax
                // movl %eax, <r3>

                break;
            case sub:
                // [sub r1, r2 => r3]
                // movl <r1>, %eax
                // subl <r2>, %eax
                // movl %eax, <r3>
                break;

            case mult:
                // [mult r1, r2 => r3]
                // movl <r1>, %eax
                // imull <r2>, %eax
                // movl %eax, <r3>
                break;

            case _div:
                // [div r1, r2 => r3]
                // push %edx
                // push %ecx
                // movl	<r1>, %eax
                // movl	<r2>, %ecx
                // cltd
                // idivl %ecx
                // movl	%eax, <r3>
                // pop %ecx
                // pop %edx
                break;

            case rsub_i:
                // [rsubI r1, c2 => r3]
                // movl $<c2>, %eax
                // subl <r1>, %eax
                // movl %eax, <r3>
                break;

            case load:
                // [loadI c1 => r2]
                // movl $<c1>, <r2>
                break;

            case load_ai_r:
                // [loadAI reg(r1), c2 => r3]
                // movl c2(<r1>), %eax
                // movl %eax, r3(%rip)
                break;

            case store_ai:
                // [storeAI r1 => reg(r2), c3]
                // movl <r1>, c3(<r2>)
                break;

            case cmp_lt:
                // cmp_lt
                //   movl    <r1>, %eax
                //   cmpl    <r2>, %eax
                //   jl    L2
                //   movl    $0, <r3>
                //   jmp    L3
                // L2:
                //   movl    $1, <r3>
                // L3:
                break;

            case cmp_le:
                // cmp_le
                //   movl    <r1>, %eax
                //   cmpl    <r2>, %eax
                //   jle    L2
                //   movl    $0, <r3>
                //   jmp    L3
                // L2:
                //   movl    $1, <r3>
                // L3:
                break;

            case cmp_eq:
                // cmp_eq
                //   movl    <r1>, %eax
                //   cmpl    <r2>, %eax
                //   je    L2
                //   movl    $0, <r3>
                //   jmp    L3
                // L2:
                //   movl    $1, <r3>
                // L3:
                break;

            case cmp_ge:
                // cmp_ge
                //   movl    <r1>, %eax
                //   cmpl    <r2>, %eax
                //   jge    L2
                //   movl    $0, <r3>
                //   jmp    L3
                // L2:
                //   movl    $1, <r3>
                // L3:
                break;

            case cmp_gt:
                // cmp_gt
                //   movl    <r1>, %eax
                //   cmpl    <r2>, %eax
                //   jg    L2
                //   movl    $0, <r3>
                //   jmp    L3
                // L2:
                //   movl    $1, <r3>
                // L3:
                break;

            case cmp_ne:
                // cmp_ne
                //   movl    <r1>, %eax
                //   cmpl    <r2>, %eax
                //   jne    L2
                //   movl    $0, <r3>
                //   jmp    L3
                // L2:
                //   movl    $1, <r3>
                // L3:
                break;

            case cbr:
                // cbr
                // movl <r1>, %eax
                // jz L3
                // jmp L2
                // L3:
                //
                // L2:
                break;

            case jump_i:
                // jmp <l1>
                break;

            case label:
                // l1:
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
    fprintf(stderr, "Reached main with code %d (arvore = %p)\n", ret, arvore);
    yylex_destroy();
    if (ret != 0) {
        return ret;
    }

    ast_t *program = (ast_t*) arvore;
    if (program != NULL) {
        // ast_program_export(program);
        // ast_program_free(program);
        iloc_program_to_string(program->program);
        print_ast(stderr, program);
    }

    return 0;
}

