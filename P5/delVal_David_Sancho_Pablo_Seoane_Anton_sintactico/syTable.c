#include "syTable.h"
#include "ht.h"
#include "tableNode.h"
#include <stdio.h>
#include <stdlib.h>

struct _syTable {
    ht *global;
    ht *local;
};

void free_wrap(void *var) { node_free((Node *) var); }

syTable *syTable_create() {
    syTable *st = calloc(1, sizeof(syTable));
    if (st == NULL) {
        return NULL;
    }
    st->global = ht_create(free_wrap);
    if (st->global == NULL) {
        free(st);
        return NULL;
    }
    return st;
}

void syTable_destroy(syTable *table) {
    ht_destroy(table->global);
    ht_destroy(table->local);
    free(table);
}

static bool syTable_insert_into_table(ht *table, Node node) {

    if (ht_present(table, node.name)) {
        return false;
    }
    Node *new_node = node_copy(&node);
    if (!new_node) {
        node_free(new_node);
    }
    if (ht_set(table, new_node->name, new_node)) {
        return true;
    } else {
        node_free(new_node);
        return false;
    }
}

static bool syTable_insert_global(syTable *st, Node node) {
    return syTable_insert_into_table(st->global, node);
}

static bool syTable_insert_local(syTable *st, Node node) {
    return syTable_insert_into_table(st->local, node);
}

static bool syTable_pop_global(syTable *st, Node node) {
    return ht_pop(st->global, node.name);
}

bool syTable_insert(syTable *st, Node node) {
    if (st->local) {
        bool status = syTable_insert_local(st, node);
        return status;
    }
    bool status = syTable_insert_global(st, node);
    return status;
}

bool syTable_create_scope(syTable *st, Node node) {
    if (st->local) {
        return false;
    }
    if (!syTable_insert_global(st, node)) {
        return false;
    }
    st->local = ht_create(free_wrap);
    if (!st->local) {
        // undo actions
        syTable_pop_global(st, node);
        return false;
    }
    if (!syTable_insert_local(st, node)) {
        // undo actions
        ht_destroy(st->local);
        syTable_pop_global(st, node);
        return false;
    }
    return true;
}

bool syTable_close_scope(syTable *st) {
    if (st->local) {
        ht_destroy(st->local);
        st->local = NULL;
        return true;
    }
    return false;
}

const Node *syTable_search(syTable *st, const char *name) {
    if (st->local) {
        Node *loc = ht_get(st->local, name);
        if (loc) {
            return loc;
        }
    }
    return ht_get(st->global, name);
}

const Node *syTable_duplicated(syTable *st, const char *name) {
    if (st->local) {
        return ht_get(st->local, name);
    }
    return ht_get(st->global, name);
}

void syTable_setFunctionValues(syTable *st, const char *name, int n_params, int n_local_vars) {
    if (st->local) {
        Node *n1 = ht_get(st->local, name);
        if (n1) {
            n1->n_parameters = n_params;
            n1->n_local_variables = n_local_vars;
        }
    }
    Node *n2 = ht_get(st->global, name);
    if (n2) {
        n2->n_parameters = n_params;
        n2->n_local_variables = n_local_vars;
    }
}
