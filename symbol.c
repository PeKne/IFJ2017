#include "symbol.h"

variable_data *create_data_variable(Ttoken *token)
{
	variable_data *data;
	data = malloc(sizeof(struct variable_data));
	if(data == NULL) {
		debug_print("%s\n", "Error allocating memory for variable_data.");

		return NULL;
	}

	data->name = malloc(sizeof(char *) * token->t_str.length);
	if(data->name == NULL) {
		debug_print("%s\n", "Error allocating memory for variable_data->name.");
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
			debug_print("%s\n", "Variable type not known.");
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
				debug_print("%s\n", "Not correct value.\n");
				break;
			}

		case variable_double:
			if(token->t_state == st_double_val || token->t_state == st_exp_doub || token->t_state == st_exp_int) {
				data->value.value_double = strtod(token->t_str.data, NULL);
				data->inicialized = 1;
				break;
			} else {
				debug_print("%s\n", "Not corret value\n");
				break;
			}

		case variable_string:
			str_append_str(&data->value.value_string, &token->t_str);
			data->inicialized = 1;
			break;

		default:
			debug_print("%s\n", "Variable type not known.\n");
			break;
	}
}

void variable_data_to_table(htab_t *table, variable_data *data)
{
	htab_listitem *item = htab_lookup_add(table, data->name);
	if(item == NULL) {
		debug_print("%s\n", "ERROR adding data to table\n");
		return;
	}

	item->type = type_variable;
	item->pointer.variable = data;

	//printf("v_(%s)\n", item->key);
}

void free_data_variable(variable_data *data)
{
	if(data->type == variable_string && data->inicialized == 1) {
		str_destroy(&data->value.value_string);
	}
	free(data->name);
	free(data);
}

function_data *create_data_function(Ttoken *token)
{
	function_data *data;
	data = malloc(sizeof(struct variable_data));
	if(data == NULL) {
		debug_print("%s\n", "Error allocating memory for function_data.");

		return NULL;
	}

	data->name = malloc(sizeof(char *) * token->t_str.length);
	if(data->name == NULL) {
		debug_print("%s\n", "Error allocating memory for function_data->name.");
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
			debug_print("%s\n", "Variable type not known.");
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
		debug_print("%s\n", "Error allocating memory for argument data.");

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
			debug_print("%s\n", "Variable type not known.");
			break;
	}
}

void function_data_to_table(htab_t *table, function_data *data)
{
	htab_listitem *item = htab_lookup_add(table, data->name);
	if(item == NULL) {
		debug_print("%s\n", "Error pushing func data to table");
		return;
	}

	item->type = type_function;
	item->pointer.function = data;

	//printf("f_(%s)\n", item->key);
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
		debug_print("%s\n", "Error allocating memory for global_data");

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
	htab_listitem *item;
	item = htab_find(global_table, function_name);
	if(item == NULL) {
		debug_print("%s\n", "Function not found");

		return 0;
	}

	global_data->name = item->pointer.function->name;
	global_data->defined = item->pointer.function->defined;
	global_data->return_type = item->pointer.function->return_type;
	global_data->arguments_count = item->pointer.function->arguments_count;
	global_data->arguments = item->pointer.function->arguments;
	global_data->local_symbol_table = item->pointer.function->local_symbol_table;

	return 1;
}

int push_function_data(char *function_name)
{
	htab_listitem *item = htab_lookup_add(global_table, function_name);
	if(item == NULL) {
		debug_print("%s\n", "Error allocating data");
		return 0;
	}

	item->pointer.function->name = global_data->name;
	item->pointer.function->defined = global_data->defined;
	item->pointer.function->return_type = global_data->return_type;
	item->pointer.function->arguments_count = global_data->arguments_count;
	item->pointer.function->arguments = global_data->arguments;
	item->pointer.function->local_symbol_table = global_data->local_symbol_table;

	return 1;
}

int variable_exist(char *variable_name)
{
	if((p == 1 ? global_data->local_symbol_table : global_table) == NULL) {
		debug_print("%s\n", "Global_data not set");

		return 0;
	}

	htab_listitem *item = htab_find((p == 1 ? global_data->local_symbol_table : global_table), variable_name);
	if(item == NULL) {
		debug_print("%s\n", "Variable not found");

		return 0;
	}

	return 1;
}

int check_variable_type(char *variable_name, Tstate state)
{
	htab_listitem *item = htab_find((p == 1 ? global_data->local_symbol_table : global_table), variable_name);
	if(item == NULL) {
		debug_print("%s\n", "Variable not found");
		return 0;
	}

	if( (item->pointer.variable->type == variable_integer && state == st_integer) ||
		(item->pointer.variable->type == variable_double && state == st_double) ||
		(item->pointer.variable->type == variable_string && state == st_string)) {
		return 1;
	} 

	return 0;
}

Tstate return_variable_type(char *variable_name)
{
	htab_listitem *item = htab_find((p == 1 ? global_data->local_symbol_table : global_table), variable_name);
	if(item == NULL) { // errory vypisuju ja - Petr Marek
		return 0;
	}

	switch(item->pointer.variable->type) {
		case variable_integer:
			return st_integer;

		case variable_double:
			return st_double;

		case variable_string:
			return st_string;

		default:
			return 0;
	}
}

int check_variable_inicialized(char *variable_name)
{
	htab_listitem *item = htab_find(global_data->local_symbol_table, variable_name);
	if(item == NULL) {
		debug_print("%s\n", "Variable not found");

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
		debug_print("%s\n", "Global_data not set");

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
		debug_print("%s\n", "Global_data not set");

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
		debug_print("%s\n", "Global_data not set");

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
		debug_print("%s\n", "Global_data not set");

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
		debug_print("%s\n", "Global_data not set");

		return 0;
	}

	if(global_data->defined == 1) {
		return 1;
	}

	return 0;
}

int buildin_to_table(void)
{
	Ttoken tmp_token;
	int err = 0;
	//LENGTH BUILDIN FUNCTION
	str_create_init(&(tmp_token.t_str), "Length");
	function_data *data_len = create_data_function(&tmp_token);
	set_defined_function(data_len);
	str_destroy(&(tmp_token.t_str));
	str_create_init(&(tmp_token.t_str), "s");
	//variable_data *data_len_s = create_data_variable(&tmp_token);
	if((err = add_argument_function(data_len, &tmp_token)) != 0) {
		return 1;
	}
	tmp_token.t_state = st_string;
	set_argument_type_function(data_len, &tmp_token);
	//set_type_variable(data_len_s, &tmp_token);
	/*htab_t *local_table_len = htab_init(HTAB_SIZE);
	variable_data_to_table(local_table_len, data_len_s);
	set_local_symbol_table(local_table_len, data_len);*/ //Je toto potreba ak sa kod generuje automaticky?
	tmp_token.t_state = st_integer;
	set_return_type_function(data_len, &tmp_token);
	//
	function_data_to_table(global_table, data_len);
	str_destroy(&(tmp_token.t_str));

	//SUBSTR BUILDIN FUNCTION
	str_create_init(&(tmp_token.t_str), "SubStr");
	function_data *data_substr = create_data_function(&tmp_token);
	set_defined_function(data_substr);
	str_destroy(&(tmp_token.t_str));
	str_create_init(&(tmp_token.t_str), "s");
	if((err = add_argument_function(data_substr, &tmp_token)) != 0) {
		return 1;
	}
	tmp_token.t_state = st_string;
	set_argument_type_function(data_substr, &tmp_token);
	str_destroy(&(tmp_token.t_str));
	str_create_init(&(tmp_token.t_str), "i");
	if((err = add_argument_function(data_substr, &tmp_token)) != 0) {
		return 1;
	}
	tmp_token.t_state = st_integer;
	set_argument_type_function(data_substr, &tmp_token);
	str_destroy(&(tmp_token.t_str));
	str_create_init(&(tmp_token.t_str), "n");
	if((err = add_argument_function(data_substr, &tmp_token)) != 0) {
		return 1;
	}
	tmp_token.t_state = st_integer;
	set_argument_type_function(data_substr, &tmp_token);

	tmp_token.t_state = st_string;
	set_return_type_function(data_substr, &tmp_token);
	//
	function_data_to_table(global_table, data_substr);
	str_destroy(&(tmp_token.t_str));

	//ASC BUILDIN FUNCTION
	str_create_init(&(tmp_token.t_str), "Asc");
	function_data *data_asc = create_data_function(&tmp_token);
	set_defined_function(data_asc);
	str_destroy(&(tmp_token.t_str));
	str_create_init(&(tmp_token.t_str), "s");
	if((err = add_argument_function(data_asc, &tmp_token)) != 0) {
		return 1;
	}
	tmp_token.t_state = st_string;
	set_argument_type_function(data_asc, &tmp_token);
	str_destroy(&(tmp_token.t_str));
	str_create_init(&(tmp_token.t_str), "i");
	if((err = add_argument_function(data_asc, &tmp_token)) != 0) {
		return 1;
	}
	tmp_token.t_state = st_integer;
	set_argument_type_function(data_asc, &tmp_token);
	//
	tmp_token.t_state = st_integer;
	set_return_type_function(data_asc, &tmp_token);
	//
	function_data_to_table(global_table, data_asc);
	str_destroy(&(tmp_token.t_str));

	//CHR BUILDIN FUNCTION
	str_create_init(&(tmp_token.t_str), "Chr");
	function_data *data_chr = create_data_function(&tmp_token);
	set_defined_function(data_chr);
	str_destroy(&(tmp_token.t_str));
	str_create_init(&(tmp_token.t_str), "i");
	//variable_data *data_len_s = create_data_variable(&tmp_token);
	if((err = add_argument_function(data_chr, &tmp_token)) != 0) {
		return 1;
	}
	tmp_token.t_state = st_integer;
	set_argument_type_function(data_chr, &tmp_token);
	//
	tmp_token.t_state = st_integer;
	set_return_type_function(data_chr, &tmp_token);
	//
	function_data_to_table(global_table, data_chr);
	str_destroy(&(tmp_token.t_str));

	return 0;
}