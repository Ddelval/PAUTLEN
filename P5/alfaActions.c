#include "alfaActions.h"
#include "syTable.h"
#include <stdio.h>

#include "generacion.h"

dataType current_type;
variableType current_class;
int vector_size = 0;

int labels = 0;
int noes = 0;
int comparisons = 0;
bool returning = false;

dataType current_function_type;
int num_params = 0;
int pos_param = 0;
int num_local_vars = 0;
int pos_local_vars = 0;
bool function_body = false;
bool function_calling = false;
int calling_params = 0;

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
    error_str index_exp_not_allowed;
    error_str function_non_eval;
    error_str read_only_scalar;
    error_str cexp_div_zero;
    error_str var_as_fun;
};
struct error_c errors = {"Error semantico en lin %d: %s\n",
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
                         "Variable local de tipo no escalar.",
                         "Expresion no valida para indexar un vector",
                         "Una funcion no puede ser evaluada",
                         "Solo esta permitido leer tipos escalares",
                         "Division entre cero en expresion constante",
                         "Una variable no puede ser llamada"};

struct internal_error_c {
    error_str create_symbol, create_table, create_scope, close_scope, insert_symbol;
};

struct internal_error_c internal_errors = {
        "Error interno al crear el simbolo: %s",
        "Error interno al crear la tabla de simbolos",
        "Error interno al crear un nuevo scope en la tabla de simbolos",
        "Error interno al cerrar el scope en la tabla de simbolos",
        "Error interno al insertar en la tabla de simbolos: %s"};


void exit_error(error_str error, const char *optional) {
    fprintf(stdout, errors.base, lincount, error, optional);
    syTable_destroy(symbolTable);
    exit(-1);
}

//-----------------------------------------------------------------------------
// Functions
//-----------------------------------------------------------------------------
void set_class(variableType class) { current_class = class; }

void set_type(dataType type) { current_type = type; }

void set_vector_size(attributes_t $4) {
    vector_size = $4.value_int;
}

void identifier(attributes_t $1) {
    if (syTable_duplicated(symbolTable, $1.lexeme)) {
        exit_error(errors.duplicated_declaration, "");
    } else {
        if (function_body && current_class == VECTOR) {
            exit_error(errors.local_no_scalar, $1.lexeme);
        }

        int size = current_class == VECTOR ? vector_size : 1;
        int pos_local_var = function_body ? num_local_vars : -1;

        Node *n;
        if (function_body) {
            n = create_variable(current_type, current_class,
                                size, $1.lexeme, pos_local_var);
            // fprintf(stderr, "\n***%d %d\n", pos_local_var, num_params);
        } else {
            n = create_variable(current_type, current_class,
                                size, $1.lexeme, -1);
        }
        if (!n) {
            exit_error(internal_errors.create_symbol, $1.lexeme);
        }

        if (!syTable_insert(symbolTable, *n)) {
            exit_error(internal_errors.insert_symbol, $1.lexeme);
        }
        node_free(n);

        if (function_body) {
            num_local_vars++;
            fprintf(yyout, ";Found a local variable\n");
        } else {
            declarar_variable(yyout, $1.lexeme, current_type, size);
        }

    }
}

void initialize() {
    symbolTable = syTable_create();
    if (!symbolTable) {
        exit_error(internal_errors.create_table, "");
    }
    escribir_subseccion_data(yyout);
    escribir_cabecera_bss(yyout);
    fflush(yyout);
}

void constant_int(attributes_t *$$, attributes_t $1) {
    $$->data_type = INT;
    $$->is_address = false;
    $$->is_constexpr = true;
    $$->value_int = $1.value_int;
}

void constant_propagate(attributes_t *$$, attributes_t $1) {
    $$->data_type = $1.data_type;
    $$->is_address = $1.is_address;
    $$->is_constexpr = true;
    $$->value_int = $1.value_int;
}

void constant_logic(attributes_t *$$, int val) {
    $$->data_type = BOOLEAN;
    $$->value_int = val;
    $$->is_constexpr = true;
    $$->is_address = false;
}

void constant_to_stack(attributes_t $1) {
    char digits[MAX_INT_DIGITS];
    sprintf(digits, "%d", $1.value_int);
    escribir_operando(yyout, digits, 0);
}

const Node *getSymbol(const char *name) {
    const Node *match = syTable_search(symbolTable, name);
    if (!match) {
        exit_error(errors.no_declarated, name);
    }
    return match;
}

void push_vector_address(attributes_t vector) {
    const Node *match = getSymbol(vector.lexeme);

    // fprintf(stderr, "%d\n", vector.index_attributes.is_constexpr);

    if (vector.index_attributes.is_constexpr) {
        char intbuffer[MAX_INT_DIGITS];
        sprintf(intbuffer, "%d", vector.index_attributes.value_int);
        escribir_operando(yyout, intbuffer, 0);
    } else {
        escribir_operando(yyout, vector.index_attributes.lexeme, 1);
    }
    escribir_elemento_vector(yyout, vector.lexeme, match->size,
                             !vector.index_attributes.is_constexpr);

}

void vector_element(attributes_t *$$, attributes_t $1, attributes_t $3) {
    const Node *match = getSymbol($1.lexeme);
    if (match->variable_type == SCALAR) {
        exit_error(errors.index_of_no_vector, "");
    }
    if ($3.data_type != INT) {
        exit_error(errors.index_no_int, "");
    }
    // fprintf(stderr, "%d\n", $3.is_address);

    if (!$3.is_constexpr && !$3.is_address) {
        exit_error(errors.index_exp_not_allowed, "");
    } else if ($3.is_address) {
        strcpy($$->index_attributes.lexeme, $3.lexeme);
        $$->index_attributes.is_constexpr = false;
    } else {
        $$->index_attributes.is_constexpr = true;
        $$->index_attributes.value_int = $3.value_int;
    }

    // Clear the exp value
    removeFromStack(yyout, 1);

    $$->is_address = true;
    $$->data_type = match->data_type;
    strcpy($$->lexeme, $1.lexeme);
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

void push_address(const Node *n) {
    if (n->type == FUNCION) {
        // Nothing to do
        return;
    } else if (n->type == PARAMETRO) {
        escribirParametro(yyout, n->param_position, num_params);
    } else if (n->pos_local_variable >= 0) {
        escribirVariableLocal(yyout, n->pos_local_variable);
    } else {
        escribir_operando(yyout, n->name, 1);
    }

}

void asign_scalar(attributes_t *$$, attributes_t $1, attributes_t $3) {
    const Node *match = getSymbol($1.lexeme);

    if (match->type == FUNCION || match->variable_type == VECTOR) {
        exit_error(errors.incompatible_assign, "");
    }
    if (match->data_type != $3.data_type) {
        exit_error(errors.incompatible_assign, "");
    }
    // fprintf(yyout, ";local: %d\n", match->pos_local_variable);
    if (match->pos_local_variable >= 0 || match->type == PARAMETRO) {
        push_address(match);
        asignarDestinoEnPila(yyout, $3.is_address);
    } else {
        asignar(yyout, match->name, $3.is_address);
    }
}

void asign_vector(attributes_t *$$, attributes_t $1, attributes_t $3) {
    push_vector_address($1);
    asignarDestinoEnPila(yyout, $3.is_address);
}

void exp_identificador(attributes_t *$$, attributes_t $1) {
    const Node *match = getSymbol($1.lexeme);

    if (match->type == FUNCION) {
        exit_error(errors.function_non_eval, "");
    } else if (match->type == PARAMETRO) {
        strcpy($$->lexeme, $1.lexeme);
    }

    $$->data_type = match->data_type;
    $$->is_address = true;
    // fprintf(yyout, ";loc:%d\n", match->pos_local_variable);
    push_address(match);
}

void exp_vector(attributes_t *$$, attributes_t $1) {
    push_vector_address($1);
    $$->is_address = true;
    $$->data_type = $1.data_type;
}

void read(attributes_t $2) {
    const Node *match = getSymbol($2.lexeme);

    if (match->type == FUNCION || match->variable_type != SCALAR) {
        exit_error(errors.read_only_scalar, "");
    } else {
        push_address(match);
        leer(yyout, $2.lexeme, match->data_type);
    }
}

void returns(attributes_t $2) {
    if (!function_body) {
        exit_error(errors.return_out_function, "");
    }
    returning = true;
    retornarFuncion(yyout, $2.is_address);
}

#define constexpr(x) \
if ($1.is_constexpr && $3.is_constexpr){ \
    $$->is_constexpr = true;           \
    $$->value_int = $1.value_int x $3.value_int; \
    }

void set_result_arithmetic(attributes_t *$$, attributes_t $1, attributes_t $3) {
    if ($1.data_type != INT || $3.data_type != INT) {
        exit_error(errors.arithmetic_with_boolean, "");
    }
    $$->is_address = false;
    $$->data_type = INT;
}

void add(attributes_t *$$, attributes_t $1, attributes_t $3) {
    set_result_arithmetic($$, $1, $3);
    sumar(yyout, $1.is_address, $3.is_address);
    constexpr(+);
}

void substract(attributes_t *$$, attributes_t $1, attributes_t $3) {
    set_result_arithmetic($$, $1, $3);
    restar(yyout, $1.is_address, $3.is_address);
    constexpr(-);
}

void divide(attributes_t *$$, attributes_t $1, attributes_t $3) {
    set_result_arithmetic($$, $1, $3);
    dividir(yyout, $1.is_address, $3.is_address);
    if ($3.is_constexpr && $3.value_int == 0) {
        exit_error(errors.cexp_div_zero, "");
    }
    constexpr(/);
}

void multiply(attributes_t *$$, attributes_t $1, attributes_t $3) {
    set_result_arithmetic($$, $1, $3);
    multiplicar(yyout, $1.is_address, $3.is_address);
    constexpr(*);
}

void uminus(attributes_t *$$, attributes_t $2) {
    if ($2.data_type != INT) {
        exit_error(errors.arithmetic_with_boolean, "");
    }
    $$->data_type = INT;
    $$->is_address = false;
    cambiar_signo(yyout, $2.is_address);
    if ($2.is_constexpr) {
        $$->is_constexpr = true;
        $$->value_int = -$2.value_int;
    }
}

void exp_paren(attributes_t *$$, attributes_t $2) {
    // TODO: Does this have any side effects ???
    *$$ = $2;
}

void set_result_logic(attributes_t *$$, attributes_t $1, attributes_t $3) {
    if ($1.data_type != BOOLEAN || $3.data_type != BOOLEAN) {
        exit_error(errors.logic_with_int, "");
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
        exit_error(errors.logic_with_int, 0);
    }
    $$->data_type = BOOLEAN;
    $$->is_address = false;
    no(yyout, $2.is_address, noes++);
}

void check_comparison(attributes_t *$$, attributes_t $1, attributes_t $3) {
    if ($1.data_type != INT || $3.data_type != INT) {
        exit_error(errors.comparison_with_bool, "");
    }
    $$->data_type = BOOLEAN;
}

void equal(attributes_t *$$, attributes_t $1, attributes_t $3) {
    check_comparison($$, $1, $3);
    igual(yyout, $1.is_address, $3.is_address, comparisons++);
}

void not_equal(attributes_t *$$, attributes_t $1, attributes_t $3) {
    check_comparison($$, $1, $3);
    distinto(yyout, $1.is_address, $3.is_address, comparisons++);
}

void gte(attributes_t *$$, attributes_t $1, attributes_t $3) {
    check_comparison($$, $1, $3);
    mayor_igual(yyout, $1.is_address, $3.is_address, comparisons++);
}

void lte(attributes_t *$$, attributes_t $1, attributes_t $3) {
    check_comparison($$, $1, $3);
    menor_igual(yyout, $1.is_address, $3.is_address, comparisons++);
}

void gt(attributes_t *$$, attributes_t $1, attributes_t $3) {
    check_comparison($$, $1, $3);
    mayor(yyout, $1.is_address, $3.is_address, comparisons++);
}

void lt(attributes_t *$$, attributes_t $1, attributes_t $3) {
    check_comparison($$, $1, $3);
    menor(yyout, $1.is_address, $3.is_address, comparisons++);
}


void wh_start(attributes_t *$$) {
    $$->label = labels++;
    while_inicio(yyout, $$->label);
}

void wh_condition(attributes_t *$$, attributes_t $1, attributes_t $3) {
    $$->label = $1.label;
    while_exp_pila(yyout, $1.is_address, $$->label);
}

void wh_end(attributes_t $1) {
    while_fin(yyout, $1.label);
}

void push_type_up(attributes_t *$$, attributes_t $1) {
    $$->data_type = $1.data_type;
}

void new_function(attributes_t *$$, attributes_t $3) {
    const Node *match = syTable_search(symbolTable, $3.lexeme);
    if (match) {
        exit_error(errors.duplicated_declaration, "");
    }

    current_function_type = current_type;
    num_params = 0;
    pos_param = 0;
    num_local_vars = 0;
    pos_local_vars = 0;
    function_body = true;
    returning = false;

    strcpy($$->lexeme, $3.lexeme);

    const Node *match2 = create_function(current_function_type, $3.lexeme, num_params, num_local_vars);

    if (!match2) {
        exit_error(internal_errors.create_symbol, $3.lexeme);
    }
    if (syTable_search(symbolTable, $3.lexeme)) {
        exit_error(errors.duplicated_declaration, "");
    }

    if (!syTable_create_scope(symbolTable, *match2)) {
        exit_error(internal_errors.create_scope, "");
    }
    node_free((Node *) match2);
}

void declare_function(attributes_t *$$, attributes_t $1, attributes_t $3) {
    syTable_setFunctionValues(symbolTable,$1.lexeme, num_params,num_local_vars);

    strcpy($$->lexeme, $1.lexeme);
    fprintf(yyout, "; %d local variables\n", num_local_vars);
    declararFuncion(yyout, $$->lexeme, num_local_vars);
}

void end_function() {
    if (!returning) {
        exit_error(errors.no_return, "");
    }
    if (!syTable_close_scope(symbolTable)) {
        exit_error(internal_errors.close_scope, "");
    }

    function_body = false;
    num_local_vars = 0;
}

void add_parameter(attributes_t $1) {
    const Node *match = syTable_search(symbolTable, $1.lexeme);
    if (match) {
        exit_error(errors.duplicated_declaration, "");
    }

    match = create_parameter(current_type, $1.lexeme, ++pos_param);
    if (!match) {
        exit_error(internal_errors.create_symbol, $1.lexeme);
    }

    if (!syTable_insert(symbolTable, *match)) {
        exit_error(internal_errors.insert_symbol, $1.lexeme);
    }

    node_free((Node *) match);
    num_params++;
    // fprintf(stderr, "\npass %d\n", num_params);
}

void accumulate_size() {
    calling_params++;
}

void exp_to_argument(attributes_t $1) {
    operandoEnPilaAArgumentoIndex(yyout, $1.is_address, calling_params);
    accumulate_size();
}

void function_call(attributes_t *$$, attributes_t $1, attributes_t $3) {
    const Node *match = syTable_search(symbolTable, $1.lexeme);

    // This checkings are ridiculous as we have already done them, but just in case
    // we change something in the middle I'm keeping them. The more, the merrier
    if (!match) {
        exit_error(errors.no_declarated, $1.lexeme);
    }
    if (match->type != FUNCION) {
        exit_error(errors.var_as_fun, "");
    }

    if (match->n_parameters != calling_params) {
        exit_error(errors.wrong_num_params, "");
    }

    llamarFuncion(yyout, $1.lexeme, calling_params);
    function_calling = false;
    calling_params = 0;

    $$->is_address = false;
    $$->data_type = match->data_type;
}

void check_calling(attributes_t *$$, attributes_t $1) {
    const Node *match = syTable_search(symbolTable, $1.lexeme);
    if (!match) {
        exit_error(errors.no_declarated, $1.lexeme);
    }
    if (match->type != FUNCION) {
        exit_error(errors.var_as_fun, "");
    }

    if (function_calling) {
        exit_error(errors.call_as_param, "");
    }

    function_calling = true;
    calling_params = 0;

    strcpy($$->lexeme, $1.lexeme);
}