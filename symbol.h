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
	char return_type;
	char *arguments_types;
	htab_t *local_symbol_table;
	char *name;
} function_data;

void variable_to_symbol_table(htab_listitem *item, char *variale_name);
void function_to_symbol_table(htab_listitem *item, char *function_name);

variable_data* create_variable(Ttoken *token);
void set_variable_type(variable_data *data, Ttoken *token);
void set_variable_value(variable_data *data, Ttoken *token);

function_data* create_function(Ttoken *token);
void raise_function_argument_count(function_data *data);
void set_function_local_table(function_data *data, htab_t *table);
void set_function_return_type(function_data *data, Ttoken *token);
void set_function_arguments(function_data *data, Ttoken *token); 