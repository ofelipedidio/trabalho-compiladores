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
#include "code_gen5.h"
}

// Included in parser.tab.c
%{
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>

#include "ast.h"

int yylex(void);
void yyerror (char const *mensagem);

extern int get_line_number(void);
extern int get_col_number(void);
extern void *arvore;

%}

%start programa

%union {
    lexeme_t lexeme;
    program_t *program_t;
    list_t list_t;
    type_t type_t;
}

%type<program_t>      program
%type<program_t>      global_list
%type<list_t>         comma_separated_variables_0
%type<list_t>         comma_separated_variables_1
%type<list_t>         comma_separated_expressions_0
%type<list_t>         comma_separated_expressions_1
%type<list_t>         comma_separated_identifiers_0
%type<list_t>         comma_separated_identifiers_1
%type<variable_t>     variable
%type<program_t>      command_list
%type<program_t>      command
%type<program_t>      command
%type<program_t>      expression
%type<program_t>      expr_7
%type<program_t>      expr_6
%type<program_t>      expr_5
%type<program_t>      expr_4
%type<program_t>      expr_3
%type<program_t>      expr_2
%type<program_t>      expr_1
%type<program_t>      expr_v
%type<type_t>         type


%%
/* Scope control */
push_scope: %empty;
pop_scope: %empty;

/* Program */
program: push_scope global_list pop_scope;

/* Empty program */
global_list: %empty;

/* Global variable */
global_list: global_list type comma_separated_identifiers ';';

/* Global function */
global_list: global_list push_scope function_header '{' command_list '}' pop_scope;
function_header: '(' comma_separated_variables ')' TK_OC_GE type '!' TK_IDENTIFICADOR;

/* Comma-separated variables */
comma_separated_variables_0: %empty { $$ = empty_list(); };
comma_separated_variables_0: comma_separated_variables_1 { $$ = $1; };
comma_separated_variables_1: variable { $$ = empty_list(); list_push($$, $1); };
comma_separated_variables_1: comma_separated_variables_1 ',' variable { $$ = $1; list_push($$, $3); };

/* Comma-separated expressions */
comma_separated_expressions_0: %empty { $$ = empty_list(); };
comma_separated_expressions_0: comma_separated_expressions_1 { $$ = $1; };
comma_separated_expressions_1: expression { $$ = empty_list(); list_push($$, $1); };
comma_separated_expressions_1: comma_separated_expressions_1 ',' expression { $$ = $1; list_push($$, $3); };

/* Comma-separated identifiers */
comma_separated_identifiers_0: %empty { $$ = empty_list(); };
comma_separated_identifiers_0: comma_separated_identifiers_1 { $$ = $1; };
comma_separated_identifiers_1: TK_IDENTIFICADOR { $$ = empty_list(); list_push($$, $1); };
comma_separated_identifiers_1: comma_separated_identifiers_1 ',' TK_IDENTIFICADOR { $$ = $1; list_push($$, $3); };

/* Variable definition */
variable: type comma_separated_identifiers_1;

/* Command list */
command_list: %empty { $$ = empty_program(); };
command_list: command_list command { $$ = $1; program_append($$, $2); };

/* Commands */
command: variable ';';
command: TK_IDENTIFICADOR '=' expression ';';
command: TK_IDENTIFICADOR '(' arguments ')' ';';
command: TK_PR_RETURN expression ';';
command: TK_PR_IF '(' expression ')' '{' block_body '}' TK_PR_ELSE '{' block_body '}' ';';
command: TK_PR_IF '(' expression ')' '{' block_body '}' ';';
command: TK_PR_WHILE '(' expression ')' '{' block_body '}' ';';
command: open_scope '{' block_body '}' close_scope ';';

/* Compound expressions */
expression: expr_7;
expr_7: expr_6;
expr_7: expr_7 TK_OC_OR expr_6;
expr_6: expr_5;
expr_6: expr_6 TK_OC_AND expr_5;
expr_5: expr_4;
expr_5: expr_5 TK_OC_EQ expr_4;
expr_5: expr_5 TK_OC_NE expr_4;
expr_4: expr_3;
expr_4: expr_4 '<' expr_3;
expr_4: expr_4 '>' expr_3;
expr_4: expr_4 TK_OC_LE expr_3;
expr_4: expr_4 TK_OC_GE expr_3;
expr_3: expr_2;
expr_3: expr_3 '+' expr_2;
expr_3: expr_3 '-' expr_2;
expr_2: expr_1;
expr_2: expr_2 '*' expr_1;
expr_2: expr_2 '/' expr_1;
expr_2: expr_2 '%' expr_1;
expr_1: expr_v;
expr_1: '-' expr_1;
expr_1: '!' expr_1;

/* Value expressions */
expr_v: '(' expression ')';
expr_v: TK_LIT_INT;
expr_v: TK_LIT_FLOAT;
expr_v: TK_LIT_TRUE;
expr_v: TK_LIT_FALSE;
expr_v: TK_IDENTIFICADOR;
expr_v: TK_IDENTIFICADOR '(' arguments ')';

/* Types */
type: TK_PR_INT { $$ = type_int; };
type: TK_PR_FLOAT { $$ = type_float; };
type: TK_PR_BOOL { $$ = type_bool; };

%%

void yyerror (char const *mensagem) {
    // A seguinte linha de código demontra como apresentar a coluna do erro, 
    // functionalidade que nao foi habilitada para seguir a especificacao com mais regor:
    // printf("Erro na linha %d, coluna %d: \n - %s\n", get_line_number(), get_col_number(), mensagem);
    printf("Erro na linha %d: \n - %s\n", get_line_number(), mensagem);
}

