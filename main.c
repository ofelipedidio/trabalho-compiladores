/* Porto Alegre, Outubro de 2023
 * INF01147 - Compiladores
 *
 * Grupo B
 * Felipe Souza Didio
 * Pedro Company Beck
 *
 */

/*
Função principal para realização da análise sintática.

Este arquivo será posterioremente substituído, não acrescente nada.
*/
#include <stdio.h>
#include "parser.tab.h" //arquivo gerado com bison -d parser.y
extern int yylex_destroy(void);

int main (int argc, char **argv)
{
    int ret = yyparse();
    yylex_destroy();
    return ret;
}
