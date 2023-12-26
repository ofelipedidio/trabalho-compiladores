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
#include "structs.h"
#include "print.h"

extern int yyparse(void);
extern int yylex_destroy(void);

void *arvore = NULL;

uint32_t lines[1024];

void recr_print(FILE *file, ast_t *ast, int left);

void print_ast(FILE *file, ast_t *ast) {
    print_ast_label(file, ast->label);
    if (ast->type != type_undefined) {
        fprintf(file, " [type=");
        print_type(file, ast->type);
        fprintf(file, "]");
    }
    if (ast->lexeme != NULL) {
        fprintf(file, " [value=");
        print_lexeme(file, ast->lexeme);
        fprintf(file, "]");
    }
    fprintf(file, "\n");
}

void recr_print(FILE *file, ast_t *ast, int left) {
    for (uint64_t i = 0; i < ast->length; i++) {
        for (int i = 0; i < left; i++) {
            if (lines[i] != 0) {
                fprintf(file, "│ ");
            } else {
                fprintf(file, "  ");
            }
        }
        if (i + 1 == ast->length) {
            fprintf(file, "└─");
            lines[left] = 0;
        } else {
            fprintf(file, "├─");
            lines[left] = 1;
        }
        print_ast(file, ast->children[i]);
        recr_print(file, ast->children[i], left+1);
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
        print_ast(stderr, program);
        recr_print(stderr, program, 0);
    }

    return 0;
}

