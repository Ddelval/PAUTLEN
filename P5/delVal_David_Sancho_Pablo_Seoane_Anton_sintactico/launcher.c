#include "syTable.h"
#include "alfa.h"
#include <stdio.h>


syTable *symbolTable;

extern FILE *yyin, *yyout;

extern int yyparse();
extern int yylex_destroy();

int main(int argc, char **argv) {
    int ret = 0;

    yyin = fopen(argv[1], "r");
    yyout = fopen(argv[2], "w");
    ret = yyparse();
    //fflush(yyout);
    //fflush(stdout);

    syTable_destroy(symbolTable);

    fclose(yyin);
    fclose(yyout);

    yylex_destroy();

    return ret;
}