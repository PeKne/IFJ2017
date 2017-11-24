#ifndef PARSER
#define PARSER

#include "lex.h"
#include "symbol.h"
#include "symtable.h"
#include "errors.h"
#include "expresion.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define HTAB_SIZE 23

//Funkce rekurzivniho sestupu
void skip_blank_lines();
bool rule_start_state();
bool rule_scope();
bool rule_function();
bool rule_function_dec(); //
bool rule_function_head(); //
bool rule_function_tail();
bool rule_par(function_data *data_f); //
bool rule_next_par(function_data *data_f); //
bool rule_check_par(); // 
bool rule_check_next_par(); //
bool rule_type(variable_data *data); //
bool rule_ret_type(function_data *data_f); //
bool rule_check_ret_type(); //
bool rule_arg_type(function_data *data_f); //
bool rule_check_arg_type(); //
bool rule_st_list();
bool rule_stat();
bool rule_eval();
bool rule_assign();
bool rule_call_par();
bool rule_call_next_par();
bool rule_pr_expr();

#endif