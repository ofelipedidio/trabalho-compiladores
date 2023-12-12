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
#include "lexeme.h"
}

// Included in parser.tab.c
%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast.h"
#include "lexeme.h"
#include "symbols.h"
#include "semantics.h"

#define ERR_UNDECLARED 10 //2.2
#define ERR_DECLARED 11 //2.2
#define ERR_VARIABLE 20 //2.3
#define ERR_FUNCTION 21 //2.3

int yylex(void);
void yyerror (char const *mensagem);

extern int get_line_number(void);
extern int get_col_number(void);
extern void *arvore;

sym_tab_t *current_scope = NULL;
sem_type_t current_type;
sym_list_t *temp_node;

int id_line;
int id_column;

/*
void register_symbol(ast_t *ast, sym_nature_t nature) {
    sym_val_t temp_val;
    fprintf(stderr, "got symbol \"%s\"\n", ast->body.leaf.value.ident);
    if ((temp_node = sym_tab_find(current_scope, ast->body.leaf.value.ident)) != NULL) {
        // TODO - Didio: write error message
        fprintf(stderr, "ERROR: symbol \"%s\" already exists!\n", temp_node->key);
        exit(ERR_DECLARED);
    }
    temp_val.line = id_line;
    temp_val.column = id_column;
    temp_val.nature = nature;
    temp_val.type = current_type;
    temp_val.lex = lex_clone(ast->body.leaf);
    current_scope->list = sym_list_insert(
        current_scope->list,
        ast->body.leaf.value.ident,
        temp_val
    ); 
    sym_tab_print(current_scope);
    fprintf(stderr, "\n");
}
*/

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
%type<variable_t>       variable
%type<variable_names_t> variable_names
%type<variable_t>       single_variable
%type<parameters_t>     function_parameters
%type<parameters_t>     parameter_list

%type<ast_type_t>       type

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

%%
open_block: %empty { current_scope = sym_tab_push(current_scope); };
close_block: %empty { current_scope = sym_tab_pop(current_scope); };

programa: open_block program close_block { $$ = $2; arvore = $$; };
programa: open_block close_block { };

program: global { $$ = ast_program_new(); ast_program_add_child($$, $1); };
program: program global { $$ = $1; ast_program_add_child($$, $2); };

global: variable ';' { $$ = ast_global_new_variable($1); };
global: function { $$ = ast_global_new_function($1); };

single_variable: type identifier { variable_names_t *temp = ast_variable_names_new(); ast_variable_names_add_child(temp, $2); $$ = ast_variable_new($1, temp); };
variable: type variable_names { $$ = ast_variable_new($1, $2); };
variable_names: identifier { $$ = ast_variable_names_new(); ast_variable_names_add_child($$, $1); };
variable_names: variable_names ',' identifier { $$ = $1; ast_variable_names_add_child($$, $3); };

function_header: '(' function_parameters ')' TK_OC_GE type '!' identifier { $$ = ast_function_new($7, $5, $2, NULL); };
function: function_header block { $$ = $1; $$->body = $2; };

function_parameters: %empty { $$ = ast_parameters_new(); };
function_parameters: parameter_list { $$ = $1; };

parameter_list: single_variable { $$ = ast_parameters_new(); ast_parameters_add_child($$, $1); };
parameter_list: parameter_list ',' single_variable { $$ = $1; ast_parameters_add_child($$, $3); };

block: open_block '{' block_body '}' close_block { $$ = $3; };
block_body: %empty { $$ = ast_block_new(); };
block_body: block_body command ';' { $$ = $1; ast_block_add_child($$, $2); };

command: variable { $$ = ast_command_new_variable($1); };
command: attribution { $$ = ast_command_new_attribution($1); };
command: call { $$ = ast_command_new_call($1); };
command: return_ { $$ = ast_command_new_return($1); };
command: if_ { $$ = ast_command_new_if($1); };
command: while_ { $$ = ast_command_new_while($1); };
command: block { $$ = ast_command_new_block($1); };

attribution: identifier '=' expression { $$ = ast_attribution_new($1, $3); }

call: identifier '(' arguments ')' { $$ = ast_call_new($1, $3); };
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
expr_v: identifier { $$ = ast_expression_new_identifier($1); }
expr_v: call { $$ = ast_expression_new_call($1); };

type: TK_PR_INT { current_type = sem_type_int; };
type: TK_PR_FLOAT { current_type = sem_type_float; };
type: TK_PR_BOOL { current_type = sem_type_bool; };

identifier: TK_IDENTIFICADOR { 
            $$ = ast_identifier_new($1.value.ident, $1); 
            id_line = get_line_number();
            id_column = get_col_number();
          };

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

