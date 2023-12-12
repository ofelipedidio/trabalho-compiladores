#include "lexeme.h"
#include <string.h>

/* ################
 * # Constructors #
 * ################ */
lexeme_t make_lexeme_int(int line, int column, long long int value) {
    lexeme_t lexeme;
    lexeme.line = line;
    lexeme.column = column;
    lexeme.type = lex_lit_int;
    lexeme.value.lit_int = value;
    return lexeme;
}

lexeme_t make_lexeme_float(int line, int column, double value) {
    lexeme_t lexeme;
    lexeme.line = line;
    lexeme.column = column;
    lexeme.type = lex_lit_float;
    lexeme.value.lit_float = value;
    return lexeme;
}

lexeme_t make_lexeme_bool(int line, int column, bool value) {
    lexeme_t lexeme;
    lexeme.line = line;
    lexeme.column = column;
    lexeme.type = lex_lit_bool;
    lexeme.value.lit_bool = value;
    return lexeme;
}

lexeme_t make_lexeme_ident(int line, int column, char *value) {
    lexeme_t lexeme;
    lexeme.line = line;
    lexeme.column = column;
    lexeme.type = lex_ident;
    lexeme.value.ident = value;
    return lexeme;
}

lexeme_t lex_clone(lexeme_t lexeme) {
    lexeme_t cloned;
    cloned.line = lexeme.line;
    cloned.column = lexeme.column;
    cloned.type = lexeme.type;

    if (lexeme.type == lex_ident) {
        cloned.value.ident = strdup(lexeme.value.ident);
    } else {
        cloned.value = lexeme.value;
    }

    return cloned;
}

/* ###############
 * # Destructors #
 * ############### */
void lexeme_free(lexeme_t lexeme) {
    switch (lexeme.type) {
        case lex_ident:
            free(lexeme.value.ident);
            break;
        default:
            break;
    }
}

/* ###########
 * # Methods #
 * ########### */
void lexeme_to_string(lexeme_t lexeme, FILE *file) {
    switch (lexeme.type) {
        case lex_lit_int:
            fprintf(file, "%lld", lexeme.value.lit_int);
            break;
        case lex_lit_float:
            fprintf(file, "%f", lexeme.value.lit_float);
            break;
        case lex_lit_bool:
            fprintf(file, "%s", lexeme.value.lit_bool ? "true" : "false");
            break;
        case lex_ident:
            fprintf(file, "%s", lexeme.value.ident);
            break;
    }
}

