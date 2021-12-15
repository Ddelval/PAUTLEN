#include "alfaActions.h"
#include "syTable.h"
#include <stdio.h>

#include "generacion.h"

dataType current_type;
variableType current_class;
extern syTable *symbolTable;
extern FILE* yyout;

void set_class(variableType class) { current_class = class; }

void set_type(dataType type) { current_type = type; }

void identifier(attributes_t $1) {
    if (syTable_search(symbolTable, $1.lexeme)) {
        //TODO: Error because  it already exists
    } else {
        //TODO: Handle vectors
        Node *n = create_variable(current_type, current_class,
                                  1, $1.lexeme, -1);
        syTable_insert(symbolTable, *n);
        node_free(n);
        declarar_variable(yyout,$1.lexeme, current_type,1);

    }
}
void initialize(){
    symbolTable = syTable_create();
    if (!symbolTable){
        fprintf(stderr,"Error allocating symbol table");
        exit(-1);
    }
    escribir_subseccion_data(yyout);
    escribir_cabecera_bss(yyout);
}