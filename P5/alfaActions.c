#include "alfaActions.h"
#include "syTable.h"
#include <stdio.h>

#include "generacion.h"

dataType current_type;
variableType current_class;
int labels = 0;
int noes = 0;
bool in_function = false;
bool returning = false;
extern syTable *symbolTable;
extern FILE *yyout;
extern int lincount;

//-----------------------------------------------------------------------------
// Error definition
//-----------------------------------------------------------------------------
typedef const char *error_str;
struct error_c {
    error_str base;
    error_str duplicated_declaration, no_declarated, arithmetic_with_boolean;
    error_str logic_with_int, comparison_with_bool, conditional_with_int;
    error_str loop_condition_int, wrong_num_params, incompatible_assign;
    error_str vector_size, index_of_no_vector, index_no_int;
    error_str no_return, return_out_function;
    error_str call_as_param, local_no_scalar;
};
struct error_c errors = {"Error semantico en lin %d",
                         "Declaracion duplicada.",
                         "Acceso a variable no declarada(%s).",
                         "Operacion aritmetica con operandos boolean.",
                         "Operacion logica con operandos int.",
                         "Comparacion con operandos boolean.",
                         "Condicional con condicion de tipo int.",
                         "Bucle con condicion de tipo int.",
                         "Numero incorrecto de parametros en llamada a funcion.",
                         "Asignacion incompatible.",
                         "El tamanyo del vector %s excede los limites permitidos (1,64).",
                         "Intento de indexacion de una variable que no es de tipo vector.",
                         "El indice en una operacion de indexacion tiene que ser de tipo entero.",
                         "Funcion %s sin sentencia de retorno.",
                         "Sentencia de retorno fuera del cuerpo de una función.",
                         "No esta permitido el uso de llamadas a funciones como parametros de otras funciones.",
                         "Variable local de tipo no escalar."};

void exit_error(error_str error, const char *optional) {
    fprintf(stdout, error, lincount, optional);
    exit(-1);
}

//-----------------------------------------------------------------------------
// Functions
//-----------------------------------------------------------------------------
void set_class(variableType class) { current_class = class; }

void set_type(dataType type) { current_type = type; }

void identifier(attributes_t $1) {
    if (syTable_search(symbolTable, $1.lexeme)) {
        exit_error(errors.duplicated_declaration, "");
    } else {
        //TODO: Handle vectors
        Node *n = create_variable(current_type, current_class,
                                  1, $1.lexeme, -1);
        syTable_insert(symbolTable, *n);
        node_free(n);
        declarar_variable(yyout, $1.lexeme, current_type, 1);

    }
}

void initialize() {
    symbolTable = syTable_create();
    if (!symbolTable) {
        fprintf(stderr, "Error allocating symbol table");
        exit(-1);
    }
    escribir_subseccion_data(yyout);
    escribir_cabecera_bss(yyout);
}

void constant(attributes_t *$$, attributes_t $1) {
    $$->data_type = INT;
    $$->is_address = false;
    $$->value_int = $1.value_int;

    char digits[MAX_INT_DIGITS];
    sprintf(digits, "%d", $1.value_int);
    escribir_operando(yyout, digits, 0);
}

void constant_propagate(attributes_t *$$, attributes_t $1) {
    $$->data_type = $1.data_type;
    $$->is_address = $1.is_address;
}

void constant_logic(attributes_t *$$, int val) {
    $$->data_type = BOOLEAN;
    $$->value_int = val;
    $$->is_address = false;
    escribir_operando(yyout, val ? "1" : "0", 0);
}

void initialize_if(attributes_t *$$, attributes_t $3) {
    if ($3.data_type != BOOLEAN) {
        exit_error(errors.conditional_with_int, "");
    }

    $$->label = labels++;

    ifthen_inicio(yyout, $3.is_address, $$->label);
}

void if_propagate(attributes_t *$$, attributes_t $1) {
    $$->label = $1.label;
}

const Node *getSymbol(const char *name) {
    const Node *match = syTable_search(symbolTable, name);
    if (!match) {
        exit_error(errors.no_declarated, name);
    }
    return match;
}

void asign_scalar(attributes_t *$$, attributes_t $1, attributes_t $3) {
    const Node *match = getSymbol($1.lexeme);

    if (match->type == FUNCION || match->type == PARAMETRO) {
        exit_error(errors.incompatible_assign, "");
    }
    if (match->data_type != $3.data_type) {
        exit_error(errors.incompatible_assign, "");
    }
    asignar(yyout, match->name, $3.is_address);
}

void exp_identificador(attributes_t *$$, attributes_t $1) {
    const Node *match = getSymbol($1.lexeme);

    if (match->type == FUNCION || match->type == PARAMETRO) {
        //TODO: Print some sort of error. Which one ???
    }

    $$->data_type = match->data_type;
    $$->is_address = true;
    escribir_operando(yyout, match->name, 1);
}

void read(attributes_t $2) {
    const Node *match = getSymbol($2.lexeme);

    if (match->type == PARAMETRO || match->type == FUNCION) {
        //TODO: Error. Also check parameter
    } else if (match->variable_type != SCALAR) {
        //TODO: Error
    } else {
        leer(yyout, $2.lexeme, match->data_type);
    }
}

void returns(attributes_t $2) {
    if (!in_function) {
        //TODO: Error
    }
    returning = true;
    retornarFuncion(yyout, $2.is_address);
}

void set_result_arithmetic(attributes_t *$$, attributes_t $1, attributes_t $3) {
    if ($1.data_type != INT || $3.data_type != INT) {
        //TODO: Error
    }
    $$->is_address = false;
    $$->data_type = INT;
}

void add(attributes_t *$$, attributes_t $1, attributes_t $3) {
    set_result_arithmetic($$, $1, $3);
    sumar(yyout, $1.is_address, $3.is_address);
}

void substract(attributes_t *$$, attributes_t $1, attributes_t $3) {
    set_result_arithmetic($$, $1, $3);
    restar(yyout, $1.is_address, $3.is_address);
}

void divide(attributes_t *$$, attributes_t $1, attributes_t $3) {
    set_result_arithmetic($$, $1, $3);
    dividir(yyout, $1.is_address, $3.is_address);
}

void multiply(attributes_t *$$, attributes_t $1, attributes_t $3) {
    set_result_arithmetic($$, $1, $3);
    multiplicar(yyout, $1.is_address, $3.is_address);
}

void uminus(attributes_t *$$, attributes_t $2) {
    if ($2.data_type != INT) {
        //TODO: Error
    }
    $$->data_type = INT;
    $$->is_address = false;
    cambiar_signo(yyout, $2.is_address);
}

void set_result_logic(attributes_t *$$, attributes_t $1, attributes_t $3) {
    if ($1.data_type != BOOLEAN || $3.data_type != BOOLEAN) {
        //TODO: Error
    }
    $$->is_address = false;
    $$->data_type = BOOLEAN;
}

void and(attributes_t *$$, attributes_t $1, attributes_t $3) {
    set_result_logic($$, $1, $3);
    y(yyout, $1.is_address, $3.is_address);
}

void or(attributes_t *$$, attributes_t $1, attributes_t $3) {
    set_result_logic($$, $1, $3);
    o(yyout, $1.is_address, $3.is_address);
}

void not(attributes_t *$$, attributes_t $2) {
    if ($2.data_type != BOOLEAN) {
        //TODO: Error
    }
    $$->data_type = BOOLEAN;
    $$->is_address = false;
    no(yyout, $2.is_address, noes++);
}