#include "symbol.h"

void variable_to_symbol_table(htab_listitem *item, char *variable_name) {
	item->type = type_variable;

	unsigned key_length = strlen(variable_name) + 1;

	item->pointer.variable = malloc(sizeof(variable_data));
	item->pointer.variable->name = malloc(sizeof(char) * key_length);

	strcpy(item->pointer.variable->name, variable_name);
	item->pointer.variable->initialized = 0;
}

void function_to_symbol_table(htab_listitem *item, char *function_name) {
	item->type = type_function;

	unsigned key_length = strlen(function_name) + 1;

	item->ptr.function = malloc(sizeof(function_data));
	item->pointer.variable->name = malloc(sizeof(char) * key_length);

	strcpy(item->pointer.variable->name, function_name);
	item->pointer.function->defined = 0;
	item->pointer.function->arguments_count = -1;
	item->pointer.function->arguments = NULL;
}

variable_data *create_variable(Ttoken *token) {
	variable_data *data = malloc(sizeof(struct variable_data));
	if(data == NULL) {
		return NULL;
	}

	data->name = malloc(sizeof(char) * (strlen(token->data) + 1));
	if(data->name == NULL) {
		free(data);
		return NULL;
	}

	strcpy(data->name, token->data);
	data->initialized = 0;

	return data;
}

void set_variable_type(variable_data *data, Ttoken *token) {
	switch(token->stav) {
		case st_integer:
			data->type = variable_integer;
			break;

		case st_double:
			data->type = variable_double;
			break;

		case st_string:
			data->type = variable_string;
			break;

		default:
			fprintf(stderr, "Non acceptable variable type.");
			break;
	}
}

void set_variable_value(variable_data *data, Ttoken *token) {
	switch(data->type) {
		case variable_integer:
			data->value.value_integer = atoi(token->data);
			data->initialized = 1;
			break;

		case variable_double:
			data->value.value_double = atof(token->data);
			data->initialized = 1;
			break;

		case variable_string:
			strcpy(data->value.value_string, token->data);
			data->initialized = 1;
			break;

		default:
			break;
	}
}

function_data* create_variable(Ttoken *token) {
	function_data *data = malloc(sizeof(struct function_data));
	if(data == NULL) {
		return NULL;
	}

	data->name = malloc(sizeof(char) * (strlen(token->data) + 1));
	if(data->name == NULL) {
		free(data);
		return NULL;
	}

	strcpy(data->name, token->data);
	data->defined = 0;
	data->arguments_count = -1;
	arguments_types = NULL;
	local_symbol_table = NULL;
}

void raise_function_argument_count(function_data *data) {
	data->arguments_count++;
}

void set_function_local_table(function_data *data, htab_t *local_table) {
	data->local_symbol_table = local_table;
}

void set_function_return_type(function_data *data, Ttoken *token) {
	data->return_type = token->data[0];

	/* switch(token->stav) {
			case st_integer:
				data->return_type = 'i';
				break;

			case st_double:
				data->return_type = 'd';
				break;

			case st_string:
				data->return_type = 's';
				break;

			default:
				fprintf(stderr, "Non acceptable return type");
				break;
	} */
}

void set_function_arguments(function_data *data, Ttoken *token) {

}