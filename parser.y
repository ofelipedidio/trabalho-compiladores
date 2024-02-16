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
#include "list.h"
#include "structs.h"
}

// Included in parser.tab.c
%{
#include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
#include "code_gen.h"

int yylex(void);
void yyerror (char const *mensagem);

extern int get_line_number(void);
extern void *arvore;

%}

%start program

%union {
    ast_t *ast_t;
    list_t *list_t;
    type_t type_t;
    lexeme_t *lexeme_t;
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
%token<lexeme_t> TK_IDENTIFICADOR
%token<lexeme_t> TK_LIT_INT
%token<lexeme_t> TK_LIT_FLOAT
%token<lexeme_t> TK_LIT_FALSE
%token<lexeme_t> TK_LIT_TRUE
%token TK_ERRO

%type<ast_t>          program
%type<ast_t>          global_list
%type<ast_t>          function_header
%type<list_t>         comma_separated_variables_0                        
%type<list_t>         comma_separated_variables_1
%type<list_t>         comma_separated_expressions_0
%type<list_t>         comma_separated_expressions_1
/* %type<list_t>         comma_separated_identifiers_0 */
%type<list_t>         comma_separated_identifiers_1
%type<ast_t>          variable                        
%type<ast_t>          command_list
%type<ast_t>          expression
%type<ast_t>          expr_7
%type<ast_t>          expr_6
%type<ast_t>          expr_5
%type<ast_t>          expr_4
%type<ast_t>          expr_3
%type<ast_t>          expr_2
%type<ast_t>          expr_1
%type<ast_t>          expr_v
%type<type_t>         type

%%
/* Scope control */
push_scope: %empty 
    { reduce_push_scope(); };
pop_scope: %empty 
    { reduce_pop_scope(); };

/* Program */
program: push_scope global_list pop_scope 
    { $$ = reduce_program($2); arvore = $$; };

/* Empty program */
global_list: %empty 
    { $$ = reduce_global_list_empty(); };

/* Global variable */
global_list: global_list type comma_separated_identifiers_1 ';' 
    { $$ = reduce_global_list_variable($1, $2, $3); };

/* Global function */
global_list: global_list function_header '{' command_list '}' 
    { $$ = reduce_global_list_function($1, $2, $4); };
function_header: '(' comma_separated_variables_0 ')' TK_OC_GE type '!' TK_IDENTIFICADOR 
    { $$ = reduce_function_header($2, $5, $7); };

/* Comma-separated variables */
comma_separated_variables_0: %empty 
    { $$ = empty_list(); };
comma_separated_variables_0: comma_separated_variables_1 
    { $$ = $1; };
comma_separated_variables_1: variable 
    { $$ = empty_list(); list_push($$, $1); };
comma_separated_variables_1: comma_separated_variables_1 ',' variable 
    { $$ = $1; list_push($$, $3); };

/* Comma-separated expressions */
comma_separated_expressions_0: %empty 
    { $$ = empty_list(); };
comma_separated_expressions_0: comma_separated_expressions_1 
    { $$ = $1; };
comma_separated_expressions_1: expression 
    { $$ = empty_list(); list_push($$, $1); };
comma_separated_expressions_1: comma_separated_expressions_1 ',' expression 
    { $$ = $1; list_push($$, $3); };

/* Comma-separated identifiers */
/* comma_separated_identifiers_0: %empty                                     */
/*     { $$ = empty_list(); };                                               */
/* comma_separated_identifiers_0: comma_separated_identifiers_1              */
/*     { $$ = $1; };                                                         */
comma_separated_identifiers_1: TK_IDENTIFICADOR
    { $$ = empty_list(); list_push($$, $1); };
comma_separated_identifiers_1: comma_separated_identifiers_1 ',' TK_IDENTIFICADOR 
    { $$ = $1; list_push($$, $3); };

/* Variable definition */
variable: type TK_IDENTIFICADOR 
    { $$ = reduce_variable($1, $2); };

/* Commands */
command_list: %empty 
    { $$ = reduce_command_empty(); };
command_list: command_list type comma_separated_identifiers_1 ';' 
    { $$ = reduce_command_variable($1, $2, $3); };
command_list: command_list TK_IDENTIFICADOR '=' expression ';' 
    { $$ = reduce_command_assignment($1, $2, $4); };
command_list: command_list TK_IDENTIFICADOR '(' comma_separated_expressions_0 ')' ';' 
    { $$ = reduce_command_call($1, $2, $4); };
command_list: command_list TK_PR_RETURN expression ';' 
    { $$ = reduce_command_return($1, $3); };
command_list: command_list TK_PR_IF '(' expression ')' '{' command_list '}' TK_PR_ELSE '{' command_list '}' ';' 
    { $$ = reduce_command_if_else($1, $4, $7, $11); };
command_list: command_list TK_PR_IF '(' expression ')' '{' command_list '}' ';' 
    { $$ = reduce_command_if($1, $4, $7); };
command_list: command_list TK_PR_WHILE '(' expression ')' '{' command_list '}' ';' 
    { $$ = reduce_command_while($1, $4, $7); };
command_list: command_list push_scope '{' command_list '}' pop_scope ';' 
    { $$ = reduce_command_block($1, $4); };

/* Compound expressions */
expression: expr_7 
    { $$ = $1; };
expr_7: expr_6 
    { $$ = $1; };
expr_7: expr_7 TK_OC_OR expr_6 
    { $$ = reduce_expr_or($1, $3); };
expr_6: expr_5 
    { $$ = $1; };
expr_6: expr_6 TK_OC_AND expr_5 
    { $$ = reduce_expr_and($1, $3); };
expr_5: expr_4 
    { $$ = $1; };
expr_5: expr_5 TK_OC_EQ expr_4 
    { $$ = reduce_expr_eq($1, $3); };
expr_5: expr_5 TK_OC_NE expr_4 
    { $$ = reduce_expr_ne($1, $3); };
expr_4: expr_3 
    { $$ = $1; };
expr_4: expr_4 '<' expr_3 
    { $$ = reduce_expr_lt($1, $3); };
expr_4: expr_4 '>' expr_3 
    { $$ = reduce_expr_gt($1, $3); };
expr_4: expr_4 TK_OC_LE expr_3 
    { $$ = reduce_expr_le($1, $3); };
expr_4: expr_4 TK_OC_GE expr_3 
    { $$ = reduce_expr_ge($1, $3); };
expr_3: expr_2 
    { $$ = $1; };
expr_3: expr_3 '+' expr_2 
    { $$ = reduce_expr_add($1, $3); };
expr_3: expr_3 '-' expr_2 
    { $$ = reduce_expr_sub($1, $3); };
expr_2: expr_1 
    { $$ = $1; };
expr_2: expr_2 '*' expr_1 
    { $$ = reduce_expr_mul($1, $3); };
expr_2: expr_2 '/' expr_1 
    { $$ = reduce_expr_div($1, $3); };
expr_2: expr_2 '%' expr_1 
    { $$ = reduce_expr_mod($1, $3); };
expr_1: expr_v 
    { $$ = $1; };
expr_1: '-' expr_1 
    { $$ = reduce_expr_inv($2); };
expr_1: '!' expr_1 
    { $$ = reduce_expr_not($2); };

/* Value expressions */
expr_v: '(' expression ')' 
    { $$ = $2; };
expr_v: TK_LIT_INT 
    { $$ = reduce_expr_int($1); };
expr_v: TK_LIT_FLOAT 
    { $$ = reduce_expr_float($1); };
expr_v: TK_LIT_TRUE 
    { $$ = reduce_expr_bool($1); };
expr_v: TK_LIT_FALSE 
    { $$ = reduce_expr_bool($1); };
expr_v: TK_IDENTIFICADOR 
    { $$ = reduce_expr_ident($1); };
expr_v: TK_IDENTIFICADOR '(' comma_separated_expressions_0 ')' 
    { $$ = reduce_expr_call($1, $3); };

/* Types */
type: TK_PR_INT { $$ = type_int; };
type: TK_PR_FLOAT { $$ = type_float; };
type: TK_PR_BOOL { $$ = type_bool; };

%%

void yyerror (char const *mensagem) {
    // A seguinte linha de código demontra como apresentar a coluna do erro, 
    // functionalidade que nao foi habilitada para seguir a especificacao com mais regor:
    // printf("Erro na linha %d, coluna %d: \n - %s\n", get_line_number(), get_col_number(), mensagem);
    fprintf(stderr, "Erro na linha %d: \n - %s\n", get_line_number(), mensagem);
}
