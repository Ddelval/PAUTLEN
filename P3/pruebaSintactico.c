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

int main(int argc, char **argv) {
    int ret = 0;

    yyin = fopen(argv[1], "r");
    yyout = fopen(argv[2], "w");
    ret = yyparse();

    return ret;
}
