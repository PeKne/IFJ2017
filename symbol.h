#ifndef SYMBOL_H
#define SYMBOL_H

#define DEBUG
#include "errors.h"
#include "symtable.h"
#include "lex.h"

typedef enum {
	variable_integer,
	variable_double,
	variable_string,
} variable_type;

typedef struct {
	Tstring argument_name;
	variable_type type;
} function_arguments;

typedef struct variable_data {
	int inicialized;
	variable_type type;
	union {
		int value_integer;
		double value_double;
		Tstring value_string;
	} value;
	char *name;
} variable_data;

typedef struct function_data {
	int defined; //predpokláme, že funkcia bola deklarovaná ak existuje položka v globálnej tabuľke
	unsigned arguments_count;
	char return_type;
	function_arguments *arguments;
	htab_t *local_symbol_table;
	char *name;
} function_data;

//#include "global.h"
extern htab_t *global_table;
extern function_data *global_data;
extern int p;

// Tvorba dát: variable *data = create_data_variable(token) - vracia NULL, ak zlyhá malloc
variable_data *create_data_variable(Ttoken *token);
void set_type_variable(variable_data *data, Ttoken *token);
void set_value_variable(variable_data *data, Ttoken *token);

// Vyhlada položku v tabuľke, v prípade, že ju nenajde, vytvorí ju a vráti ukazateľ, nastaví typ položky a priradí data
void variable_data_to_table(htab_t *table, variable_data *data);

// Volane priamo len v prípade, že ešte nebola volana funkcia variable_data_to_table a je potreba uvoľniť pamäť
void free_data_variable(variable_data *data);



function_data *create_data_function(Ttoken *token);
void set_defined_function(function_data *data);
void set_return_type_function(function_data *data, Ttoken *token);
void set_local_symbol_table(htab_t *table, function_data *data);

// Ret ERR_INTERN v pripade ze zlyhá malloc/realloc, inak 0
int add_argument_function(function_data *data, Ttoken *token);
void set_argument_type_function(function_data *data, Ttoken *token);

// Vyhlada položku v tabuľke, v prípade, že ju nenajde, vytvorí ju a vráti ukazateľ, nastaví typ položky a priradí data
void function_data_to_table(htab_t *table, function_data *data);

// Volane priamo len v pripade, že ešte nebola volana funcia function_data_to_table a je potreba uvoľniť pamäť
void free_data_function(function_data *data);


//Init globálnych dát, vracia NULL, ak zlyhá malloc
function_data *create_global_data(void);
//Pri vstupe do tela funkcie použiť na nahratie dát danej funkcie do globálnych dát, 1 v prípade úspechu, inak 0
int retrieve_function_data(char *function_name);
int push_function_data(char *function_name);

//Použiť v expresion
//Kontrola či premenná existuje v lokálnej tabuľke globálnych dát, 1 ak existuje, 0 ak neexistuje alebo tabuľka v glob. datách == NULL
int variable_exist(char *variable_name);

int check_variable_type(char *variable_name, Tstate state);
Tstate return_variable_type(char *variable_name);
int check_variable_inicialized(char *variable_name);


//Použiť v rekurzívnom zostupe
int check_function_return_type(Tstate state);
int check_argument_count(unsigned count);
int check_argument_type(Tstate state, unsigned index);
int check_argument_name(char *name, unsigned index);
int check_defined_function(void);

#endif
