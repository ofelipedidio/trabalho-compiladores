#ifndef LEXEME
#define LEXEME

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#define bool int
#define true 1
#define false 0

/* ##############
 * # Structures #
 * ############## */
typedef enum {
    lex_lit_int,
    lex_lit_float,
    lex_lit_bool,
    lex_ident,
} lexeme_type_t;

typedef struct {
    lexeme_type_t type;
    char *text;
    uint64_t line;
    uint64_t column;
} lexeme_t;

/* ################
 * # Constructors #
 * ################ */
lexeme_t make_lexeme_int(int line, int column, long long int value);

lexeme_t make_lexeme_float(int line, int column, double value);

lexeme_t make_lexeme_bool(int line, int column, bool value);

lexeme_t make_lexeme_ident(int line, int column, char *value);

lexeme_t lex_clone(lexeme_t lexeme);

/* ###############
 * # Destructors #
 * ############### */
void lexeme_free(lexeme_t lexeme);

/* ###########
 * # Methods #
 * ########### */
void lexeme_to_string(lexeme_t lexeme, FILE *file);

#endif // !LEXEME
