#ifndef _SY_TABLE_H
#define _SY_TABLE_H

#include "ht.h"
#include "tableNode.h"
typedef struct _syTable syTable;

syTable *syTable_create();
void syTable_destroy(syTable *table);
bool syTable_insert(syTable *st, Node node);
bool syTable_create_scope(syTable *st, Node node);
bool syTable_close_scope(syTable *st);
const Node *syTable_search(syTable *st, const char * name);
const Node *syTable_duplicated(syTable *st, const char * name);
void syTable_setFunctionValues(syTable *st, const char *name, int n_params, int n_local_vars);
#endif