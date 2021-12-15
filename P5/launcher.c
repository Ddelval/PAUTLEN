#include "syTable.h"
#include "alfa.h"
#include <stdio.h>


syTable *symbolTable;

extern FILE *yyin, *yyout;

extern int yyparse();

int main(int argc, char **argv) {
    int ret = 0;

    yyin = fopen(argv[1], "r");
    yyout = fopen(argv[2], "w");
    ret = yyparse();

    return ret;
}