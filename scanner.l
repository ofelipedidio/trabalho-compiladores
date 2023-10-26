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
{block_comment_begin}(.|{white})*{block_comment_end} {process_match();}

int {return TK_PR_INT;}
float {return TK_PR_FLOAT;}
bool {return TK_PR_BOOL;}
if {return TK_PR_IF;}
else {return TK_PR_ELSE;}
while {return TK_PR_WHILE;}
return {return TK_PR_RETURN;}

\! {return (int) '!';}
\* {return (int) '*';}
\/ {return (int) '/';}
\% {return (int) '%';}
\+ {return (int) '+';}
\- {return (int) '-';}
\< {return (int) '<';}
\> {return (int) '>';}
\{ {return (int) '{';}
\} {return (int) '}';}
\( {return (int) '(';}
\) {return (int) ')';}
\= {return (int) '=';}
\, {return (int) ',';}
\; {return (int) ';';}

\<\= {return TK_OC_LE;}
\>\= {return TK_OC_GE;}
\=\= {return TK_OC_EQ;}
\!\= {return TK_OC_NE;}
\&   {return TK_OC_AND;}
\|   {return TK_OC_OR;}

{digit}+ {return TK_LIT_INT;}
{digit}*\.{digit}+ {return TK_LIT_FLOAT;}
false {return TK_LIT_FALSE;}
true {return TK_LIT_TRUE;}

({alpha}|_)({alphanum}|_)* {return TK_IDENTIFICADOR;}

. {return TK_ERRO;}

%%

int get_line_number(void) {
    return yylineno;
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

