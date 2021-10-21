#include <stdio.h>
#include <stdlib.h>

extern FILE *yyin, *yyout;
extern int yylex();


int main(int argc, char **argv) {
    int c;

    yyin = fopen(argv[1], "r");
    yyout = fopen(argv[2], "w");

    while ((c = yylex())) {
        printf("%d\n", c);
    }

    return 0;
}