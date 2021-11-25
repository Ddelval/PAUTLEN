#include "syTable.h"
#include "ht.h"
#include <stdio.h>
#include <stdlib.h>
struct _syTable {
    ht *global;
    ht *local;
};

void free_element(void *element) { free(element); }

syTable *syTable_create() {
    syTable *st = calloc(1, sizeof(syTable));
    if (st == NULL) {
        return NULL;
    }
    st->global = ht_create(free_element);
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
static bool syTable_insert_into_table(ht *table, const char *name, int val) {
    if (ht_present(table, name)) {
        return false;
    }
    int *elem = calloc(1, sizeof(int));
    if (!elem) {
        return false;
    }
    *elem = val;
    if (ht_set(table, name, elem)) {
        return true;
    } else {
        free(elem);
        return false;
    }
}
static bool syTable_insert_global(syTable *st, const char *name, int val) {
    return syTable_insert_into_table(st->global, name, val);
}
static bool syTable_insert_local(syTable *st, const char *name, int val) {
    return syTable_insert_into_table(st->local, name, val);
}
bool syTable_insert(syTable *st, const char *name, int val) {
    if (st->local) {
        return syTable_insert_local(st, name, val);
    }
    return syTable_insert_global(st, name, val);
}

/*
static bool syTable_pop_local(syTable *st, const char *name) {
    return ht_pop(st->local, name);
}
*/

static bool syTable_pop_global(syTable *st, const char *name) {
    return ht_pop(st->global, name);
}

bool syTable_create_scope(syTable *st, const char *name, int arg) {
    if (st->local) {
        return false;
    }
    if (!syTable_insert_global(st, name, arg)) {
        return false;
    }
    st->local = ht_create(free_element);
    if (!st->local) {
        // undo actions
        syTable_pop_global(st, name);
        return false;
    }
    if (!syTable_insert_local(st, name, arg)) {
        // undo actions
        ht_destroy(st->local);
        syTable_pop_global(st, name);
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

const int *syTable_search(syTable *st, const char *name) {
    if (st->local) {
        int *loc = ht_get(st->local, name);
        if (loc) {
            return loc;
        }
    }
    return ht_get(st->global, name);
}
