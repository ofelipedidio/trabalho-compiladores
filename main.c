/* Porto Alegre, Novembro de 2023
 * INF01147 - Compiladores
 *
 * Grupo B
 * Felipe Souza Didio - 00323392
 * Pedro Company Beck - 00324055
 *
 */

#include <stdio.h>

extern int yyparse(void);
extern int yylex_destroy(void);

void *arvore = NULL;
void exporta (void *arvore);

struct tree_t {

};

int main (int argc, char **argv)
{
    int ret = yyparse(); 
    exporta (arvore);
    yylex_destroy();
    return ret;
}

void exporta (void *arvore) {
}
