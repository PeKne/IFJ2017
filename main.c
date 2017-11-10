#include <stdio.h>
#include <stdlib.h>
#include "strings.h"
#include "symbol.h"
#include "symtable.h"
#include "lex.h"


//TEST
int main(void)
{
	printf("TEST 1 - tvorba struktury pre data premennej\n");
	str_create(&(token.t_str));
	str_add_char(&(token.t_str), 'a');

	variable_data *data;
	data = create_data_variable(&token);
	if(data == NULL) {
		fprintf(stderr, "Error allocating memory\n");
		return -1;
	}

	printf("name: %s ... init: %d\n", data->name, data->inicialized);

	str_destroy(&(token.t_str));

	printf("TEST 2 - nastavenie datoveho typu premennej ... v pripade SUCCESS nezahlasi nic, pri chybnom datovom type na strderr\n");
	token.t_state = st_integer;
	set_type_variable(data, &token);

	token.t_state = st_double;
	set_type_variable(data, &token);

	token.t_state = st_string;
	set_type_variable(data, &token);

	token.t_state = st_del;
	set_type_variable(data, &token);

	printf("TEST 3 - nastavenie hodnoty premennej\n");
	//1
	token.t_state = st_integer;
	set_type_variable(data, &token);

	str_create(&(token.t_str));
	str_add_char(&(token.t_str), '1');

	set_value_variable(data, &token);

	printf("Value: %d ... init: %d\n", data->value.value_integer, data->inicialized);
	data->inicialized = 0;
	str_destroy(&(token.t_str));
	//2
	token.t_state = st_double;
	set_type_variable(data, &token);

	str_create(&(token.t_str));
	str_add_char(&(token.t_str), '2');

	set_value_variable(data, &token);

	printf("Value: %f .. init: %d\n", data->value.value_double, data->inicialized);
	data->inicialized = 0;
	str_destroy(&(token.t_str));
	//3
	token.t_state = st_string;
	set_type_variable(data, &token);

	str_create(&(token.t_str));
	str_add_char(&(token.t_str), 'a');

	set_value_variable(data, &token);

	printf("%s\n", data->value.value_string);

	str_add_char(&(token.t_str), 'b');

	set_value_variable(data, &token);

	printf("%s\n", data->value.value_string);

	str_destroy(&(token.t_str));

	printf("TEST 4 - tvorba struktury pre data funkcie\n");

	str_create(&(token.t_str));
	str_add_char(&(token.t_str), 'f');

	function_data *data_f;
	data_f = create_data_function(&token);
	if(data_f == NULL) {
		fprintf(stderr, "Error allocating memory\n");
		return -1;
	}

	printf("Name: %s ... defined: %d ... argCount: %d\n", data_f->name, data_f->defined, data_f->arguments_count);

	printf("TEST 5 - nastavenie navratoveho typu funkcie\n");
	token.t_state = st_integer;
	set_return_type_function(data_f, &token);
	printf("Ret: %c\n", data_f->return_type);

	token.t_state = st_double;
	set_return_type_function(data_f, &token);
	printf("Ret: %c\n", data_f->return_type);

	token.t_state = st_string;
	set_return_type_function(data_f, &token);
	printf("Ret: %c\n", data_f->return_type);

	token.t_state = st_del;
	set_return_type_function(data_f, &token);

	str_destroy(&(token.t_str));

	printf("TEST 6 - praca s argumentami funkcie\n");

	str_create_init(&(token.t_str), "arg1");

	add_argument_function(data_f, &token);
	printf("Argument c: %d = %s\n", data_f->arguments_count, data_f->arguments[data_f->arguments_count - 1].argument_name->data);
/*
	str_destroy(&(token.t_str));

	str_create_init(&(token.t_str), "arg2");

	add_argument_function(data_ff, &token);
	printf("Argument c: %d = %s\n", data_f->arguments_count, data_f->arguments[data_f->arguments_count - 1].argument_name->data);

	str_destroy(&(token.t_str));

	return 0;
	*/
}
