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

extern int get_line_number(void);
extern int get_col_number(void);

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
%token<ident> TK_IDENTIFICADOR
%token<int> TK_LIT_INT
%token<float> TK_LIT_FLOAT
%token<int> TK_LIT_FALSE
%token<int> TK_LIT_TRUE
%token TK_ERRO

%

%start programa

%%

programa: global_definition { };
programa: { };

global_definition: global_element { };
global_definition: global_definition global_element { };

global_element: global_variable_definition { };
global_element: global_function_definition { };

global_variable_definition: type global_variable_definition_names ';' { };

global_variable_definition_names: TK_IDENTIFICADOR { };
global_variable_definition_names: global_variable_definition_names ',' TK_IDENTIFICADOR { };

global_function_definition: '(' parameter_list_definition ')' TK_OC_GE type '!' TK_IDENTIFICADOR block { };
global_function_definition: '('                           ')' TK_OC_GE type '!' TK_IDENTIFICADOR block { };

parameter_list_definition: parameter_definition { };
parameter_list_definition: parameter_list_definition ',' parameter_definition { };

parameter_definition: type TK_IDENTIFICADOR { };

block: '{' block_body '}' { };
block: '{'            '}' { };

block_body: command ';' { };
block_body: block_body command ';' { };

command: variable_declaration { };
command: variable_attribution { };
command: function_call { };
command: return_statement { };
command: if_statement { };
command: while_statement { };
command: block { };

variable_declaration: type variable_names { };

variable_names: TK_IDENTIFICADOR { };
variable_names: variable_names ',' TK_IDENTIFICADOR { };

variable_attribution: TK_IDENTIFICADOR '=' expression { };

function_call: TK_IDENTIFICADOR '(' parameter_list ')' { };
function_call: TK_IDENTIFICADOR '('                ')' { };

parameter_list: expression { };
parameter_list: parameter_list ',' expression { };

return_statement: TK_PR_RETURN expression { };

if_statement: TK_PR_IF '(' expression ')' block { };
if_statement: TK_PR_IF '(' expression ')' block TK_PR_ELSE block { };

while_statement: TK_PR_WHILE '(' expression ')' block { };

expression: expr_1 { };

expr_1: expr_2 { };
expr_1: '-' expr_1 { };
expr_1: '!' expr_1 { };

expr_2: expr_3 { };
expr_2: expr_2 '*' expr_3 { };
expr_2: expr_2 '/' expr_3 { };
expr_2: expr_2 '%' expr_3 { };

expr_3: expr_4 { };
expr_3: expr_3 '+' expr_4 { };
expr_3: expr_3 '-' expr_4 { };

expr_4: expr_5 { };
expr_4: expr_4 '<' expr_5 { };
expr_4: expr_4 '>' expr_5 { };
expr_4: expr_4 TK_OC_LE expr_5 { };
expr_4: expr_4 TK_OC_GE expr_5 { };

expr_5: expr_6 { };
expr_5: expr_5 TK_OC_EQ expr_6 { };
expr_5: expr_5 TK_OC_NE expr_6 { };

expr_6: expr_7 { };
expr_6: expr_6 TK_OC_AND expr_7 { };

expr_7: expr_8 { };
expr_7: expr_7 TK_OC_OR expr_8 { };

expr_8: '(' expression ')' { };
expr_8: TK_IDENTIFICADOR { };
expr_8: literal { };
expr_8: function_call { };

type: TK_PR_INT { };
type: TK_PR_FLOAT { };
type: TK_PR_BOOL { };

literal: TK_LIT_INT { fprintf(stderr, "INT_LITERAL\n"); printf("%d\n", yylval); };
literal: TK_LIT_FLOAT { fprintf(stderr, "FLOAT_LITERAL\n"); };
literal: TK_LIT_TRUE { fprintf(stderr, "TRUE_LITERAL\n"); };
literal: TK_LIT_FALSE { fprintf(stderr, "FALSE_LITERAL\n"); };

%%

void yyerror (char const *mensagem) {
    // A seguinte linha de código demontra como apresentar a coluna do erro, 
    // functionalidade que nao foi habilitada para seguir a especificacao com mais regor:
    // printf("Erro na linha %d, coluna %d: \n - %s\n", get_line_number(), get_col_number(), mensagem);
    printf("Erro na linha %d: \n - %s\n", get_line_number(), mensagem);
}

