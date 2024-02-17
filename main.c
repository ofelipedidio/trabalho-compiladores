/* Porto Alegre, Novembro de 2023
 * INF01147 - Compiladores
 *
 * Grupo B
 * Felipe Souza Didio - 00323392
 * Pedro Company Beck - 00324055
 *
 */

#include <stdio.h>
#include "code_gen.h"
#include "list.h"
#include "new_list.h"
#include "structs.h"
#include "print.h"

extern int yyparse(void);
extern int yylex_destroy(void);

void *arvore = NULL;

typedef struct {
    int64_t id;
    int64_t next_1; // label
    int64_t next_2; // label
    int64_t next_i; // label
    int64_t next_nat; // label
} block_t;

typedef nlist_definition(block_t) blocks_t;

uint64_t next_id = 0;

block_t new_block() {
    block_t block;
    block.id = -1;
    block.next_1 = -1;
    block.next_2 = -1;
    block.next_i = -1;
    block.next_nat = -1;
    return block;
}

void generate_graph(ast_t *program) {
    blocks_t blocks;
    nlist_init(blocks);

    block_t current_block;
    current_block = new_block();
    int ignore_label = 0;

    // inst 1
    // inst 2
    // cbr r1, label2, label1

    // label:
    // inst 3
    // cbr r1, label, label1

    // label1:

    fprintf(stdout, "digraph {\n");
    fprintf(stdout, "node [shape=\"box\"];\n");
    fprintf(stdout, "\"%ld\" [label=\"", current_block.id);
    for (uint64_t i = 0; i < program->program->length; i++) {
        iloc_instruction_t inst = program->program->instructions[i];
        switch (inst.instruction) {
            case cbr:
                iloc_instruction_to_string(&inst);
                current_block.next_1 = inst.r2;
                current_block.next_2 = inst.r3;
                ignore_label = 1;
                nlist_push(blocks, current_block);
                fprintf(stdout, "\"];\n");
                break;
            case jump_i:
                iloc_instruction_to_string(&inst);
                current_block.next_i = inst.r1;
                ignore_label = 1;
                nlist_push(blocks, current_block);
                fprintf(stdout, "\"];\n");
                break;
            case label:
                // TODO: store handle
                if (!ignore_label) {
                    current_block.next_nat = inst.r1;
                    nlist_push(blocks, current_block);
                    fprintf(stdout, "\"];\n");
                }

                current_block = new_block();
                current_block.id = inst.r1;
                ignore_label = 0;
                fprintf(stdout, "\"%ld\" [label=\"", current_block.id);
                iloc_instruction_to_string(&inst);
                break;
            default:
                iloc_instruction_to_string(&inst);
                ignore_label = 0;
                break;
        }
    }
    nlist_push(blocks, current_block);
    fprintf(stdout, "\"];\n");

    for (size_t i = 0; i < blocks.length; i++) {
        block_t block = nlist_get_unsafe(blocks, i);
        if (block.next_1 >= 0) {
            fprintf(stdout, "\"%ld\" -> \"%ld\" [label=\"true\"];\n", block.id, block.next_1);
        }
        if (block.next_2 >= 0) {
            fprintf(stdout, "\"%ld\" -> \"%ld\" [label=\"false\"];\n", block.id, block.next_2);
        }
        if (block.next_i >= 0) {
            fprintf(stdout, "\"%ld\" -> \"%ld\";\n", block.id, block.next_i);
        }
        if (block.next_nat >= 0) {
            fprintf(stdout, "\"%ld\" -> \"%ld\";\n", block.id, block.next_nat);
        }
    }
    fprintf(stdout, "}\n");
}

int main (int argc, char **argv) {
    int ret = yyparse(); 
    // fprintf(stderr, "Reached main with code %d (arvore = %p)\n", ret, arvore);
    yylex_destroy();
    if (ret != 0) {
        return ret;
    }

    ast_t *program = (ast_t*) arvore;
    if (program != NULL) {
        // ast_program_export(program);
        // ast_program_free(program);
        // iloc_program_to_string(program->program);
        // print_ast(stderr, program);
        generate_graph(program);
    }

    return 0;
}

