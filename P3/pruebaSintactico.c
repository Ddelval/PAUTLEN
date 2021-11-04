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
extern int error_type;

void yyerror(const char *s) {
    printf("Error:%s", s);
    exit(1); // TODO: CHECK
}

int main(int argc, char **argv) {
    int c;
    const char *error = "****Error en [lin %d, col %d]: %s (%s)\n";
    const char *too_long = "identificador demasiado largo";
    const char *not_allowed = "simbolo no permitido";

    yyin = fopen(argv[1], "r");
    yyout = fopen(argv[2], "w");

    while ((c = yyparse())) {
        if (c == TOK_ERROR) {
            const char *message = error_type ? not_allowed : too_long;

            fprintf(stderr, error, lincount + 1, colcount + 1, message, yytext);
            break;
        }
        fprintf(yyout, "%s %d %s \n", string_val, c, yytext);
    }

    return 0;
}
