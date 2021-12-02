#include "tableNode.h"
#include <stdlib.h>

Node *node_create(elementType e_type, dataType d_type, variableType v_type,
                  short size, const char *name, int n_parameters,
                  int n_local_variables, int pos_local_variable,
                  int param_position) {
    Node *node = (Node *)calloc(1, sizeof(Node));
    if (node == NULL) {
        return NULL;
    }
    node->name = strdup(name);
    if (node->name == NULL) {
        free(node);
        return NULL;
    }

    node->type = e_type;
    node->data_type = d_type;
    node->variable_type = v_type;
    node->size = size;
    node->n_parameters = n_parameters;
    node->n_local_variables = n_local_variables;
    node->pos_local_variable = pos_local_variable;
    node->param_position = param_position;

    return node;
}
Node *node_copy(const Node *node) {
    return node_create(node->type, node->data_type, node->variable_type,
                       node->size, node->name, node->n_parameters,
                       node->n_local_variables, node->pos_local_variable,
                       node->param_position);
}

Node *create_variable(dataType d_type, variableType v_type, short size,
                      const char *name, int pos_local_variable) {
    return create_node(VARIABLE, d_type, v_type, size, name, -1, -1,
                       pos_local_variable, -1);
}

Node *create_parameter(dataType d_type, const char *name, int pos_parameter) {
    return create_node(PARAMETRO, d_type, SCALAR, 1, name, -1, -1, -1,
                       pos_parameter);
}

Node *create_function(dataType ret_type, const char *name, int n_parameters,
                      int n_loc_variables) {
    return create_node(FUNCION, ret_type, SCALAR, 1, name, n_parameters,
                       n_loc_variables, -1, -1);
}

void node_free(Node *node) {
    if (!node) {
        return;
    }
    free(node->name);
    free(node);
}
