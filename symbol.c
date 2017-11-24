#include "symbol.h"

variable_data *create_data_variable(Ttoken *token)
{
	variable_data *data;
	data = malloc(sizeof(struct variable_data));
	if(data == NULL) {
		fprintf(stderr, "Error allocating memory for variable_data.\n");

		return NULL;
	}

	data->name = malloc(sizeof(char *) * token->t_str.length);
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
			if(token->t_state == st_int_val) {
				data->value.value_integer = strtol(token->t_str.data, NULL, 10);
				data->inicialized = 1;
				break;
			} else {
				printf("Not correct value.\n");
				break;
			}

		case variable_double:
			if(token->t_state == st_double_val || token->t_state == st_exp_doub || token->t_state == st_exp_int) {
				data->value.value_double = strtod(token->t_str.data, NULL);
				data->inicialized = 1;
				break;
			} else {
				printf("Not corret value\n");
				break;
			}

		case variable_string:
			strcpy(data->value.value_string, token->t_str.data);
			data->inicialized = 1;
			break;

		default:
			fprintf(stderr, "Variable type not known.\n");
			break;
	}
}

void variable_data_to_table(htab_t *table, variable_data *data)
{
	htab_listitem *item;
	item = htab_lookup_add(table, data->name);

	item->type = type_variable;
	item->pointer.variable = data;
}

void free_data_variable(variable_data *data)
{
	free(data->name);
	free(data);
}

function_data *create_data_function(Ttoken *token)
{
	function_data *data;
	data = malloc(sizeof(struct variable_data));
	if(data == NULL) {
		fprintf(stderr, "Error allocating memory for function_data.\n");

		return NULL;
	}

	data->name = malloc(sizeof(char *) * token->t_str.length);
	if(data->name == NULL) {
		fprintf(stderr, "Error allocating memory for function_data->name.\n");
		free(data);

		return NULL;
	}

	strcpy(data->name, token->t_str.data);
	data->defined = 0;
	data->arguments_count = 0;
	data->arguments = NULL;
	data->local_symbol_table = NULL;

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

void set_local_symbol_table(htab_t *table, function_data *data)
{
	data->local_symbol_table = table;
}


//Nespravna alokacia pamate, opravit EDIT: opravene
int add_argument_function(function_data *data, Ttoken *token)
{
	data->arguments_count++;

	if(data->arguments_count == 1) {
		data->arguments = malloc(sizeof(function_arguments));
	} else {
		data->arguments = realloc(data->arguments, sizeof(function_arguments) * data->arguments_count);
	} 

	if(data->arguments == NULL) {
		fprintf(stderr, "Error allocating memory for argument data.\n");

		return 1;
	}

	str_create(&(data->arguments[data->arguments_count - 1].argument_name));
	str_append_str(&(data->arguments[data->arguments_count - 1].argument_name), &(token->t_str));

	return 0;
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

void function_data_to_table(htab_t *table, function_data *data)
{
	htab_listitem *item = htab_lookup_add(table, data->name);

	item->type = type_function;
	item->pointer.function = data;
}

void free_data_function(function_data *data)
{
	if(data->name != NULL) {
		free(data->name);
	}

	if(data->arguments != NULL) {
		for(unsigned i = 0; i < data->arguments_count; i++) {
			str_destroy(&(data->arguments[i].argument_name));
		}
		free(data->arguments);
	}

	if(data->local_symbol_table != NULL) {
		htab_free(data->local_symbol_table);
	}
	
	free(data);
}


function_data *create_global_data(void)
{
	function_data *data;
	data = malloc(sizeof(struct function_data));
	if(data == NULL) {
		fprintf(stderr, "Error allocating memory for global_data\n");

		return NULL;
	}

	data->name = NULL;
	data->defined = 0;
	data->arguments_count = 0;
	data->arguments = NULL;
	data->local_symbol_table = NULL;

	return data;
}


int retrieve_function_data(char *function_name)
{
	htab_listitem *item = htab_find(global_table, function_name);
	if(item == NULL) {
		fprintf(stderr, "Function not found\n");

		return 0;
	}

	global_data->defined = item->pointer.function->defined;
	global_data->return_type = item->pointer.function->return_type;
	global_data->arguments_count = item->pointer.function->arguments_count;
	global_data->arguments = item->pointer.function->arguments;
	global_data->local_symbol_table = item->pointer.function->local_symbol_table;

	free(item);
	return 1;
}

int variable_exist(char *variable_name)
{
	if((p == 1 ? global_data->local_symbol_table : global_table) == NULL) {
		fprintf(stderr, "global_data not set\n");

		return 0;
	}

	htab_listitem *item = htab_find((p == 1 ? global_data->local_symbol_table : global_table), variable_name);
	if(item == NULL) {
		fprintf(stderr, "Variable not found\n");

		return 0;
	}

	return 1;
}

int check_variable_type(char *variable_name, Tstate state)
{
	htab_listitem *item = htab_find((p == 1 ? global_data->local_symbol_table : global_table), variable_name);
	if(item == NULL) {
		fprintf(stderr, "Variable not found\n");

		return 0;
	}

	if( (item->pointer.variable->type == variable_integer && state == st_integer) ||
		(item->pointer.variable->type == variable_double && state == st_double) ||
		(item->pointer.variable->type == variable_string && state == st_string)) {
		return 1;
	} 

	return 0;
}

int check_variable_inicialized(char *variable_name)
{
	htab_listitem *item = htab_find(global_data->local_symbol_table, variable_name);
	if(item == NULL) {
		fprintf(stderr, "Variable not found\n");

		return 0;
	}

	if(item->pointer.variable->inicialized == 1) {
		return 1;
	} 
	
	return 0;
}

int check_function_return_type(Tstate state)
{
	if(global_data == NULL) {
		fprintf(stderr, "global_data not set\n");

		return 0;
	}

 	if( (global_data->return_type == 'i' && state == st_integer) ||
		(global_data->return_type == 'd' && state == st_double) ||
		(global_data->return_type == 's' && state == st_string)) {
		return 1;
	} 

	return 0;
}

int check_argument_count(unsigned count)
{
	if(global_data == NULL) {
		fprintf(stderr, "global_data not set\n");

		return 0;
	}

	if(global_data->arguments_count == count) {
		return 1;
	}

	return 0;
}

int check_argument_type(Tstate state, unsigned index)
{
	if(global_data->arguments == NULL) {
		fprintf(stderr, "global_data not set\n");

		return 0;
	}

	if( (global_data->arguments[index - 1].type == variable_integer && state == st_integer) ||
		(global_data->arguments[index - 1].type == variable_double && state == st_double) ||
		(global_data->arguments[index - 1].type == variable_string && state == st_string)) {
		return 1;
	}

	return 0;
}

int check_argument_name(char *name, unsigned index)
{
	if(global_data->arguments == NULL) {
		fprintf(stderr, "global_data not set\n");

		return 0;
	}

	if(strcmp(global_data->arguments[index - 1].argument_name.data, name) == 0) {
		return 1;
	}

	return 0;
}

int check_defined_function(void) 
{
	if(global_data == NULL) {
		fprintf(stderr, "global_data not set\n");

		return 0;
	}

	if(global_data->defined == 1) {
		return 1;
	}

	return 0;
}