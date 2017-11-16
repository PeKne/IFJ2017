#ifndef PARSER
#define PARSER

//Funkce rekurzivniho sestupu
void skip_blank_lines()
bool rule_start_state();
bool rule_scope();
bool rule_function();
bool rule_function_dec();
bool rule_function_head();
bool rule_funtion_tail();
bool rule_par();
bool rule_next_par();
bool rule_type();
bool rule_st_list();
bool rule_stat();
bool rule_eval();
bool rule_assign();
bool rule_call_par();
bool rule_call_next_par();
bool rule_pr_expr();

#endif