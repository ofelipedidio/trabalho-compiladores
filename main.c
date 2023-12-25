/* Porto Alegre, Novembro de 2023
 * INF01147 - Compiladores
 *
 * Grupo B
 * Felipe Souza Didio - 00323392
 * Pedro Company Beck - 00324055
 *
 */

#include "list.h"
#include "structs.h"
#include <stdio.h>

extern int yyparse(void);
extern int yylex_destroy(void);

void *arvore = NULL;

uint32_t lines[1024];

void recr_print(ast_t *ast, int left);

void print_ast(ast_t *ast) {
    switch (ast->label) {
        case ast_global_list:
            fprintf(stderr, "ast_global_list");
            break;
        case ast_var_decl:
            fprintf(stderr, "ast_var_decl");
            break;
        case ast_func_decl:
            fprintf(stderr, "ast_func_decl");
            break;
        case ast_func_header:
            fprintf(stderr, "ast_func_header");
            break;
        case ast_command_list:
            fprintf(stderr, "ast_command_list");
            break;
        case ast_assignment:
            fprintf(stderr, "ast_assignment");
            break;
        case ast_call:
            fprintf(stderr, "ast_call");
            break;
        case ast_return:
            fprintf(stderr, "ast_return");
            break;
        case ast_if:
            fprintf(stderr, "ast_if");
            break;
        case ast_while:
            fprintf(stderr, "ast_while");
            break;
        case ast_expr_or:
            fprintf(stderr, "ast_expr_or");
            break;
        case ast_expr_and:
            fprintf(stderr, "ast_expr_and");
            break;
        case ast_expr_eq:
            fprintf(stderr, "ast_expr_eq");
            break;
        case ast_expr_ne:
            fprintf(stderr, "ast_expr_ne");
            break;
        case ast_expr_lt:
            fprintf(stderr, "ast_expr_lt");
            break;
        case ast_expr_gt:
            fprintf(stderr, "ast_expr_gt");
            break;
        case ast_expr_le:
            fprintf(stderr, "ast_expr_le");
            break;
        case ast_expr_ge:
            fprintf(stderr, "ast_expr_ge");
            break;
        case ast_expr_add:
            fprintf(stderr, "ast_expr_add");
            break;
        case ast_expr_sub:
            fprintf(stderr, "ast_expr_sub");
            break;
        case ast_expr_mul:
            fprintf(stderr, "ast_expr_mul");
            break;
        case ast_expr_div:
            fprintf(stderr, "ast_expr_div");
            break;
        case ast_expr_mod:
            fprintf(stderr, "ast_expr_mod");
            break;
        case ast_expr_inv:
            fprintf(stderr, "ast_expr_inv");
            break;
        case ast_expr_not:
            fprintf(stderr, "ast_expr_not");
            break;
        case ast_ident:
            fprintf(stderr, "ast_val_lit");
            break;
        case ast_val_int:
            fprintf(stderr, "ast_val_int");
            break;
        case ast_val_float:
            fprintf(stderr, "ast_val_float");
            break;
        case ast_val_bool:
            fprintf(stderr, "ast_val_bool");
            break;
    }
    if (ast->type != type_undefined) {
        fprintf(stderr, " [type=");
        switch (ast->type) {
            case type_undefined:
                fprintf(stderr, "undefined");
                break;
            case type_int:
                fprintf(stderr, "int");
                break;
            case type_float:
                fprintf(stderr, "float");
                break;
            case type_bool:
                fprintf(stderr, "bool");
                break;
        }
        fprintf(stderr, "]");
    }
    if (ast->lexeme != NULL) {
        fprintf(stderr, " [value=");
        switch (ast->lexeme->lex_ident_t.type) {
            case lex_ident:
                fprintf(stderr, "\"%s\"", ast->lexeme->lex_ident_t.value);
                break;
            case lex_int:
                fprintf(stderr, "%ld", ast->lexeme->lex_int_t.value);
                break;
            case lex_float:
                fprintf(stderr, "%f", ast->lexeme->lex_float_t.value);
                break;
            case lex_bool:
                fprintf(stderr, "%s", ast->lexeme->lex_bool_t.value == 0 ? "false" : "true");
                break;
        }
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
        print_ast(ast);
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

