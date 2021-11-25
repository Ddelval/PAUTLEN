#ifndef _SY_TABLE_H
#define _SY_TABLE_H

#include "ht.h"
typedef struct _syTable syTable;

syTable *syTable_create();
void syTable_destroy(syTable *table);
bool syTable_insert(syTable *st, const char *name, int val);
bool syTable_create_scope(syTable *st, const char *name,int arg);
bool syTable_close_scope(syTable *st);
const int *syTable_search(syTable *st, const char *name);
#endif