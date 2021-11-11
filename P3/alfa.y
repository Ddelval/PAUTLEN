%{
#include <stdlib.h>
#include <stdio.h>

extern int yylex();
extern int yyparse();
void yyerror(const char*s);
extern FILE *yyin, *yyout;
%}


%token TOK_MAIN
%token TOK_INT                 
%token TOK_BOOLEAN             
%token TOK_ARRAY
%token TOK_FUNCTION            
%token TOK_IF                  
%token TOK_ELSE                
%token TOK_WHILE               
%token TOK_SCANF               
%token TOK_PRINTF              
%token TOK_RETURN              

%token TOK_PUNTOYCOMA          
%token TOK_COMA                
%token TOK_PARENTESISIZQUIERDO 
%token TOK_PARENTESISDERECHO   
%token TOK_CORCHETEIZQUIERDO   
%token TOK_CORCHETEDERECHO     
%token TOK_LLAVEIZQUIERDA      
%token TOK_LLAVEDERECHA        
%token TOK_ASIGNACION          
%token TOK_MAS                 
%token TOK_MENOS               
%token TOK_DIVISION            
%token TOK_ASTERISCO           
%token TOK_AND                 
%token TOK_OR                  
%token TOK_NOT                 
%token TOK_IGUAL               
%token TOK_DISTINTO            


%token TOK_MENORIGUAL          
%token TOK_MAYORIGUAL          
%token TOK_MENOR               
%token TOK_MAYOR               

%token<cadena> TOK_IDENTIFICADOR
%token<numero> TOK_CONSTANTE_ENTERA
%token<numero> TOK_TRUE
%token<numero> TOK_FALSE

%token TOK_ERROR

%%
programa: TOK_MAIN TOK_LLAVEIZQUIERDA declaraciones funciones sentencias TOK_LLAVEDERECHA 
{fprintf(yyout, ";R1:\t<programa> ::= main { <declaraciones> <funciones> <sentencias> }\n");};

declaraciones: declaracion
{fprintf(yyout, ";R2:\t<declaraciones> ::= <declaracion>");};

declaraciones: declaracion declaraciones
{fprintf(yyout, ";R3:\t<declaraciones> ::= <declaracion> <declaraciones>");};

declaracion: clase identificadores TOK_PUNTOYCOMA
{fprintf(yyout, ";R4:\t<declaracion> ::= <clase> <identificadores> ;");};

clase: clase_escalar
{fprintf(yyout, ";R5:\t<clase> ::= <clase_escalar>");};

clase: clase_vector
{fprintf(yyout, ";R7:\t<clase> ::= <clase_vector>");};
clase_escalar: tipo
{fprintf(yyout, ";R9:\t<clase_escalar> ::= <tipo>");};

tipo: TOK_INT
{fprintf(yyout, ";R10:\t<tipo> ::= TOK_INT");};
tipo: TOK_BOOLEAN
{fprintf(yyout, ";R11:\t<tipo> ::= TOK_BOOLEAN");};

clase_vector: TOK_ARRAY tipo TOK_CORCHETEIZQUIERDO constante_entera TOK_CORCHETEDERECHO;

identificadores: identificador;
identificadores: identificador TOK_COMA identificadores;

funciones: funcion funciones;
funciones: ;

funcion: TOK_FUNCTION tipo identificador TOK_PARENTESISIZQUIERDO 
        parametros_funcion TOK_PARENTESISDERECHO TOK_LLAVEIZQUIERDA 
        declaraciones_funcion sentencias TOK_LLAVEDERECHA;
    
parametros_funcion: parametro_funcion resto_parametros_funcion;
parametros_funcion:;

resto_parametros_funcion: TOK_PUNTOYCOMA parametro_funcion resto_parametros_funcion;
resto_parametros_funcion: ;

parametro_funcion: tipo identificador;
declaraciones_funcion: declaraciones;
declaraciones_funcion: ;
sentencias: sentencia;
sentencias: sentencia sentencias;

sentencia: sentencia_simple TOK_PUNTOYCOMA;
sentencia: bloque;

sentencia_simple: asignacion;
sentencia_simple: lectura;
sentencia_simple: escritura;
sentencia_simple: retorno_funcion;

bloque: condicional;
bloque: bucle;

asignacion: identificador TOK_IGUAL exp;
asignacion: elemento_vector TOK_IGUAL exp;

elemento_vector: identificador TOK_CORCHETEIZQUIERDO exp TOK_CORCHETEDERECHO;

condicional: TOK_IF TOK_PARENTESISIZQUIERDO exp TOK_PARENTESISDERECHO 
            TOK_LLAVEIZQUIERDA sentencias TOK_LLAVEDERECHA;
condicional: TOK_IF TOK_PARENTESISIZQUIERDO exp TOK_PARENTESISDERECHO 
            TOK_LLAVEIZQUIERDA sentencias TOK_LLAVEDERECHA TOK_ELSE 
            TOK_LLAVEIZQUIERDA sentencias TOK_LLAVEDERECHA;

bucle: TOK_WHILE TOK_PARENTESISIZQUIERDO exp TOK_PARENTESISDERECHO 
            TOK_LLAVEIZQUIERDA sentencias TOK_LLAVEDERECHA;

lectura: TOK_SCANF identificador;

escritura: TOK_PRINTF exp;

retorno_funcion: TOK_RETURN exp;

exp: exp TOK_MAS exp;
exp: exp TOK_MENOS exp;
exp: exp TOK_DIVISION exp;
exp: exp TOK_ASTERISCO exp;
exp: TOK_MENOS exp;
exp: exp TOK_AND exp;
exp: exp TOK_OR exp;
exp: exp TOK_NOT exp;
exp: identificador;
exp: constante;
exp: TOK_PARENTESISIZQUIERDO exp TOK_PARENTESISDERECHO;
exp: TOK_PARENTESISIZQUIERDO comparacion TOK_PARENTESISDERECHO;
exp: elemento_vector;
exp: identificador TOK_PARENTESISIZQUIERDO 
    lista_expresiones TOK_PARENTESISDERECHO;

lista_expresiones: exp resto_lista_expresiones;
resto_lista_expresiones: TOK_COMA exp resto_lista_expresiones;
resto_lista_expresiones: ;

comparacion: exp TOK_IGUAL exp;
comparacion: exp TOK_DISTINTO exp;
comparacion: exp TOK_MENORIGUAL exp;
comparacion: exp TOK_MAYORIGUAL exp;
comparacion: exp TOK_MENOR exp;
comparacion: exp TOK_MAYOR exp;

constante: constante_entera;
constante: constante_logica;

constante_logica: TOK_TRUE;
constante_logica: TOK_FALSE;
constante_entera: TOK_CONSTANTE_ENTERA;

identificador: TOK_IDENTIFICADOR;
%%
