/* Porto Alegre, Novembro de 2023
 * INF01147 - Compiladores
 *
 * Grupo B
 * Felipe Souza Didio - 00323392
 * Pedro Company Beck - 00324055
 *
 */

#include <stdio.h>
#include "ast.h"
#include "lexeme.h"

extern int yyparse(void);
extern int yylex_destroy(void);

void *arvore = NULL;
// void exporta (void *arvore);

int recr_print_lines[1024];

int main (int argc, char **argv) {
    int ret = yyparse(); 
    fprintf(stderr, "Reached main with code %d (arvore = %p)\n", ret, arvore);
    yylex_destroy();
    if (ret != 0) {
        return ret;
    }

    program_t *program = (program_t*) arvore;
    if (program != NULL) {
        // ast_program_export(program);
        ast_program_free(program);
    }

    return 0;
}

