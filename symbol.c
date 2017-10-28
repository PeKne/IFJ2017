#include "symbol.h"

void variable_to_symbol_table(htab_listitem *item, char *variable_name) {
	item->type = type_variable;

	unsigned key_length = strlen(variable_name) + 1;

	item->pointer.variable = malloc(sizeof(variable_data));
	item->pointer.variable->name = malloc(sizeof(char) * key_length);

	strcpy(item->pointer.variable->name, variable_name);
	item->pointer.variable->initialized = 0;
}

void function_to_symbol_table(htab_listitem *ite, char *function_name) {
	item->type = type_function;

	unsigned key_length = strlen(function_name) + 1;

	item->ptr.function = malloc(sizeof(function_data));
	item->pointer.variable->name = malloc(sizeof(char) * key_length);

	strcpy(item->pointer.variable->name, function_name);
	item->pointer.function->defined = 0;
	item->pointer.function->arguments_count = 0;
	item->pointer.function->arguments = NULL;
}