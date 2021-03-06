// Simple hash table implemented in C.

#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "ht.h"

// Hash table entry (slot may be filled or empty).
typedef struct {
    const char *key; // key is NULL if this slot is empty
    void *value;
    int marked;
} ht_entry;

// Hash table structure: create with ht_create, free with ht_destroy.
struct ht {
    ht_entry *entries; // hash slots
    size_t capacity;   // size of _entries array
    size_t length;     // number of items in hash table
    size_t marked_count;
    free_function free_element;
};
typedef unsigned long long ull;

#define INITIAL_CAPACITY 16 // must not be zero

ht *ht_create(free_function fun) {
    // Allocate space for hash table struct.
    ht *table = (ht *)malloc(sizeof(ht));
    if (table == NULL) {
        return NULL;
    }
    table->length = 0;
    table->marked_count = 0;
    table->capacity = INITIAL_CAPACITY;
    table->free_element = fun;

    // Allocate (zero'd) space for entry buckets.
    table->entries = (ht_entry *)calloc(table->capacity, sizeof(ht_entry));
    if (table->entries == NULL) {
        free(table); // error, free table before we return!
        return NULL;
    }
    return table;
}

void ht_destroy(ht *table) {
    if (!table) return;
    // First free allocated keys.
    for (size_t i = 0; i < table->capacity; i++) {
        if (table->entries[i].key != NULL) {
            free((void *)table->entries[i].key);
            table->free_element(table->entries[i].value);
        }
    }

    // Then free entries array and table itself.
    free(table->entries);
    free(table);
}

#define FNV_OFFSET 14695981039346656037UL
#define FNV_PRIME 1099511628211UL

// Return 64-bit FNV-1a hash for key (NUL-terminated). See description:
// https://en.wikipedia.org/wiki/Fowler–Noll–Vo_hash_function
static ull hash_key(const char *key) {
    ull hash = FNV_OFFSET;
    for (const char *p = key; *p; p++) {
        hash ^= (ull)(unsigned char)(*p);
        hash *= FNV_PRIME;
    }
    return hash;
}

void *ht_get(ht *table, const char *key) {
    // AND hash with capacity-1 to ensure it's within entries array.
    ull hash = hash_key(key);
    size_t index = (size_t)(hash & (ull)(table->capacity - 1));

    // Loop till we find an empty entry.
    while (table->entries[index].key != NULL || table->entries[index].marked) {
        if (table->entries[index].key &&
            strcmp(key, table->entries[index].key) == 0) {
            // Found key, return value.
            return table->entries[index].value;
        }
        // Key wasn't in this slot, move to next (linear probing).
        index++;
        if (index >= table->capacity) {
            // At end of entries array, wrap around.
            index = 0;
        }
    }
    return NULL;
}

bool ht_present(ht *table, const char *key) {
    // AND hash with capacity-1 to ensure it's within entries array.
    ull hash = hash_key(key);
    size_t index = (size_t)(hash & (ull)(table->capacity - 1));

    // Loop till we find an empty entry.
    while (table->entries[index].key != NULL || table->entries[index].marked) {

        if (table->entries[index].key &&
            strcmp(key, table->entries[index].key) == 0) {
            // Found key, return value.
            return true;
        }
        // Key wasn't in this slot, move to next (linear probing).
        index++;
        if (index >= table->capacity) {
            // At end of entries array, wrap around.
            index = 0;
        }
    }

    return false;
}

bool ht_pop(ht *table, const char *key) {
    ht_entry *entries = table->entries;
    size_t capacity = table->capacity;
    ull hash = hash_key(key);
    size_t index = (size_t)(hash & (ull)(capacity - 1));

    // Loop till we find an empty entry.
    while (entries[index].key != NULL || table->entries[index].marked) {
        if (entries[index].key && strcmp(key, entries[index].key) == 0) {
            // Found key (it already exists), update value.
            free((void *)table->entries[index].key);
            table->free_element(table->entries[index].value);
            entries[index].key = NULL;
            entries[index].marked = true;
            table->marked_count++;
            return true;
        }
        // Key wasn't in this slot, move to next (linear probing).
        index++;
        if (index >= capacity) {
            // At end of entries array, wrap around.
            index = 0;
        }
    }
    return false;
}

// Internal function to set an entry (without expanding table).
static const char *ht_set_entry(ht *table, ht_entry *entries, size_t capacity,
                                const char *key, void *value, size_t *plength) {
    // AND hash with capacity-1 to ensure it's within entries array.
    ull hash = hash_key(key);
    size_t index = (size_t)(hash & (ull)(capacity - 1));

    // Loop till we find an empty entry.
    while (entries[index].key != NULL || entries[index].marked) {
        if (entries[index].key && strcmp(key, entries[index].key) == 0) {
            // Found key (it already exists), update value.
            table->free_element(entries[index].value);
            entries[index].value = value;
            return entries[index].key;
        }
        // Key wasn't in this slot, move to next (linear probing).
        index++;
        if (index >= capacity) {
            // At end of entries array, wrap around.
            index = 0;
        }
    }

    // Didn't find key, allocate+copy if needed, then insert it.
    if (plength != NULL) {
        key = strdup(key);
        if (key == NULL) {
            return NULL;
        }
        (*plength)++;
    }

    entries[index].key = (char *)key;
    entries[index].value = value;
    return key;
}

// Expand hash table to twice its current size. Return true on success,
// false if out of memory.
static int ht_expand(ht *table) {
    // Allocate new entries array.
    size_t new_capacity = table->capacity * 2;
    if (new_capacity < table->capacity) {
        return false; // overflow (capacity would be too big)
    }
    ht_entry *new_entries = (ht_entry *)calloc(new_capacity, sizeof(ht_entry));
    if (new_entries == NULL) {
        return false;
    }

    // Iterate entries, move all non-empty ones to new table's entries.
    for (size_t i = 0; i < table->capacity; i++) {
        ht_entry entry = table->entries[i];
        if (entry.key != NULL) {
            ht_set_entry(table, new_entries, new_capacity, entry.key,
                         entry.value, NULL);
        }
    }

    // Free old entries array and update this table's details.
    free(table->entries);
    table->entries = new_entries;
    table->capacity = new_capacity;
    return true;
}

static int ht_rehash(ht *table) {
    // Allocate new entries array.
    size_t new_capacity = table->capacity;
    ht_entry *new_entries = (ht_entry *)calloc(new_capacity, sizeof(ht_entry));
    if (new_entries == NULL) {
        return false;
    }

    // Iterate entries, move all non-empty ones to new table's entries.
    for (size_t i = 0; i < table->capacity; i++) {
        ht_entry entry = table->entries[i];
        if (entry.key != NULL) {
            ht_set_entry(table, new_entries, new_capacity, entry.key,
                         entry.value, NULL);
        }
    }

    // Free old entries array and update this table's details.
    free(table->entries);
    table->entries = new_entries;
    table->capacity = new_capacity;
    return true;
}

const char *ht_set(ht *table, const char *key, void *value) {
    assert(value != NULL);
    if (value == NULL) {
        return NULL;
    }

    // If length will exceed half of current capacity, expand it.
    if (table->length >= table->capacity / 2) {
        if (!ht_expand(table)) {
            return NULL;
        }
    }
    if (table->length + table->marked_count >= 0.7 * table->capacity) {
        if (!ht_rehash(table)) {
            return NULL;
        }
        table->marked_count = 0;
    }

    // Set entry and update length.
    return ht_set_entry(table, table->entries, table->capacity, key, value,
                        &table->length);
}

size_t ht_length(ht *table) { return table->length; }

hti ht_iterator(ht *table) {
    hti it;
    it._table = table;
    it._index = 0;
    return it;
}

bool ht_next(hti *it) {
    // Loop till we've hit end of entries array.
    ht *table = it->_table;
    while (it->_index < table->capacity) {
        size_t i = it->_index;
        it->_index++;
        if (table->entries[i].key != NULL) {
            // Found next non-empty item, update iterator key and value.
            ht_entry entry = table->entries[i];
            it->key = entry.key;
            it->value = entry.value;
            return true;
        }
    }
    return false;
}