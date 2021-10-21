#include <stdio.h>
#include <stdlib.h>
#include "tokens.h"

extern FILE *yyin, *yyout;
extern int yylex();
extern const char * yytext;
extern const char* string_val;

int main(int argc, char **argv) {
    int c;

    yyin = fopen(argv[1], "r");
    yyout = fopen(argv[2], "w");

    while ((c = yylex())) {
        if(c==TOK_ERROR){
            
        }
        printf("%s %d %s \n",string_val, c,yytext);
    }

    return 0;
}