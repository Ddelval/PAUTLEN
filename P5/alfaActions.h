#ifndef _ALFA_ACTIONS_H
#define _ALFA_ACTIONS_H
#include "alfa.h"
void set_class(variableType class);
void set_type(dataType type);
void identifier(attributes_t $1);

void initialize();

void constant(attributes_t* $$, attributes_t $1);
void constant_propagate(attributes_t *$$, attributes_t $1);
void constant_logic(attributes_t *$$);

void initialize_if(attributes_t *$$, attributes_t $3);
void if_propagate(attributes_t *$$, attributes_t $1);
void asign_scalar(attributes_t *$$, attributes_t $1, attributes_t $3);
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

#endif