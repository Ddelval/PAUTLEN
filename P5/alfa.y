%{
#include <stdlib.h>
#include <stdio.h>
#include "alfaActions.h"
#include "generacion.h"

extern int yylex();
extern int yyparse();
void yyerror(const char*s);
extern FILE *yyin, *yyout;
extern const char *yytext;
extern const char *string_val;
extern int colcount;
extern int lincount;
extern int lastlen;
extern int error_type;


%}


%union {
    attributes_t attributes;
}

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

%token <attributes> TOK_IDENTIFICADOR
%token <attributes> TOK_CONSTANTE_ENTERA
%token TOK_TRUE
%token TOK_FALSE

%token TOK_ERROR

%type <attributes> asignacion
%type <attributes> condicional
%type <attributes> condicional_exp
%type <attributes> condicional_exp_else
%type <attributes> condicional_if
%type <attributes> comparacion
%type <attributes> elemento_vector
%type <attributes> exp
%type <attributes> constante
%type <attributes> constante_entera
%type <attributes> constante_logica
%type <attributes> identificador
%type <attributes> while_start
%type <attributes> while_condition

%type <attributes> funcion_tipo
%type <attributes> funcion_decl
%type <attributes> parametros_funcion
%type <attributes> idf_llamada_funcion
%type <attributes> lista_expresiones
%type <attributes> declaraciones_funcion
%type <attributes> declaraciones

// Following the C operator precedence standard:

%left TOK_OR
%left TOK_AND 
%left TOK_IGUAL TOK_DISTINTO
%left TOK_MENOR TOK_MAYOR TOK_MENORIGUAL TOK_MAYORIGUAL 
%left TOK_MAS TOK_MENOS
%left TOK_ASTERISCO TOK_DIVISION
%right TOK_NOT MENOSU


%%
programa: TOK_MAIN inicio TOK_LLAVEIZQUIERDA declaraciones escribir_TS funciones escribir_MAIN sentencias TOK_LLAVEDERECHA
{
	fprintf(yyout, ";R1:\t<programa> ::= main { <declaraciones> <funciones> <sentencias> }\n");
	escribir_fin(yyout);
};

inicio:
{initialize();};

escribir_TS:
{escribir_segmento_codigo(yyout);};

escribir_MAIN:
{escribir_inicio_main(yyout);};

declaraciones: declaracion
{
	fprintf(yyout, ";R2:\t<declaraciones> ::= <declaracion>\n");
};

declaraciones: declaracion declaraciones
{
	fprintf(yyout, ";R3:\t<declaraciones> ::= <declaracion> <declaraciones>\n");
};

declaracion: clase identificadores TOK_PUNTOYCOMA
{fprintf(yyout, ";R4:\t<declaracion> ::= <clase> <identificadores> ;\n");};

clase: clase_escalar
{
	fprintf(yyout, ";R5:\t<clase> ::= <clase_escalar>\n");
	set_class(SCALAR);
};

clase: clase_vector
{
	fprintf(yyout, ";R7:\t<clase> ::= <clase_vector>\n");
	set_class(VECTOR);
};

clase_escalar: tipo
{fprintf(yyout, ";R9:\t<clase_escalar> ::= <tipo>\n");};

tipo: TOK_INT
{
    fprintf(yyout, ";R10:\t<tipo> ::= int\n");
    set_type(INT);
};

tipo: TOK_BOOLEAN
{
    fprintf(yyout, ";R11:\t<tipo> ::= boolean\n");
    set_type(BOOLEAN);
};

clase_vector: TOK_ARRAY tipo TOK_CORCHETEIZQUIERDO constante_entera TOK_CORCHETEDERECHO
{
    fprintf(yyout, ";R15:\t<clase_vector> ::= array <tipo> [ <constante_entera> ]\n");
    set_vector_size($4);
};

identificadores: identificador
{fprintf(yyout, ";R18:\t<identificadores> ::= <identificador>\n");};

identificadores: identificador TOK_COMA identificadores
{fprintf(yyout, ";R19:\t<identificadores> ::= <identificador> , <identificadores>\n");};

funciones: funcion funciones
{fprintf(yyout, ";R20:\t<funciones> ::= <funcion> <funciones>\n");};

funciones: 
{fprintf(yyout, ";R21:\t<funciones> ::=\n");};

funcion: funcion_decl sentencias TOK_LLAVEDERECHA
{
	fprintf(yyout, ";R22:\t<funcion> ::= function <tipo> <identificador> ( <parametros_funcion> ) { <declaraciones_funcion> <sentencias> }\n");
	end_function();
};

funcion_decl: funcion_tipo TOK_PARENTESISIZQUIERDO
              parametros_funcion TOK_PARENTESISDERECHO TOK_LLAVEIZQUIERDA declaraciones_funcion
{
	declare_function(&$$, $1, $3);
};

funcion_tipo: TOK_FUNCTION tipo TOK_IDENTIFICADOR
{
	new_function(&$$, $3);
};
    
parametros_funcion: parametro_funcion resto_parametros_funcion
{fprintf(yyout, ";R23:\t<parametros_funcion> ::= <parametro_funcion> <resto_parametros_funcion>\n");};

parametros_funcion:
{fprintf(yyout, ";R24:\t<parametros_funcion> ::=\n");};

resto_parametros_funcion: TOK_PUNTOYCOMA parametro_funcion resto_parametros_funcion
{fprintf(yyout, ";R25:\t<resto_parametros_funcion> ::= ; <parametro_funcion> <resto_parametros_funcion>\n");};

resto_parametros_funcion: 
{fprintf(yyout, ";R26:\t<resto_parametros_funcion> ::=\n");};

parametro_funcion: tipo funcion_identificador
{fprintf(yyout, ";R27:\t<parametro_funcion> ::= <tipo> <identificador>\n");};

funcion_identificador: TOK_IDENTIFICADOR
{
	add_parameter($1);
};

declaraciones_funcion: declaraciones
{
	fprintf(yyout, ";R28:\t<declaraciones_funcion> ::= <declaraciones>\n");
};

declaraciones_funcion: 
{fprintf(yyout, ";R29:\t<declaraciones_funcion> ::=\n");};

sentencias: sentencia
{fprintf(yyout, ";R30:\t<sentencias> ::= <sentencia>\n");};

sentencias: sentencia sentencias
{fprintf(yyout, ";R31:\t<sentencias> ::= <sentencia> <sentencias>\n");};

sentencia: sentencia_simple TOK_PUNTOYCOMA
{fprintf(yyout, ";R32:\t<sentencia> ::= <sentencia_simple> ;\n");};

sentencia: bloque
{fprintf(yyout, ";R33:\t<sentencia> ::= <bloque>\n");};

sentencia_simple: asignacion
{fprintf(yyout, ";R34:\t<sentencia_simple> ::= <asignacion>\n");};

sentencia_simple: lectura
{fprintf(yyout, ";R35:\t<sentencia_simple> ::= <lectura>\n");};

sentencia_simple: escritura
{fprintf(yyout, ";R36:\t<sentencia_simple> ::= <escritura>\n");};

sentencia_simple: retorno_funcion
{fprintf(yyout, ";R38:\t<sentencia_simple> ::= <retorno_funcion>\n");};

bloque: condicional
{fprintf(yyout, ";R40:\t<bloque> ::= <condicional>\n");};

bloque: bucle
{fprintf(yyout, ";R41:\t<bloque> ::= <bucle>\n");};

asignacion: TOK_IDENTIFICADOR TOK_ASIGNACION exp
{
	fprintf(yyout, ";R43:\t<asignacion> ::= <identificador> = <exp>\n");
	asign_scalar(&$$, $1, $3);
};

asignacion: elemento_vector TOK_ASIGNACION exp
{
	fprintf(yyout, ";R44:\t<asignacion> ::= <elemento_vector> = <exp>\n");
	asign_vector(&$$, $1, $3);
};

elemento_vector: TOK_IDENTIFICADOR TOK_CORCHETEIZQUIERDO exp TOK_CORCHETEDERECHO
{
fprintf(yyout, ";R48:\t<elemento_vector> ::= <identificador> [ <exp> ]\n");
vector_element(&$$,$1,$3);
};

condicional: condicional_exp TOK_LLAVEDERECHA
{
    fprintf(yyout, ";R50:\t<condicional> ::= if ( <exp> ) { <sentencias> }\n");
    ifthen_fin(yyout, $1.label);
};

condicional: condicional_exp_else TOK_LLAVEIZQUIERDA sentencias TOK_LLAVEDERECHA
{
    fprintf(yyout, ";R51:\t<condicional> ::= if ( <exp> ) { <sentencias> } else { <sentencias> }\n");
    ifthenelse_fin(yyout, $1.label);
};

condicional_exp: condicional_if sentencias
{
    if_propagate(&$$, $1);
};

condicional_exp_else: condicional_exp TOK_LLAVEDERECHA TOK_ELSE
{
    ifthenelse_fin_then(yyout, $1.label);
};

condicional_if: TOK_IF TOK_PARENTESISIZQUIERDO exp TOK_PARENTESISDERECHO TOK_LLAVEIZQUIERDA
{
    initialize_if(&$$, $3);
};

bucle: while_condition TOK_LLAVEIZQUIERDA sentencias TOK_LLAVEDERECHA
{
fprintf(yyout, ";R52:\t<bucle> ::= while ( <exp> ) { <sentencias> }\n");
wh_end($1);
};

while_condition: while_start TOK_PARENTESISIZQUIERDO exp TOK_PARENTESISDERECHO
{
	wh_condition(&$$,$1,$3);
}
while_start: TOK_WHILE
{
	wh_start(&$$);
}

lectura: TOK_SCANF TOK_IDENTIFICADOR
{
	fprintf(yyout, ";R54:\t<lectura> ::= scanf <identificador>\n");
	read($2);
};

escritura: TOK_PRINTF exp
{
	fprintf(yyout, ";R56:\t<escritura> ::= printf <exp>\n");
	escribir(yyout, $2.is_address, $2.data_type);
};

retorno_funcion: TOK_RETURN exp
{
	fprintf(yyout, ";R61:\t<retorno_funcion> ::= return <exp>\n");
	returns($2);
};

exp: exp TOK_MAS exp
{
	fprintf(yyout, ";R72:\t<exp> ::= <exp> + <exp>\n");
	add(&$$, $1, $3);
};

exp: exp TOK_MENOS exp
{
	fprintf(yyout, ";R73:\t<exp> ::= <exp> - <exp>\n");
	substract(&$$, $1, $3);
};

exp: exp TOK_DIVISION exp
{
	fprintf(yyout, ";R74:\t<exp> ::= <exp> / <exp>\n");
	divide(&$$, $1, $3);
};

exp: exp TOK_ASTERISCO exp
{
	fprintf(yyout, ";R75:\t<exp> ::= <exp> * <exp>\n");
	multiply(&$$, $1, $3);
};

exp: TOK_MENOS exp %prec MENOSU
{
	fprintf(yyout, ";R76:\t<exp> ::= - <exp>\n");
	uminus(&$$, $2);
};

exp: exp TOK_AND exp
{
	fprintf(yyout, ";R77:\t<exp> ::= <exp> && <exp>\n");
	and(&$$, $1, $3);
};

exp: exp TOK_OR exp
{
	fprintf(yyout, ";R78:\t<exp> ::= <exp> || <exp>\n");
	or(&$$, $1, $3);
};

exp: TOK_NOT exp
{
	fprintf(yyout, ";R79:\t<exp> ::= ! <exp>\n");
	not(&$$, $2);
};

exp: TOK_IDENTIFICADOR
{
	fprintf(yyout, ";R80:\t<exp> ::= <identificador>\n");
	exp_identificador(&$$,$1);
};

exp: constante
{
	fprintf(yyout, ";R81:\t<exp> ::= <constante>\n");
	constant_propagate(&$$, $1);
	constant_to_stack($1);
};

exp: TOK_PARENTESISIZQUIERDO exp TOK_PARENTESISDERECHO
{
	fprintf(yyout, ";R82:\t<exp> ::= ( <exp> )\n");
	exp_paren(&$$,$2);
};

exp: TOK_PARENTESISIZQUIERDO comparacion TOK_PARENTESISDERECHO
{
	fprintf(yyout, ";R83:\t<exp> ::= ( <comparacion> )\n");
	push_type_up(&$$,$2);
};

exp: elemento_vector
{
	fprintf(yyout, ";R85:\t<exp> ::= <elemento_vector>\n");
	exp_vector(&$$,$1);
};

exp: idf_llamada_funcion TOK_PARENTESISIZQUIERDO
    lista_expresiones TOK_PARENTESISDERECHO
{
	fprintf(yyout, ";R88:\t<exp> ::= <identificador> ( <lista_expresiones> )\n");
	function_call(&$$,$1,$3);
};

idf_llamada_funcion: TOK_IDENTIFICADOR
{
	check_calling(&$$, $1);
};

lista_expresiones: exp resto_lista_expresiones
{
	fprintf(yyout, ";R89:\t<lista_expresiones> ::= <exp> <resto_lista_expresiones>\n");
	//operandoEnPilaAArgumento(yyout, $1.is_address);
	//accumulate_size();
	exp_to_argument($1);
};

lista_expresiones: 
{fprintf(yyout, ";R90:\t<lista_expresiones> ::=\n");};

resto_lista_expresiones: TOK_COMA exp resto_lista_expresiones
{
	fprintf(yyout, ";R91:\t<resto_lista_expresiones> ::= , <exp> <resto_lista_expresiones>\n");
	exp_to_argument($2);
	//operandoEnPilaAArgumentoIndex(yyout, $2.is_address);
	//accumulate_size();
};

resto_lista_expresiones: 
{fprintf(yyout, ";R92:\t<resto_lista_expresiones> ::=\n");};

comparacion: exp TOK_IGUAL exp
{
	fprintf(yyout, ";R93:\t<comparacion> ::= <exp> == <exp>\n");
	equal(&$$, $1, $3);
};

comparacion: exp TOK_DISTINTO exp
{
	fprintf(yyout, ";R94:\t<comparacion> ::= <exp> != <exp>\n");
	not_equal(&$$, $1, $3);
};

comparacion: exp TOK_MENORIGUAL exp
{
	fprintf(yyout, ";R95:\t<comparacion> ::= <exp> <= <exp>\n");
	lte(&$$, $1, $3);
};

comparacion: exp TOK_MAYORIGUAL exp
{
	fprintf(yyout, ";R96:\t<comparacion> ::= <exp> >= <exp>\n");
	gte(&$$, $1, $3);
};

comparacion: exp TOK_MENOR exp
{
	fprintf(yyout, ";R97:\t<comparacion> ::= <exp> < <exp>\n");
	lt(&$$, $1, $3);
};

comparacion: exp TOK_MAYOR exp
{
	fprintf(yyout, ";R98:\t<comparacion> ::= <exp> > <exp>\n");
	gt(&$$, $1, $3);
};

constante: constante_logica
{
	fprintf(yyout, ";R99:\t<constante> ::= <constante_logica>\n");
	constant_propagate(&$$, $1);
};

constante: constante_entera
{
	fprintf(yyout, ";R100:\t<constante> ::= <constante_entera>\n");
	constant_propagate(&$$, $1);
};

constante_logica: TOK_TRUE
{
    fprintf(yyout, ";R102:\t<constante_logica> ::= true\n");
    constant_logic(&$$,1);
};

constante_logica: TOK_FALSE
{
	fprintf(yyout, ";R103:\t<constante_logica> ::= false\n");
	constant_logic(&$$,0);
};

constante_entera: TOK_CONSTANTE_ENTERA
{
	fprintf(yyout, ";R104:\t<constante_entera> ::= TOK_CONSTANTE_ENTERA\n");
	constant_int(&$$, $1);
};

identificador: TOK_IDENTIFICADOR
{
	fprintf(yyout, ";R108:\t<identificador> ::= TOK_IDENTIFICADOR\n");
	identifier($1);
};
%%


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