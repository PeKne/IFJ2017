#include "symbol.h"
#include "garbage.h"

extern int p;

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
			fprintf(stderr, "Variable type not known.");
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
				fprintf(stderr, "Not correct value.\n");
				break;
			}

		case variable_double:
			if(token->t_state == st_double_val || token->t_state == st_exp_doub || token->t_state == st_exp_int) {
				data->value.value_double = strtod(token->t_str.data, NULL);
				data->inicialized = 1;
				break;
			} else {
				fprintf(stderr, "Not corret value\n");
				break;
			}

		case variable_string:
			str_append_str(&data->value.value_string, &token->t_str);
			data->inicialized = 1;
			break;

		default:
			fprintf(stderr, "Variable type not known.\n");
			break;
	}
}

void free_data_variable(variable_data *data)
{
	if(data->type == variable_string && data->inicialized == 1) {
		str_destroy(&data->value.value_string);
	}
	g_free(data->name);
	g_free(data);
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
			fprintf(stderr, "Variable type not known.");
			break;
	}
}

int add_argument_function(function_data *data, Ttoken *token)
{
	data->arguments_count++;

	if(data->arguments_count == 1) {
		data->arguments = g_malloc(sizeof(function_arguments));
	} else {
		data->arguments = g_realloc(data->arguments, sizeof(function_arguments) * data->arguments_count);
	} 

	if(data->arguments == NULL) {
		fprintf(stderr, "Error allocating memory for argument data.");

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
			fprintf(stderr, "Variable type not known.");
			break;
	}
}


void free_data_function(function_data *data)
{
	if(data->name != NULL) {
		g_free(data->name);
	}

	if(data->arguments != NULL) {
		for(unsigned i = 0; i < data->arguments_count; i++) {
			str_destroy(&(data->arguments[i].argument_name));
		}
		g_free(data->arguments);
	}

	if(data->local_symbol_table != NULL) {
		htab_free(data->local_symbol_table);
	}
	
	g_free(data);
}

int variable_exist(char *variable_name)
{
	if((p == 1 ? global.local_sym : global.global_table) == NULL) {
		fprintf(stderr, "global table not set\n");

		return 0;
	}

	htab_listitem *item;
	item = htab_find((p == 1 ? global.local_sym : global.global_table), variable_name);
	if(item == NULL) {
		fprintf(stderr, "Variable not found\n");

		return 0;
	}

	return 1;
}

int check_variable_type(char *variable_name, Tstate state)
{
	htab_listitem *item;
	item = htab_find((p == 1 ? global.local_sym : global.global_table), variable_name);
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

Tstate return_variable_type(char *variable_name)
{
	htab_listitem *item;
	item = htab_find((p == 1 ? global.local_sym : global.global_table), variable_name);
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

Tstate return_function_type(void)
{
	if(global.current_func_name == NULL) {
		fprintf(stderr, "global.current_func_name not set\n");

		return 0;
	}

	htab_listitem *item;
	item = htab_find(global.global_table, global.current_func_name);
	if(item == NULL) {
		fprintf(stderr, "Func not found\n");

		return 0;
	}

	switch(item->pointer.function->return_type) {
		case 'i':
			return st_integer;
		case 'd':
			return st_double;
		case 's':
			return st_string;
		default:
			return 0;
	}
}

int check_variable_inicialized(char *variable_name)
{
	htab_listitem *item;
	item = htab_find((p == 1 ? global.local_sym : global.global_table), variable_name);
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
	if(global.current_func_name == NULL) {
		fprintf(stderr, "global.current_func_name not set\n");

		return 0;
	}

	htab_listitem *item;
	item = htab_find(global.global_table, global.current_func_name);
	if(item == NULL) {
		fprintf(stderr, "Func not found\n");

		return 0;
	}

 	if( (item->pointer.function->return_type == 'i' && state == st_integer) ||
		(item->pointer.function->return_type == 'd' && state == st_double) ||
		(item->pointer.function->return_type == 's' && state == st_string)) {
		return 1;
	} 

	return 0;
}

int check_argument_count(unsigned count)
{
	if(global.current_func_name == NULL) {
		fprintf(stderr, "global.current_func_name not set\n");

		return 0;
	}

	htab_listitem *item;
	item = htab_find(global.global_table, global.current_func_name);
	if(item == NULL) {
		fprintf(stderr, "Func not found");

		return 0;
	}

	if(item->pointer.function->arguments_count == count) {
		return 1;
	}

	return 0;
}

int check_argument_type(Tstate state, unsigned index)
{
	if(global.current_arguments == NULL) {
		fprintf(stderr, "global.current_arguments not set\n");

		return 0;
	}

	if( (global.current_arguments[index - 1].type == variable_integer && state == st_integer) ||
		(global.current_arguments[index - 1].type == variable_double && state == st_double) ||
		(global.current_arguments[index - 1].type == variable_string && state == st_string)) {
		return 1;
	}

	return 0;
}

int check_argument_name(char *name, unsigned index)
{
	if(global.current_arguments == NULL) {
		fprintf(stderr, "global.current_arguments not set.\n");

		return 0;
	}

	if(strcmp(global.current_arguments[index - 1].argument_name.data, name) == 0) {
		return 1;
	}

	return 0;
}

int check_defined_function(void) 
{
	if(global.current_func_name == NULL) {
		fprintf(stderr, "global.current_func_name not set\b");

		return 0;
	}

	htab_listitem *item;
	item = htab_find(global.global_table, global.current_func_name);
	if(item == NULL) {
		fprintf(stderr, "Func not found\n");

		return 0;
	}

	if(item->pointer.function->defined == 1) {
		return 1;
	}

	return 0;
}
/*
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
	set_local_symbol_table(local_table_len, data_len);*
	tmp_token.t_state = st_integer;
	set_return_type_function(data_len, &tmp_token);
	//
	function_data_to_table(global.global_table, data_len);
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
	function_data_to_table(global.global_table, data_substr);
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
	function_data_to_table(global.global_table, data_asc);
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
	function_data_to_table(global.global_table, data_chr);
	str_destroy(&(tmp_token.t_str));

	return 0;
}*/

void variable_init(htab_listitem *item, char *variable_name)
{
	item->type = type_variable;

	item->pointer.variable = g_malloc(sizeof(variable_data));
	item->pointer.variable->name = g_malloc(sizeof(char *) * (strlen(variable_name) + 1));

	strcpy(item->pointer.variable->name, variable_name);
	item->pointer.variable->inicialized = 0;
}

void function_init(htab_listitem *item, char *function_name)
{
	item->type = type_function;

	item->pointer.function = g_malloc(sizeof(function_data));
	item->pointer.function->name = g_malloc(sizeof(char *) * (strlen(function_name) + 1));

	strcpy(item->pointer.function->name, function_name);
	item->pointer.function->arguments_count = 0;
	item->pointer.function->arguments = NULL;
	item->pointer.function->defined = 0;
}