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
        // ast_program_export(program);
        // ast_program_free(program);
        for (size_t i = 0; i < program->_program.length; i++) {
            // fprintf(stdout, "%lu ", i);
            fprintf(stdout, "%s", program->_program.items[i]);
        }
        // print_ast(stderr, program);
    }

    return 0;
}

