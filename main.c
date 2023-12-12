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

/*
void debug_print(ast_t *node, int depth);

void debug_print(ast_t *node, int depth) {
    fprintf(stderr, "%s", ast_node_type_name(node->type));
    switch (node->type) {
        case lit_int_type:
        case lit_float_type:
        case lit_bool_type:
        case ident:
            fprintf(stderr, "[");
            lexeme_to_string(node->body.leaf, stderr);
            fprintf(stderr, "]\n");
            break;
        default:
            fprintf(stderr, "\n");
            for (int i = 0; i < node->body.node.n; i++) {
                for (int i = 0; i < depth; i++) {
                    fprintf(stderr, recr_print_lines[i] ? "│ " : "  ");
                }
                recr_print_lines[depth] = i + 1 != node->body.node.n;
                fprintf(stderr, (i + 1 == node->body.node.n) ? "└─" : "├─");
                debug_print(node->body.node.children[i], depth+1);
            }
            break;
    }
}

void exporta (void *arvore) {
    if (arvore == NULL) {
        return;
    }

    ast_t *ast = (ast_t*) arvore;

    switch (ast->type) {
        case noop:
            break;
        case call_argument:
            break;
        case func_declaration:
            fprintf(stdout, "%p [label=\"%s\"]\n", ast, ast->body.node.children[0]->body.leaf.value.ident);
            break;
        case statement_decl:
            fprintf(stdout, "%p [label=\"=\"]\n", ast);
            break;
        case statement_attr:
            fprintf(stdout, "%p [label=\"=\"]\n", ast);
            break;
        case statement_call:
            fprintf(stdout, "%p [label=\"call %s\"]\n", ast, ast->body.node.children[0]->body.leaf.value.ident);
            break;
        case statement_return:
            fprintf(stdout, "%p [label=\"return\"]\n", ast);
            break;
        case statement_if:
            fprintf(stdout, "%p [label=\"if\"]\n", ast);
            break;
        case statement_while:
            fprintf(stdout, "%p [label=\"while\"]\n", ast);
            break;
        case expr_not:
            fprintf(stdout, "%p [label=\"!\"]\n", ast);
            break;
        case expr_inv:
            fprintf(stdout, "%p [label=\"-\"]\n", ast);
            break;
        case expr_mult:
            fprintf(stdout, "%p [label=\"*\"]\n", ast);
            break;
        case expr_div:
            fprintf(stdout, "%p [label=\"/\"]\n", ast);
            break;
        case expr_mod:
            fprintf(stdout, "%p [label=\"%%\"]\n", ast);
            break;
        case expr_add:
            fprintf(stdout, "%p [label=\"+\"]\n", ast);
            break;
        case expr_sub:
            fprintf(stdout, "%p [label=\"-\"]\n", ast);
            break;
        case expr_lt:
            fprintf(stdout, "%p [label=\"<\"]\n", ast);
            break;
        case expr_gt:
            fprintf(stdout, "%p [label=\">\"]\n", ast);
            break;
        case expr_le:
            fprintf(stdout, "%p [label=\"<=\"]\n", ast);
            break;
        case expr_ge:
            fprintf(stdout, "%p [label=\">=\"]\n", ast);
            break;
        case expr_eq:
            fprintf(stdout, "%p [label=\"==\"]\n", ast);
            break;
        case expr_ne:
            fprintf(stdout, "%p [label=\"!=\"]\n", ast);
            break;
        case expr_and:
            fprintf(stdout, "%p [label=\"&\"]\n", ast);
            break;
        case expr_or:
            fprintf(stdout, "%p [label=\"|\"]\n", ast);
            break;
        case lit_int_type:
            fprintf(stdout, "%p [label=\"%lld\"]\n", ast, ast->body.leaf.value.lit_int);
            break;
        case lit_float_type:
            fprintf(stdout, "%p [label=\"%f\"]\n", ast, ast->body.leaf.value.lit_float);
            break;
        case lit_bool_type:
            fprintf(stdout, "%p [label=\"%s\"]\n", ast, ast->body.leaf.value.lit_bool ? "true" : "false");
            break;
        case ident:
            fprintf(stdout, "%p [label=\"%s\"]\n", ast, ast->body.leaf.value.ident);
            break;
        default:
            fprintf(stdout, "df\n");
            break;
    }

    int i = 0;
    switch (ast->type) {
        // Ignore
        case lit_int_type:
        case lit_float_type:
        case lit_bool_type:
        case ident:
            return;

        // Skip first argument
        case statement_call:
        case func_declaration:
            i = 1;
            break;

        // Special case
        case call_argument:
            i = ast->body.node.n;
            if (ast->body.node.n == 1) {
                exporta(ast->body.node.children[0]);
            } else {
                fprintf(stdout, "%p, %p\n", ast->body.node.children[0], ast->body.node.children[1]->body.node.children[0]);
                exporta(ast->body.node.children[0]);
                exporta(ast->body.node.children[1]);
            }
            break;

        default:
            break;
    }

    for (; i < ast->body.node.n; i++) {
        switch (ast->body.node.children[i]->type) {
            // No print
            case noop:
                break;

            case call_argument:
                fprintf(stdout, "%p, %p\n", ast, ast->body.node.children[i]->body.node.children[0]);
                exporta(ast->body.node.children[i]);
                break;

            default:
                fprintf(stdout, "%p, %p\n", ast, ast->body.node.children[i]);
                exporta(ast->body.node.children[i]);
                break;

        }
    }
}
*/

int main (int argc, char **argv)
{
    int ret = yyparse(); 
    // exporta (arvore);
    yylex_destroy();
    if (ret != 0) {
        return ret;
    }

    program_t *program = (program_t*) arvore;
    if (program != NULL) {
        ast_program_export(program);
        ast_program_free(program);
    }

    return 0;
}

