%{
    #include <stdio.h>
    #include "tokens.h"
    extern int yylen;
    const char* string_val;
    #define cret(x)  string_val=#x; return x;
%}

%option noyywrap

DIGIT   [0-9]
LETTER  [A-Za-z]

%%

"//"[^\n]* { }
";"   { cret(TOK_PUNTOYCOMA); }
","   { cret(TOK_COMA); }
"("   { cret(TOK_PARENTESISIZQUIERDO); }
")"   { cret(TOK_PARENTESISDERECHO); }
"["   { cret(TOK_CORCHETEIZQUIERDO); }
"]"   { cret(TOK_CORCHETEDERECHO); }
"{"   { cret(TOK_LLAVEIZQUIERDA); }
"}"   { cret(TOK_LLAVEDERECHA); }
"="   { cret(TOK_ASIGNACION); }
"+"   { cret(TOK_MAS); }
"-"   { cret(TOK_MENOS); }
"/"   { cret(TOK_DIVISION); }
"*"   { cret(TOK_ASTERISCO); }
"&&"  { cret(TOK_AND); }
"||"    { cret(TOK_OR        );}
"!"    { cret(TOK_NOT       );}
"=="    { cret(TOK_IGUAL     );}
"!="    { cret(TOK_DISTINTO  );}
"<="    { cret(TOK_MENORIGUAL);}
">="    { cret(TOK_MAYORIGUAL);}
"<"    { cret(TOK_MENOR     );}
">"    { cret(TOK_MAYOR     );}
"main"  { cret(TOK_MAIN    ); }
"int"  { cret(TOK_INT     ); }
"boolean"  { cret(TOK_BOOLEAN ); }
"array"  { cret(TOK_ARRAY   ); }
"function"  { cret(TOK_FUNCTION); }
"if"  { cret(TOK_IF      ); }
"else"  { cret(TOK_ELSE    ); }
"while"  { cret(TOK_WHILE   ); }
"scanf"  { cret(TOK_SCANF   ); }
"printf"  { cret(TOK_PRINTF  ); }
"return"  { cret(TOK_RETURN  ); }
" " { }

"true" {cret(TOK_TRUE);}
"false" {cret(TOK_FALSE);}

[A-Za-z]([A-Za-z]|[0-9])* { if (yyleng > 100) { cret(TOK_ERROR); } else { cret(TOK_IDENTIFICADOR); } }
[0-9]+ { cret(TOK_CONSTANTE_ENTERA); }


%%