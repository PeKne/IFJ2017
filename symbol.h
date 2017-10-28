#include "symtable.h"

typedef enum {
	variable_integer,
	variable_double,
	variable_string;
} variable_type;

typedef struct variable_data {
	int inicialized;
	variable_type type;
	union {
		int value_integer;
		double value_double;
		char value_string[256];
	} value;
	char *name;
} variable_data;

typedef struct function_data {
	int defined;
	unsigned arguments_count;
	char *arguments;
	htab_t *local_symbol_table;
	char *name;
} function_data;

void variable_to_symbol_table(htab_listitem *item, char *key);
void function_to_symbol_table(htab_listitem *item, char *function_name);