/* Porto Alegre, Novembro de 2023
 * INF01147 - Compiladores
 *
 * Grupo B
 * Felipe Souza Didio - 00323392
 * Pedro Company Beck - 00324055
 *
 */

%option yylineno
%option noinput nounput noyywrap nodefault

%{
#include<stdio.h>
#include<stdlib.h>
#include"parser.tab.h"

int line = 1;
int col = 0;
int line_next = 1;
int col_next = 1;

int get_line_number(void);
int get_col_number(void);
void process_match();
%}

white [ \t\r\n]+
digit [0-9]
alpha [a-z]
alphanum [a-z0-9]

line_comment_begin \/\/
block_comment_begin \/\*
block_comment_end \*\/

%%

{white} {process_match();}

{line_comment_begin}.* {process_match();}
{block_comment_begin}([^*]|\*+[^/*])*(\**){block_comment_end} {process_match(); }

int {process_match(); return TK_PR_INT;}
float {process_match(); return TK_PR_FLOAT;}
bool {process_match(); return TK_PR_BOOL;}
if {process_match(); return TK_PR_IF;}
else {process_match(); return TK_PR_ELSE;}
while {process_match(); return TK_PR_WHILE;}
return {process_match(); return TK_PR_RETURN;}

\! {process_match(); return (int) '!';}
\* {process_match(); return (int) '*';}
\/ {process_match(); return (int) '/';}
\% {process_match(); return (int) '%';}
\+ {process_match(); return (int) '+';}
\- {process_match(); return (int) '-';}
\< {process_match(); return (int) '<';}
\> {process_match(); return (int) '>';}
\{ {process_match(); return (int) '{';}
\} {process_match(); return (int) '}';}
\( {process_match(); return (int) '(';}
\) {process_match(); return (int) ')';}
\= {process_match(); return (int) '=';}
\, {process_match(); return (int) ',';}
\; {process_match(); return (int) ';';}

\<\= {process_match(); return TK_OC_LE;}
\>\= {process_match(); return TK_OC_GE;}
\=\= {process_match(); return TK_OC_EQ;}
\!\= {process_match(); return TK_OC_NE;}
\&   {process_match(); return TK_OC_AND;}
\|   {process_match(); return TK_OC_OR;}

{digit}+ {process_match(); yylval.lexeme = yylval.lexeme = make_lexeme_int(get_line_number(), atoll(yytext)); return TK_LIT_INT;}
{digit}*\.{digit}+ {process_match(); yylval.lexeme = make_lexeme_float(get_line_number(), atof(yytext)); return TK_LIT_FLOAT;}
true {process_match(); yylval.lexeme = make_lexeme_bool(get_line_number(), 1); return TK_LIT_TRUE;}
false {process_match(); yylval.lexeme = make_lexeme_bool(get_line_number(), 0); return TK_LIT_FALSE;}

({alpha}|_)({alphanum}|_)* {process_match(); yylval.lexeme = make_lexeme_ident(get_line_number(), strdup(yytext)); return TK_IDENTIFICADOR;}

. {process_match(); return TK_ERRO;}

%%

int get_line_number(void) {
    return line;
}

int get_col_number(void) {
    return col;
}

void process_match() {
    line = line_next;
    col = col_next;
    for (int i = 0; i < yyleng; i++) {
        switch (yytext[i]) {
            case '\n':
                line_next++;
                col_next = 0;
                break;
            default:
                col_next++;
                break;
        }
    }
}

