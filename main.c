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
        iloc_program_to_string(program->program);
        print_ast(stderr, program);
    }

    return 0;
}

