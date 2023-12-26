#include <stdio.h>
#include "structs.h"

void print_type(FILE *file, type_t type) {
    switch (type) {
        case type_undefined:
            fprintf(file, "undefined");
            break;
        case type_int:
            fprintf(file, "int");
            break;
        case type_float:
            fprintf(file, "float");
            break;
        case type_bool:
            fprintf(file, "bool");
            break;
    }
}

void print_ast_label(FILE *file, ast_label_t label) {
    switch (label) {
        case ast_global_list:
            fprintf(file, "ast_global_list");
            break;
        case ast_var_decl:
            fprintf(file, "ast_var_decl");
            break;
        case ast_func_decl:
            fprintf(file, "ast_func_decl");
            break;
        case ast_func_header:
            fprintf(file, "ast_func_header");
            break;
        case ast_command_list:
            fprintf(file, "ast_command_list");
            break;
        case ast_assignment:
            fprintf(file, "ast_assignment");
            break;
        case ast_call:
            fprintf(file, "ast_call");
            break;
        case ast_return:
            fprintf(file, "ast_return");
            break;
        case ast_if:
            fprintf(file, "ast_if");
            break;
        case ast_while:
            fprintf(file, "ast_while");
            break;
        case ast_expr_or:
            fprintf(file, "ast_expr_or");
            break;
        case ast_expr_and:
            fprintf(file, "ast_expr_and");
            break;
        case ast_expr_eq:
            fprintf(file, "ast_expr_eq");
            break;
        case ast_expr_ne:
            fprintf(file, "ast_expr_ne");
            break;
        case ast_expr_lt:
            fprintf(file, "ast_expr_lt");
            break;
        case ast_expr_gt:
            fprintf(file, "ast_expr_gt");
            break;
        case ast_expr_le:
            fprintf(file, "ast_expr_le");
            break;
        case ast_expr_ge:
            fprintf(file, "ast_expr_ge");
            break;
        case ast_expr_add:
            fprintf(file, "ast_expr_add");
            break;
        case ast_expr_sub:
            fprintf(file, "ast_expr_sub");
            break;
        case ast_expr_mul:
            fprintf(file, "ast_expr_mul");
            break;
        case ast_expr_div:
            fprintf(file, "ast_expr_div");
            break;
        case ast_expr_mod:
            fprintf(file, "ast_expr_mod");
            break;
        case ast_expr_inv:
            fprintf(file, "ast_expr_inv");
            break;
        case ast_expr_not:
            fprintf(file, "ast_expr_not");
            break;
        case ast_ident:
            fprintf(file, "ast_val_lit");
            break;
        case ast_val_int:
            fprintf(file, "ast_val_int");
            break;
        case ast_val_float:
            fprintf(file, "ast_val_float");
            break;
        case ast_val_bool:
            fprintf(file, "ast_val_bool");
            break;
    }
}

void print_lexeme(FILE *file, lexeme_t *lexeme) {
        switch (lexeme->lex_ident_t.type) {
            case lex_ident:
                fprintf(file, "\"%s\"", lexeme->lex_ident_t.value);
                break;
            case lex_int:
                fprintf(file, "%ld", lexeme->lex_int_t.value);
                break;
            case lex_float:
                fprintf(file, "%f", lexeme->lex_float_t.value);
                break;
            case lex_bool:
                fprintf(file, "%s", lexeme->lex_bool_t.value == 0 ? "false" : "true");
                break;
        }
}

void print_nature(FILE *file, nature_t nature) {
    switch (nature) {
        case nat_literal:
            fprintf(file, "nat_literal");
            break;
        case nat_function:
            fprintf(file, "nat_function");
            break;
        case nat_identifier:
            fprintf(file, "nat_identifier");
            break;
    }
}

void print_scope(FILE *file, scope_t *scope) {
    fprintf(file, "[Scope]\n");
    list_iterate(scope->entries, i) {
        name_entry_t *entry = list_get_as(scope->entries, i, name_entry_t);
        fprintf(file, "- \"%s\" (", entry->lexeme->lex_ident_t.value);
        print_nature(file, entry->nature);
        fprintf(file, ") [");
        print_type(file, entry->type);
        fprintf(file, "] [offset = %ld, line = %ld, column = %ld]\n", entry->offset, entry->line, entry->column);
    }
}

