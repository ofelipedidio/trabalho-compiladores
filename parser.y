/* Porto Alegre, Novembro de 2023
 * INF01147 - Compiladores
 *
 * Grupo B
 * Felipe Souza Didio - 00323392
 * Pedro Company Beck - 00324055
 *
 */

%define parse.error verbose

%{
#include <stdio.h>
#include <stdlib.h>

int yylex(void);
void yyerror (char const *mensagem);
%}

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
%token TK_IDENTIFICADOR
%token TK_LIT_INT
%token TK_LIT_FLOAT
%token TK_LIT_FALSE
%token TK_LIT_TRUE
%token TK_ERRO

%start programa

%%

programa: global_definition | ;

global_definition: global_element | global_definition global_element

global_element: global_variable_definition | global_function_definition;

global_variable_definition: type global_variable_definition_names ';';

global_variable_definition_names: TK_IDENTIFICADOR | TK_IDENTIFICADOR ',' global_variable_definition_names;

global_function_definition: '(' parameter_list_definition ')' TK_OC_GE type '!' TK_IDENTIFICADOR block;
global_function_definition: '('                           ')' TK_OC_GE type '!' TK_IDENTIFICADOR block;

parameter_list_definition: parameter_definition | parameter_list_definition ',' parameter_definition;

parameter_definition: type TK_IDENTIFICADOR;

block: '{' block_body '}';
block: '{'            '}';

block_body: command ';' | block_body command ';';

command: variable_declaration | variable_attribution | function_call | return_statement | if_statement | while_statement;

variable_declaration: type variable_names;

variable_names: TK_IDENTIFICADOR | variable_names ',' TK_IDENTIFICADOR;

variable_attribution: TK_IDENTIFICADOR '=' expression;

function_call: TK_IDENTIFICADOR '(' parameter_list ')';
function_call: TK_IDENTIFICADOR '('                ')';

parameter_list: expression | parameter_list ',' expression

return_statement: TK_PR_RETURN expression;

if_statement: TK_PR_IF '(' expression ')' block;
if_statement: TK_PR_IF '(' expression ')' block TK_PR_ELSE block;

while_statement: TK_PR_WHILE '(' expression ')' block;

expression: expr_1;

expr_1: expr_2 | '-' expr_1 | '!' expr_1;

expr_2: expr_3 | expr_2 '*' expr_3 | expr_2 '/' expr_3 | expr_2 '%' expr_3;

expr_3: expr_4 | expr_3 '+' expr_4 | expr_3 '-' expr_4;

expr_4: expr_5 | expr_4 '<' expr_5 | expr_4 '>' expr_5 | expr_4 TK_OC_LE expr_5 | expr_4 TK_OC_GE expr_5;

expr_5: expr_6 | expr_5 TK_OC_EQ expr_6 | expr_5 TK_OC_NE expr_6;

expr_6: expr_7 | expr_6 TK_OC_AND expr_7;

expr_7: expr_8 | expr_7 TK_OC_OR expr_8;

expr_8: '(' expression ')' | TK_IDENTIFICADOR | literal | function_call;

type: TK_PR_INT | TK_PR_FLOAT | TK_PR_BOOL;

literal: TK_PR_INT | TK_LIT_FLOAT | TK_LIT_TRUE | TK_LIT_FALSE;

%%

void yyerror (char const *mensagem) {
    printf("%s\n", mensagem);
}

