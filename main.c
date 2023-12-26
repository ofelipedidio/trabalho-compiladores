/* Porto Alegre, Novembro de 2023
 * INF01147 - Compiladores
 *
 * Grupo B
 * Felipe Souza Didio - 00323392
 * Pedro Company Beck - 00324055
 *
 */

#include "code_gen.h"
#include "list.h"
#include "structs.h"
#include <stdio.h>

extern int yyparse(void);
extern int yylex_destroy(void);

void *arvore = NULL;

uint32_t lines[1024];

void recr_print(ast_t *ast, int left);

void print_ast(ast_t *ast) {
    print_ast_label(ast->label);
    if (ast->type != type_undefined) {
        fprintf(stderr, " [type=");
        print_type(ast->type);
        fprintf(stderr, "]");
    }
    if (ast->lexeme != NULL) {
        fprintf(stderr, " [value=");
        print_lexeme(ast->lexeme);
        fprintf(stderr, "]");
    }
    fprintf(stderr, "\n");
}

void recr_print(ast_t *ast, int left) {
    for (uint64_t i = 0; i < ast->length; i++) {
        for (int i = 0; i < left; i++) {
            if (lines[i] != 0) {
                fprintf(stderr, "│ ");
            } else {
                fprintf(stderr, "  ");
            }
        }
        if (i + 1 == ast->length) {
            fprintf(stderr, "└─");
            lines[left] = 0;
        } else {
            fprintf(stderr, "├─");
            lines[left] = 1;
        }
        print_ast(ast->children[i]);
        recr_print(ast->children[i], left+1);
    }
}

int main (int argc, char **argv) {
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
        print_ast(program);
        recr_print(program, 0);
    }

    return 0;
}

