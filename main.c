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

char buffer[1024];

extern int yyparse(void);
extern int yylex_destroy(void);

void *arvore = NULL;
void exporta (void *arvore);

int recr_print_lines[1024];

void recr_print(ast_t *node, int depth);

void recr_print(ast_t *node, int depth) {
    ast_value_label(node->value, buffer);
    printf("%s[%s]\n", ast_node_type_name(node->type), buffer);
    for (int i = 0; i < node->n; i++) {
        for (int i = 0; i < depth; i++) {
            printf(recr_print_lines[i] ? "│ " : "  ");
        }
        recr_print_lines[depth] = i + 1 != node->n;
        printf((i + 1 == node->n) ? "└─" : "├─");
        recr_print(node->children[i], depth+1);
    }
}

int main (int argc, char **argv)
{
    int ret = yyparse(); 
    exporta (arvore);
    yylex_destroy();

    ast_t *tree = (ast_t*) arvore;
    recr_print(tree, 0);
    ast_free(tree);

    return ret;
}

void exporta (void *arvore) {
}
