#include "tokens.h"
#include <stdio.h>
#include <stdlib.h>

extern FILE *yyin, *yyout;
extern int yylex();
extern int yyparse();
extern const char *yytext;
extern const char *string_val;
extern int colcount;
extern int lincount;
extern int lastlen;
extern int error_type;

void yyerror(const char *s) {
    const char *error_sint = "****Error sintactico en [lin %d, col %d]\n";
    const char *error = "****Error en [lin %d, col %d]: %s (%s)\n";
    const char *too_long = "identificador demasiado largo";
    const char *not_allowed = "simbolo no permitido";
    if (error_type == -1) {
        printf(error_sint, lincount + 1, colcount + 1 - lastlen);
    } else {
        const char *message = error_type ? not_allowed : too_long;
        printf(error, lincount + 1, colcount + 1, message, yytext);
    }
}

int main(int argc, char **argv) {
    int ret = 0;

    yyin = fopen(argv[1], "r");
    yyout = fopen(argv[2], "w");
    ret = yyparse();

    return ret;
}
