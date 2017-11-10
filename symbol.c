#include "symbol.h"

variable_data *create_data_variable(Ttoken *token)
{
	variable_data *data = malloc(sizeof(struct variable_data));
	if(data == NULL) {
		fprintf(stderr, "Error allocating memory for variable_data.\n");

		return NULL;
	}

	data->name = malloc(sizeof(char) * token->t_str.length);
	if(data->name == NULL) {
		fprintf(stderr, "Error allocating memory for variable_data->name.\n");
		free(data);

		return NULL;
	}

	strcpy(data->name, token->t_str.data);
	data->inicialized = 0;

	return data;
}

void set_type_variable(variable_data *data, Ttoken *token)
{
	switch(token->t_state) {
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
			fprintf(stderr, "Variable type not known.\n");
			break;
	}
}

void set_value_variable(variable_data *data, Ttoken *token)
{
	switch(data->type) {
		case variable_integer:
			data->value.value_integer = atoi(token->t_str.data);
			data->inicialized = 1;
			break;

		case variable_double:
			data->value.value_double = atof(token->t_str.data);
			data->inicialized = 1;
			break;

		case variable_string:
			strcpy(data->value.value_string, token->t_str.data);
			data->inicialized = 1;
			break;

		default:
			fprintf(stderr, "Variable type not known.\n");
			break;
	}
}
/*
void variable_data_to_table(htab_t *table, variable_data *data)
{
	htab_listitem *item = htab_lookup_add(table, data->name);

	item->type = type_variable;
	item->pointer.variable = data;
}
*/
function_data *create_data_function(Ttoken *token)
{
	function_data *data = malloc(sizeof(struct variable_data));
	if(data == NULL) {
		fprintf(stderr, "Error allocating memory for function_data.\n");

		return NULL;
	}

	data->name = malloc(sizeof(char) * token->t_str.length);
	if(data->name == NULL) {
		fprintf(stderr, "Error allocating memory for function_data->name.\n");
		free(data);

		return NULL;
	}

	strcpy(data->name, token->t_str.data);
	data->defined = 0;
	data->arguments_count = 0;
	data->arguments = NULL;
	//data->local_symbol_table = NULL;

	return data;
}

void set_defined_function(function_data *data)
{
	data->defined = 1;
}

void set_return_type_function(function_data *data, Ttoken *token)
{
	switch(token->t_state) {
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
			fprintf(stderr, "Variable type not known.\n");
			break;
	}
}
/*
void set_local_symbol_table(htab_t *table, function_data *data)
{
	data->local_symbol_table = table;
}
*/

//Nespravna alokacia pamate, opravit
void add_argument_function(function_data *data, Ttoken *token)
{
	data->arguments_count++;

	if(data->arguments_count == 1) {
		data->arguments = malloc(sizeof(function_arguments));
	} else {
		data->arguments = realloc(data->arguments, sizeof(function_arguments) * data->arguments_count);
	}

	if(data->arguments == NULL) {
		fprintf(stderr, "Error allocating memory for argument data.\n");
	}

	Tstring string;
	str_create_init(&string, token->t_str.data);

	strcpy(data->arguments[data->arguments_count - 1].argument_name->data, string.data);
} 

void set_argument_type_function(function_data *data, Ttoken *token)
{
	switch(token->t_state) {
		case st_integer:
			data->arguments[data->arguments_count - 1].type = variable_integer;
			break;

		case st_double:
			data->arguments[data->arguments_count - 1].type = variable_double;
			break;

		case st_string:
			data->arguments[data->arguments_count - 1].type = variable_string;
			break;

		default:
			fprintf(stderr, "Variable type not known.\n");
			break;
	}
}
/*
void function_data_to_table(htab_t *table, function_data *data)
{
	htab_listitem *item;
	item = htab_lookup_add(table, data->name);

	item->type = type_function;
	item->pointer.function = data;
}
*/
