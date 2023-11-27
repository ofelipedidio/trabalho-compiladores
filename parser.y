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

%code requires { #include "ast.h" }

%start programa

%union {
    ast_value_t value;
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
%token<value> TK_IDENTIFICADOR
%token<value> TK_LIT_INT
%token<value> TK_LIT_FLOAT
%token<value> TK_LIT_FALSE
%token<value> TK_LIT_TRUE
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
%type<node> expr_8 

%type<node> literal
%type<node> identifier

%%

programa: global_definition { $$ = $1; arvore = $$; };
programa: { };

global_definition: global_variable_definition { /* NOOP */ };
global_definition: global_function_definition { $$ = $1; };
global_definition: global_definition global_variable_definition { $$ = $1; };
global_definition: global_definition global_function_definition { ast_add_child($1, $2); $$ = $1;};

global_variable_definition: type global_variable_definition_names ';' { /* NOOP */ };

global_variable_definition_names: identifier { ast_free($1); /* NOOP */ };
global_variable_definition_names: global_variable_definition_names ',' identifier { ast_free($3); /* NOOP */ };

global_function_definition: '(' parameter_list_definition ')' TK_OC_GE type '!' TK_IDENTIFICADOR block { $$ = ast_new(func_declaration, $7); ast_add_child($$, $8); };
global_function_definition: '('                           ')' TK_OC_GE type '!' TK_IDENTIFICADOR block { $$ = ast_new(func_declaration, $6); ast_add_child($$, $7); };

parameter_list_definition: parameter_definition { /* NOOP */ };
parameter_list_definition: parameter_list_definition ',' parameter_definition { /* NOOP */ };

parameter_definition: type identifier { ast_free($2); /* NOOP */ };

block: '{' block_body '}' { $$ = $2; };
block: '{'            '}' { $$ = ast_new_noop("empty_block"); };

block_body: command ';' { $$ = $1; };
block_body: command ';' block_body { if ($1->type == noop) { $$ = $3; } else if ($3->type == noop) { $$ = $1; } else { ast_add_child($1, $3); $$ = $1; } };

command: variable_declaration { /* NOOP */ };
command: variable_attribution { $$ = $1; };
command: function_call { $$ = $1; };
command: return_statement { $$ = $1; };
command: if_statement { $$ = $1; };
command: while_statement { $$ = $1; };
command: block { $$ = $1; };

variable_declaration: type variable_names { /* NOOP */ };

variable_names: identifier { ast_free($1); /* NOOP */ };
variable_names: variable_names ',' identifier { ast_free($3); /* NOOP */ };

variable_attribution: identifier '=' expression { $$ = ast_new_empty(statement_attr); ast_add_child($$, $1); ast_add_child($$, $3); };

function_call: TK_IDENTIFICADOR '(' parameter_list ')' { $$ = ast_new(statement_call, $1); ast_add_child($$, $3); };
function_call: TK_IDENTIFICADOR '('                ')' { $$ = ast_new(statement_call, $1); ast_add_child($$, ast_new_noop("empty_call")); };

parameter_list: expression { $$ = $1; };
parameter_list: expression ',' parameter_list  { ast_add_child($1, $3); $$ = $1; };

return_statement: TK_PR_RETURN expression { $$ = ast_new_empty(statement_return); ast_add_child($$, $2); };

if_statement: TK_PR_IF '(' expression ')' block { $$ = ast_new_empty(statement_if); ast_add_child($$, $3); ast_add_child($$, $5); ast_add_child($$, ast_new_noop("else_block")); };
if_statement: TK_PR_IF '(' expression ')' block TK_PR_ELSE block { $$ = ast_new_empty(statement_if); ast_add_child($$, $3); ast_add_child($$, $5); ast_add_child($$, $7); };

while_statement: TK_PR_WHILE '(' expression ')' block { $$ = ast_new_empty(statement_while); ast_add_child($$, $3); ast_add_child($$, $5); };

expression: expr_1 { $$ = $1; };

expr_1: expr_2 { $$ = $1; };
expr_1: '-' expr_1 { $$ = ast_new_empty(expr_inv); ast_add_child($$, $2); };
expr_1: '!' expr_1 { $$ = ast_new_empty(expr_not); ast_add_child($$, $2); };

expr_2: expr_3 { $$ = $1; };
expr_2: expr_2 '*' expr_3 { $$ = ast_new_empty(expr_mult); ast_add_child($$, $1); ast_add_child($$, $3);};
expr_2: expr_2 '/' expr_3 { $$ = ast_new_empty(expr_div); ast_add_child($$, $1); ast_add_child($$, $3);};
expr_2: expr_2 '%' expr_3 { $$ = ast_new_empty(expr_mod); ast_add_child($$, $1); ast_add_child($$, $3);};

expr_3: expr_4 { $$ = $1; };
expr_3: expr_3 '+' expr_4 { $$ = ast_new_empty(expr_add); ast_add_child($$, $1); ast_add_child($$, $3);};
expr_3: expr_3 '-' expr_4 { $$ = ast_new_empty(expr_sub); ast_add_child($$, $1); ast_add_child($$, $3);};

expr_4: expr_5 { $$ = $1; };
expr_4: expr_4 '<' expr_5 { $$ = ast_new_empty(expr_lt); ast_add_child($$, $1); ast_add_child($$, $3);};
expr_4: expr_4 '>' expr_5 { $$ = ast_new_empty(expr_gt); ast_add_child($$, $1); ast_add_child($$, $3);};
expr_4: expr_4 TK_OC_LE expr_5 { $$ = ast_new_empty(expr_le); ast_add_child($$, $1); ast_add_child($$, $3);};
expr_4: expr_4 TK_OC_GE expr_5 { $$ = ast_new_empty(expr_ge); ast_add_child($$, $1); ast_add_child($$, $3);};

expr_5: expr_6 { $$ = $1; };
expr_5: expr_5 TK_OC_EQ expr_6 { $$ = ast_new_empty(expr_eq); ast_add_child($$, $1); ast_add_child($$, $3);};
expr_5: expr_5 TK_OC_NE expr_6 { $$ = ast_new_empty(expr_ne); ast_add_child($$, $1); ast_add_child($$, $3);} 

expr_6: expr_7 { $$ = $1; };
expr_6: expr_6 TK_OC_AND expr_7 { $$ = ast_new_empty(expr_and); ast_add_child($$, $1); ast_add_child($$, $3);};

expr_7: expr_8 { $$ = $1; };
expr_7: expr_7 TK_OC_OR expr_8 { $$ = ast_new_empty(expr_or); ast_add_child($$, $1); ast_add_child($$, $3);};

expr_8: '(' expression ')' { $$ = $2; };
expr_8: identifier { $$ = $1; };
expr_8: literal { $$ = $1; };
expr_8: function_call { $$ = $1; };

type: TK_PR_INT { };
type: TK_PR_FLOAT { };
type: TK_PR_BOOL { };

identifier: TK_IDENTIFICADOR { $$ = ast_new(lit_ident_type, $1); };

literal: TK_LIT_INT { $$ = ast_new(lit_int_type, $1); };
literal: TK_LIT_FLOAT { $$ = ast_new(lit_float_type, $1); };
literal: TK_LIT_TRUE { $$ = ast_new(lit_bool_type, $1); };
literal: TK_LIT_FALSE { $$ = ast_new(lit_bool_type, $1); };

%%

void yyerror (char const *mensagem) {
    // A seguinte linha de código demontra como apresentar a coluna do erro, 
    // functionalidade que nao foi habilitada para seguir a especificacao com mais regor:
    // printf("Erro na linha %d, coluna %d: \n - %s\n", get_line_number(), get_col_number(), mensagem);
    printf("Erro na linha %d: \n - %s\n", get_line_number(), mensagem);
}

