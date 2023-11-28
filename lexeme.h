#ifndef LEXEME
#define LEXEME

#include <stdio.h>
#include <stdlib.h>

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

typedef union {
    long long int lit_int;
    double lit_float;
    bool lit_bool;
    char *ident;
} lexeme_value_t;

typedef struct {
    long long int line;
    lexeme_type_t type;
    lexeme_value_t value;
} lexeme_t;

/* ################
 * # Constructors #
 * ################ */
lexeme_t make_lexeme_int(int line, long long int value);

lexeme_t make_lexeme_float(int line, double value);

lexeme_t make_lexeme_bool(int line, bool value);

lexeme_t make_lexeme_ident(int line, char *value);

/* ###############
 * # Destructors #
 * ############### */
void lexeme_free(lexeme_t lexeme);

/* ###########
 * # Methods #
 * ########### */
void lexeme_to_string(lexeme_t lexeme, FILE *file);

#endif // !LEXEME
