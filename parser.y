/* Porto Alegre, Novembro de 2023
 * INF01147 - Compiladores
 *
 * Grupo B
 * Felipe Souza Didio - 00323392
 * Pedro Company Beck - 00324055
 *
 */

%define parse.error verbose

// Included in parser.tab.h
%code requires { 
#include "ast.h"
}

// Included in parser.tab.c
%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast.h"
#include "lexeme.h"
#include "semantics.h"

int yylex(void);
void yyerror (char const *mensagem);

extern int get_line_number(void);
extern int get_col_number(void);
extern void *arvore;

ast_type_t current_type;
sym_tab_t *current_scope = NULL;

int delayed_param_error = 0;
identifier_t *error_identifier;

#define handle_bin_op(bin_op_type, self, left, right) self = ast_expression_new_bin_op(ast_bin_op_new(bin_op_type, left, right));

%}

%start programa

%union {
    lexeme_t lexeme;
    program_t *program_t;
    global_t *global_t;
    function_t *function_t;
    variable_t *variable_t;
    variable_names_t *variable_names_t;
    parameters_t *parameters_t;
    command_t *command_t;
    block_t *block_t;
    attribution_t *attribution_t;
    call_t *call_t;
    arguments_t *arguments_t;
    return_t *return_t;
    if_t *if_t;
    while_t *while_t;
    expression_t *expression_t;
    literal_t *literal_t;
    identifier_t *identifier_t;
    ast_type_t ast_type_t;
}

%token TK_PR_INT
%token TK_PR_FLOAT
%token TK_PR_BOOL
%token TK_PR_IF
%token TK_PR_ELSE
%token TK_PR_WHILE
%token TK_PR_RETURN
%token TK_OC_LE
%token TK_OC_GE
%token TK_OC_EQ
%token TK_OC_NE
%token TK_OC_AND
%token TK_OC_OR
%token<lexeme> TK_IDENTIFICADOR
%token<lexeme> TK_LIT_INT
%token<lexeme> TK_LIT_FLOAT
%token<lexeme> TK_LIT_FALSE
%token<lexeme> TK_LIT_TRUE
%token TK_ERRO

%type<program_t>        programa
%type<program_t>        program
%type<global_t>         global
%type<function_t>       function
%type<function_t>       function_header
%type<variable_t>       gvariable
%type<variable_names_t> gvariable_names
%type<variable_t>       lvariable
%type<variable_names_t> lvariable_names
%type<parameters_t>     function_parameters
%type<parameters_t>     parameter_list

%type<command_t>        command
%type<block_t>          block
%type<block_t>          block_body
%type<attribution_t>    attribution
%type<call_t>           call
%type<arguments_t>      arguments
%type<arguments_t>      argument_list
%type<return_t>         return_
%type<if_t>             if_
%type<while_t>          while_

%type<expression_t>     expression 
%type<expression_t>     expr_1 
%type<expression_t>     expr_2
%type<expression_t>     expr_3 
%type<expression_t>     expr_4 
%type<expression_t>     expr_5 
%type<expression_t>     expr_6 
%type<expression_t>     expr_7 
%type<expression_t>     expr_v 

%type<literal_t>        literal
%type<identifier_t>     identifier

%type<ast_type_t>       type

%%
open_block: %empty { current_scope = sym_tab_push(current_scope); };
close_block: %empty { current_scope = sym_tab_pop(current_scope); };

programa: open_block program close_block { $$ = $2; arvore = $$; };
programa: open_block close_block { };
program: global { $$ = ast_program_new(); ast_program_add_child($$, $1); };
program: program global { $$ = $1; ast_program_add_child($$, $2); };

global: gvariable ';' { $$ = ast_global_new_variable($1); };
global: function { $$ = ast_global_new_function($1); };

gvariable: type gvariable_names { $$ = ast_variable_new($1, $2); };
gvariable_names: identifier {
    $$ = ast_variable_names_new();
    ast_variable_names_add_child($$, $1);
    register_symbol_global_variable(current_scope, current_scope, $1, sem_nature_id, current_type);
};
gvariable_names: gvariable_names ',' identifier {
    $$ = $1;
    ast_variable_names_add_child($$, $3);
    register_symbol_global_variable(current_scope, current_scope, $3, sem_nature_id, current_type);
};

lvariable: type lvariable_names { $$ = ast_variable_new($1, $2); };
lvariable_names: identifier {
    $$ = ast_variable_names_new();
    ast_variable_names_add_child($$, $1);
    register_symbol_local_variable(current_scope, current_scope, $1, sem_nature_id, current_type);
};
lvariable_names: lvariable_names ',' identifier {
    $$ = $1;
    ast_variable_names_add_child($$, $3);
    register_symbol_local_variable(current_scope, current_scope, $3, sem_nature_id, current_type);
};

function: function_header '{' block_body '}' close_block { $$ = $1; $$->body = $3; };
function_header: open_block '(' function_parameters ')' TK_OC_GE type '!' identifier {
    if (delayed_param_error) {
        printf("Erro semantico: variavel \"%s\" foi declarada multiplas vezes.\n", error_identifier->text);
        printf("- Contexto: linha %lld, coluna %lld\n", error_identifier->lexeme.line, error_identifier->lexeme.column);
        printf("- Contexto: na declaracao da funcao \"%s\"\n", $8->text);
        exit(ERR_DECLARED);
    }

    $$ = ast_function_new($8, $6, $3, NULL);
    register_symbol_function(current_scope, current_scope->parent, $8, sem_nature_func, $6);
};
function_parameters: %empty { $$ = ast_parameters_new(); };
function_parameters: parameter_list { $$ = $1; };
parameter_list: type identifier {
    $$ = ast_parameters_new();
    variable_names_t *temp = ast_variable_names_new();
    ast_variable_names_add_child(temp, $2);
    ast_parameters_add_child($$, ast_variable_new($1, temp));
    register_symbol_parameter(current_scope, current_scope, $2, sem_nature_id, $1, &delayed_param_error, &error_identifier);
};
parameter_list: parameter_list ',' type identifier { 
    $$ = $1;
    variable_names_t *temp = ast_variable_names_new();
    ast_variable_names_add_child(temp, $4);
    ast_parameters_add_child($$, ast_variable_new($3, temp));
    register_symbol_parameter(current_scope, current_scope, $4, sem_nature_id, $3, &delayed_param_error, &error_identifier);
};

block: open_block '{' block_body '}' close_block { $$ = $3; };
block_body: %empty { $$ = ast_block_new(); };
block_body: block_body command ';' { $$ = $1; ast_block_add_child($$, $2); };

command: lvariable { $$ = ast_command_new_variable($1); };
command: attribution { $$ = ast_command_new_attribution($1); };
command: call { $$ = ast_command_new_call($1); };
command: return_ { $$ = ast_command_new_return($1); };
command: if_ { $$ = ast_command_new_if($1); };
command: while_ { $$ = ast_command_new_while($1); };
command: block { $$ = ast_command_new_block($1); };

attribution: identifier '=' expression {
    $$ = ast_attribution_new($1, $3);

    sym_val_t *val = sym_tab_find(current_scope, $1->text);
    //sym_value_print(val);
    if (val == NULL) {
        printf("Erro semantico: variavel \"%s\" foi utilizada antes de ser declarada.\n", $1->text);
        printf("- Contexto: attribuicao na linha %lld, coluna %lld\n", $1->lexeme.line, $1->lexeme.column);
        sym_tab_t *global_scope = current_scope;
        while (global_scope->parent != NULL) {
            global_scope = global_scope->parent;
        }
        printf("- Contexto: dentro da funcao \"%s\"\n", global_scope->list->key);
        exit(ERR_UNDECLARED);
    }
    if (val->nature == sem_nature_func) {
        exit(ERR_FUNCTION);
    }
}
call: identifier '(' arguments ')' {
    $$ = ast_call_new($1, $3);
    sym_val_t *val = sym_tab_find(current_scope, $1->text);
    if (val == NULL) {
        printf("Erro semantico: variavel \"%s\" foi utilizada antes de ser declarada.\n", $1->text);
        printf("- Contexto: chamada de funcao na linha %lld, coluna %lld\n", $1->lexeme.line, $1->lexeme.column);
        sym_tab_t *global_scope = current_scope;
        while (global_scope->parent != NULL) {
            global_scope = global_scope->parent;
        }
        printf("- Contexto: dentro da funcao \"%s\"\n", global_scope->list->key);
        exit(ERR_UNDECLARED);
    }
    if (val->nature == sem_nature_id) {
        printf("Erro semantico: variavel \"%s\" foi utilizada como funcao.\n", $1->text);
        printf("- Contexto: linha %lld, coluna %lld\n", $1->lexeme.line, $1->lexeme.column);
        sym_tab_t *global_scope = current_scope;
        while (global_scope->parent != NULL) {
            global_scope = global_scope->parent;
        }
        printf("- Contexto: dentro da funcao \"%s\"\n", global_scope->list->key);
        exit(ERR_VARIABLE);
    }
    $$->type = val->type;

    // Useful debugging tool!
    /*
    if (strcmp($1->text, "print_type") == 0) {
        if ($3->len == 1) {
            fprintf(stderr, "The type of `");
            ast_expression_print($3->arguments[0]);
            fprintf(stderr, "` is `%s`\n", ast_type_to_string($3->arguments[0]->type));
        }
    }
    */
};
arguments: %empty { $$ = ast_arguments_new(); };
arguments: argument_list { $$ = $1; };
argument_list: expression { $$ = ast_arguments_new(); ast_arguments_add_child($$, $1); };
argument_list: argument_list ',' expression { $$ = $1; ast_arguments_add_child($$, $3); };
return_: TK_PR_RETURN expression { $$ = ast_return_new($2); };
if_: TK_PR_IF '(' expression ')' block TK_PR_ELSE block { $$ = ast_if_new($3, $5, $7); };
if_: TK_PR_IF '(' expression ')' block                  { $$ = ast_if_new($3, $5, ast_block_new()); };
while_: TK_PR_WHILE '(' expression ')' block { $$ = ast_while_new($3, $5); };

expression: expr_7 { $$ = $1; };
expr_1: expr_v { $$ = $1; };
expr_1: '-' expr_1 { $$ = ast_expression_new_un_op(ast_un_op_new(op_inv, $2)); };
expr_1: '!' expr_1 { $$ = ast_expression_new_un_op(ast_un_op_new(op_not, $2)); };
expr_2: expr_1 { $$ = $1; };
expr_2: expr_2 '*' expr_1 { handle_bin_op(op_mul, $$, $1, $3); };
expr_2: expr_2 '/' expr_1 { handle_bin_op(op_div, $$, $1, $3); };
expr_2: expr_2 '%' expr_1 { handle_bin_op(op_mod, $$, $1, $3); };
expr_3: expr_2 { $$ = $1; };
expr_3: expr_3 '+' expr_2 { handle_bin_op(op_add, $$, $1, $3); };
expr_3: expr_3 '-' expr_2 { handle_bin_op(op_sub, $$, $1, $3); };
expr_4: expr_3 { $$ = $1; };
expr_4: expr_4 '<' expr_3 { handle_bin_op(op_lt, $$, $1, $3); };
expr_4: expr_4 '>' expr_3 { handle_bin_op(op_gt, $$, $1, $3); };
expr_4: expr_4 TK_OC_LE expr_3 { handle_bin_op(op_le, $$, $1, $3); };
expr_4: expr_4 TK_OC_GE expr_3 { handle_bin_op(op_ge, $$, $1, $3); };
expr_5: expr_4 { $$ = $1; };
expr_5: expr_5 TK_OC_EQ expr_4 { handle_bin_op(op_eq, $$, $1, $3); };
expr_5: expr_5 TK_OC_NE expr_4 { handle_bin_op(op_ne, $$, $1, $3); };
expr_6: expr_5 { $$ = $1; };
expr_6: expr_6 TK_OC_AND expr_5 { handle_bin_op(op_and, $$, $1, $3); };
expr_7: expr_6 { $$ = $1; };
expr_7: expr_7 TK_OC_OR expr_6 { handle_bin_op(op_or, $$, $1, $3); };
expr_v: '(' expression ')' { $$ = $2; };
expr_v: literal { $$ = ast_expression_new_literal($1); };
expr_v: identifier {
    $$ = ast_expression_new_identifier($1);

    sym_val_t *val = sym_tab_find(current_scope, $1->text);
    if (val == NULL) {
        printf("Erro semantico: variavel \"%s\" foi utilizada antes de ser declarada.\n", $1->text);
        printf("- Contexto: expressao na linha %lld, coluna %lld\n", $1->lexeme.line, $1->lexeme.column);
        sym_tab_t *global_scope = current_scope;
        while (global_scope->parent != NULL) {
            global_scope = global_scope->parent;
        }
        printf("- Contexto: dentro da funcao \"%s\"\n", global_scope->list->key);
        exit(ERR_UNDECLARED);
    }
    if (val->nature == sem_nature_func) {
        printf("Erro semantico: funcao \"%s\" foi utilizada como variavel.\n", $1->text);
        printf("- Contexto: expressao na linha %lld, coluna %lld\n", $1->lexeme.line, $1->lexeme.column);
        sym_tab_t *global_scope = current_scope;
        while (global_scope->parent != NULL) {
            global_scope = global_scope->parent;
        }
        printf("- Contexto: dentro da funcao \"%s\"\n", global_scope->list->key);
        exit(ERR_FUNCTION);
    }
    $$->type = val->type;
}
expr_v: call { $$ = ast_expression_new_call($1); };

type: TK_PR_INT { $$ = ast_int; current_type = $$; };
type: TK_PR_FLOAT { $$ = ast_float; current_type = $$; };
type: TK_PR_BOOL { $$ = ast_bool; current_type = $$; };

identifier: TK_IDENTIFICADOR { $$ = ast_identifier_new($1.value.ident, $1); };

literal: TK_LIT_INT { $$ = ast_literal_new_int(ast_int_new($1.value.lit_int, $1)); };
literal: TK_LIT_FLOAT { $$ = ast_literal_new_float(ast_float_new($1.value.lit_float, $1)); };
literal: TK_LIT_TRUE { $$ = ast_literal_new_bool(ast_bool_new_true($1)); };
literal: TK_LIT_FALSE { $$ = ast_literal_new_bool(ast_bool_new_false($1)); };

%%

void yyerror (char const *mensagem) {
    // A seguinte linha de código demontra como apresentar a coluna do erro, 
    // functionalidade que nao foi habilitada para seguir a especificacao com mais regor:
    // printf("Erro na linha %d, coluna %d: \n - %s\n", get_line_number(), get_col_number(), mensagem);
    printf("Erro na linha %d: \n - %s\n", get_line_number(), mensagem);
}

