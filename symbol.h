#ifndef SYMBOL_H
#define SYMBOL_H

#include "strings.h"
#include "lex.h"
#include "symtable.h"

/* NIEKTORE FUNKCIE SU ZAKOMENTOVANIE Z DOVODU NEDORIESENEHO LINKOVANIA */

typedef enum {
	variable_integer,
	variable_double,
	variable_string,
} variable_type;

typedef struct {
	Tstring *argument_name;
	variable_type type;
} function_arguments;

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
	function_arguments *arguments;
	//htab_t *local_symbol_table;
	char *name;
} function_data;

variable_data *create_data_variable(Ttoken *token);

void set_type_variable(variable_data *data, Ttoken *token);

void set_value_variable(variable_data *data, Ttoken *token);

//void variable_data_to_table(htab_t *table, variable_data *data);

function_data *create_data_function(Ttoken *token);

void set_defined_function(function_data *data);

void set_return_type_function(function_data *data, Ttoken *token);

//void set_local_symbol_table(htab_t *table, function_data *data);

void add_argument_function(function_data *data, Ttoken *token);

void set_argument_type_function(function_data *data, Ttoken *token);

//void function_data_to_table(htab_t *table, function_data *data);

#endif
