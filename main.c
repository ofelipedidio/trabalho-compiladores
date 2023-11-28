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

char buffer[1024];

extern int yyparse(void);
extern int yylex_destroy(void);

void *arvore = NULL;
void exporta (void *arvore);

int recr_print_lines[1024];

void recr_print(ast_t *node, int depth);

void recr_print(ast_t *node, int depth) {
    printf("%s", ast_node_type_name(node->type));
    switch (node->type) {
        case lit_int_type:
        case lit_float_type:
        case lit_bool_type:
        case ident:
            printf("[");
            lexeme_to_string(node->body.leaf, stdout);
            printf("]\n");
            break;
        default:
            printf("\n");
            for (int i = 0; i < node->body.node.n; i++) {
                for (int i = 0; i < depth; i++) {
                    printf(recr_print_lines[i] ? "│ " : "  ");
                }
                recr_print_lines[depth] = i + 1 != node->body.node.n;
                printf((i + 1 == node->body.node.n) ? "└─" : "├─");
                recr_print(node->body.node.children[i], depth+1);
            }
            break;
    }
}

int main (int argc, char **argv)
{
    int ret = yyparse(); 
    exporta (arvore);
    yylex_destroy();

    ast_t *tree = (ast_t*) arvore;
    if (tree != NULL) {
        recr_print(tree, 0);
        ast_free(tree);
    } else {
        printf("<empty ast> (no functions parsed)\n");
    }

    return ret;
}

void exporta (void *arvore) {
}
