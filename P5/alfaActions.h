#ifndef _ALFA_ACTIONS_H
#define _ALFA_ACTIONS_H
#include "alfa.h"
void set_class(variableType class);
void set_type(dataType type);
void set_vector_size(attributes_t $4);
void identifier(attributes_t $1);

void initialize();

void constant_int(attributes_t* $$, attributes_t $1);
void constant_propagate(attributes_t *$$, attributes_t $1);
void constant_logic(attributes_t *$$, int val);
void constant_to_stack(attributes_t $1);

void vector_element(attributes_t* $$, attributes_t $1,attributes_t $3);

void initialize_if(attributes_t *$$, attributes_t $3);
void if_propagate(attributes_t *$$, attributes_t $1);
void asign_scalar(attributes_t *$$, attributes_t $1, attributes_t $3);
void asign_vector(attributes_t *$$, attributes_t $1, attributes_t $3);
void exp_identificador(attributes_t* $$, attributes_t $1);

void read(attributes_t $2);
void returns(attributes_t $2);
void add(attributes_t *$$, attributes_t $1, attributes_t $3);
void substract(attributes_t *$$, attributes_t $1, attributes_t $3);
void divide(attributes_t *$$, attributes_t $1, attributes_t $3);
void multiply(attributes_t *$$, attributes_t $1, attributes_t $3);

void uminus(attributes_t *$$, attributes_t $2);

void and(attributes_t *$$, attributes_t $1, attributes_t $3);
void or(attributes_t *$$, attributes_t $1, attributes_t $3);

void not(attributes_t *$$, attributes_t $2);

void equal(attributes_t *$, attributes_t $1, attributes_t $3);
void not_equal(attributes_t *$, attributes_t $1, attributes_t $3);
void gte(attributes_t *$, attributes_t $1, attributes_t $3);
void lte(attributes_t *$, attributes_t $1, attributes_t $3);
void gt(attributes_t *$, attributes_t $1, attributes_t $3);
void lt(attributes_t *$, attributes_t $1, attributes_t $3);

void wh_start(attributes_t* $$);
void wh_condition(attributes_t* $$, attributes_t $1,attributes_t $3);
void wh_end(attributes_t $1);
void push_type_up(attributes_t*$$, attributes_t $1);

void new_function(attributes_t *$$, attributes_t $3);
void declare_function(attributes_t *$$, attributes_t $1,attributes_t $3);
void end_function();
void add_parameter(attributes_t $1);

#endif