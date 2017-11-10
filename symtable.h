#ifndef SYMTABLE_H
#define SYMTABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include "symbol.h"

typedef struct htab_listitem htab_listitem;
typedef struct htab_t htab_t;

typedef enum {
	type_variable,
	type_function,
} item_type;

struct htab_listitem {
	struct htab_listitem *next;
	item_type type;
	union
	{
		struct variable_data *variable;
		struct function_data *function;
	} pointer;
	char *key;
};

struct htab_t {
	unsigned size;
	struct htab_listitem *list[];
};

unsigned hash_function(const char *str);

htab_t *htab_init(unsigned size);

htab_t *htab_copy(htab_t *table);

htab_listitem *htab_lookup_add(htab_t *table, const char *key);

htab_listitem *htab_find(htab_t *table, const char *key);

void htab_foreach(htab_t *table, void (*function)(const char *key, item_type type, htab_listitem *item));

void htab_remove(htab_t *table, const char *key);

void htab_clear(htab_t *table);

void htab_free(htab_t *table);

#endif
