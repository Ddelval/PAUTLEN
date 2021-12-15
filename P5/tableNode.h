
#ifndef _TABLE_NODE_H
#define _TABLE_NODE_H
#include "alfa.h"

typedef struct {
    elementType type;
    dataType data_type;
    variableType variable_type;
    short size;
    char *name;

    int n_parameters;
    int n_local_variables;

    int pos_local_variable;
    int param_position;

} Node;

Node *node_copy(const Node *node);
Node *node_create(elementType e_type, dataType d_type, variableType v_type,
                  short size, const char *name, int n_parameters,
                  int n_local_variables, int pos_local_variable,
                  int param_position);
Node *create_variable(dataType d_type, variableType v_type, short size,
                      const char *name, int pos_local_variable);

Node *create_parameter(dataType d_type, const char *name, int pos_parameter);

Node *create_function(dataType ret_type, const char *name, int n_parameters,
                      int n_loc_variables);

void node_free(Node *node);

#endif