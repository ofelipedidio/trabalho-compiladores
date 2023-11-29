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
extern void *arvore;

%}

%code requires { 
#include "ast.h"
#include "lexeme.h"
}

%start programa

%union {
    lexeme_t lexeme;
    ast_t *node;
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

%type<node> programa
%type<node> global_definition
%type<node> global_function_definition

%type<node> block 
%type<node> block_body 

%type<node> command 

%type<node> variable_attribution 
%type<node> function_call 
%type<node> parameter_list 
%type<node> return_statement 
%type<node> if_statement 
%type<node> while_statement 

%type<node> expression 
%type<node> expr_1 
%type<node> expr_2
%type<node> expr_3 
%type<node> expr_4 
%type<node> expr_5 
%type<node> expr_6 
%type<node> expr_7 
%type<node> expr_v 

%type<node> literal
%type<node> identifier

%%

programa: global_definition { $$ = $1; arvore = $$; };
programa: { };

global_definition: global_variable_definition { $$ = ast_make_node(noop); };
global_definition: global_function_definition { $$ = $1; };
global_definition: global_definition global_variable_definition { $$ = $1; };
global_definition: global_definition global_function_definition { if ($1 != NULL && $1->type != noop) { ast_append($1, $2); $$ = $1; } else { ast_free($1); $$ = $2; } };

global_variable_definition: type global_variable_definition_names ';' { /* NOOP */ };

global_variable_definition_names: identifier { ast_free($1); /* NOOP */ };
global_variable_definition_names: global_variable_definition_names ',' identifier { ast_free($3); /* NOOP */ };

global_function_definition: '(' parameter_list_definition ')' TK_OC_GE type '!' identifier block { $$ = ast_make_node(func_declaration); ast_append($$, $7); ast_append($$, $8); };
global_function_definition: '('                           ')' TK_OC_GE type '!' identifier block { $$ = ast_make_node(func_declaration); ast_append($$, $6); ast_append($$, $7); };

parameter_list_definition: parameter_definition { /* NOOP */ };
parameter_list_definition: parameter_list_definition ',' parameter_definition { /* NOOP */ };

parameter_definition: type identifier { ast_free($2); /* NOOP */ };

block: '{' block_body '}' { $$ = $2; };
block: '{'            '}' { $$ = ast_make_node(noop); };

block_body: command ';' { $$ = $1; };
block_body: command ';' block_body { if ($1->type != noop) {ast_append($1, $3); $$ = $1;} else {ast_free($1); $$ = $3; } };

command: variable_declaration { $$ = ast_make_node(noop); };
command: variable_attribution { $$ = $1; };
command: function_call { $$ = $1; };
command: return_statement { $$ = $1; };
command: if_statement { $$ = $1; };
command: while_statement { $$ = $1; };
command: block { $$ = $1; };

variable_declaration: type variable_names { /* NOOP */ };

variable_names: identifier { ast_free($1); /* NOOP */ };
variable_names: variable_names ',' identifier { ast_free($3); /* NOOP */ };

variable_attribution: identifier '=' expression { $$ = ast_make_node(statement_attr); ast_append($$, $1); ast_append($$, $3); };

function_call: identifier '(' parameter_list ')' { $$ = ast_make_node(statement_call); ast_append($$, $1); ast_append($$, $3); };
function_call: identifier '('                ')' { $$ = ast_make_node(statement_call); ast_append($$, $1); ast_append($$, ast_make_node(noop)); };

parameter_list: expression { $$ = ast_make_node(call_argument); ast_append($$, $1); };
parameter_list: expression ',' parameter_list  { $$ = ast_make_node(call_argument); ast_append($$, $1); ast_append($$, $3); };

return_statement: TK_PR_RETURN expression { $$ = ast_make_node(statement_return); ast_append($$, $2); };

if_statement: TK_PR_IF '(' expression ')' block TK_PR_ELSE block { $$ = ast_make_node(statement_if); ast_append($$, $3); ast_append($$, $5); ast_append($$, $7); };
if_statement: TK_PR_IF '(' expression ')' block                  { $$ = ast_make_node(statement_if); ast_append($$, $3); ast_append($$, $5); ast_append($$, ast_make_node(noop)); };

while_statement: TK_PR_WHILE '(' expression ')' block { $$ = ast_make_node(statement_while); ast_append($$, $3); ast_append($$, $5); };

expression: expr_7 { $$ = $1; };

expr_1: expr_v { $$ = $1; };
expr_1: '-' expr_1 { $$ = ast_make_node(expr_inv); ast_append($$, $2); };
expr_1: '!' expr_1 { $$ = ast_make_node(expr_not); ast_append($$, $2); };

expr_2: expr_1 { $$ = $1; };
expr_2: expr_2 '*' expr_1 { $$ = ast_make_node(expr_mult); ast_append($$, $1); ast_append($$, $3);};
expr_2: expr_2 '/' expr_1 { $$ = ast_make_node(expr_div); ast_append($$, $1); ast_append($$, $3);};
expr_2: expr_2 '%' expr_1 { $$ = ast_make_node(expr_mod); ast_append($$, $1); ast_append($$, $3);};

expr_3: expr_2 { $$ = $1; };
expr_3: expr_3 '+' expr_2 { $$ = ast_make_node(expr_add); ast_append($$, $1); ast_append($$, $3);};
expr_3: expr_3 '-' expr_2 { $$ = ast_make_node(expr_sub); ast_append($$, $1); ast_append($$, $3);};

expr_4: expr_3 { $$ = $1; };
expr_4: expr_4 '<' expr_3 { $$ = ast_make_node(expr_lt); ast_append($$, $1); ast_append($$, $3);};
expr_4: expr_4 '>' expr_3 { $$ = ast_make_node(expr_gt); ast_append($$, $1); ast_append($$, $3);};
expr_4: expr_4 TK_OC_LE expr_3 { $$ = ast_make_node(expr_le); ast_append($$, $1); ast_append($$, $3);};
expr_4: expr_4 TK_OC_GE expr_3 { $$ = ast_make_node(expr_ge); ast_append($$, $1); ast_append($$, $3);};

expr_5: expr_4 { $$ = $1; };
expr_5: expr_5 TK_OC_EQ expr_4 { $$ = ast_make_node(expr_eq); ast_append($$, $1); ast_append($$, $3);};
expr_5: expr_5 TK_OC_NE expr_4 { $$ = ast_make_node(expr_ne); ast_append($$, $1); ast_append($$, $3);} 

expr_6: expr_5 { $$ = $1; };
expr_6: expr_6 TK_OC_AND expr_5 { $$ = ast_make_node(expr_and); ast_append($$, $1); ast_append($$, $3);};

expr_7: expr_6 { $$ = $1; };
expr_7: expr_7 TK_OC_OR expr_6 { $$ = ast_make_node(expr_or); ast_append($$, $1); ast_append($$, $3);};

expr_v: '(' expression ')' { $$ = $2; };
expr_v: identifier { $$ = $1; };
expr_v: literal { $$ = $1; };
expr_v: function_call { $$ = $1; };

type: TK_PR_INT { };
type: TK_PR_FLOAT { };
type: TK_PR_BOOL { };

identifier: TK_IDENTIFICADOR { $$ = ast_make_leaf(ident, $1); };

literal: TK_LIT_INT { $$ = ast_make_leaf(lit_int_type, $1); };
literal: TK_LIT_FLOAT { $$ = ast_make_leaf(lit_float_type, $1); };
literal: TK_LIT_TRUE { $$ = ast_make_leaf(lit_bool_type, $1); };
literal: TK_LIT_FALSE { $$ = ast_make_leaf(lit_bool_type, $1); };

%%

void yyerror (char const *mensagem) {
    // A seguinte linha de código demontra como apresentar a coluna do erro, 
    // functionalidade que nao foi habilitada para seguir a especificacao com mais regor:
    // printf("Erro na linha %d, coluna %d: \n - %s\n", get_line_number(), get_col_number(), mensagem);
    printf("Erro na linha %d: \n - %s\n", get_line_number(), mensagem);
}

