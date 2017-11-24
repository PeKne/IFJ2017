#include "parser.h"
#include "expresion.h"
extern function_data *global_data;
extern int p;
extern int ar_count;

/**************************FUNKCE-REKURZIVNIHO-SESTUPU*********************************/
/**************************************************************************************/

void skip_blank_lines(){
	while(token.t_state == st_eol){
		generate_token();
	}
}

bool rule_start_state(){ // stav <start t_state>
    generate_token();
    bool return_value = false;
    if(token.t_state == st_scope || token.t_state == st_declare || token.t_state == st_function){ // simulace pravidla 1.
        printf(".IFJcode17\n");///
        printf("JUMP $$main\n\n");
        return_value = (rule_function() && rule_scope());
    }
    return return_value;
}

bool rule_scope(){ // pravidlo <scope>
    skip_blank_lines();
	bool return_value = false;
    if(token.t_state == st_scope){ // simulace pravidla 2.
        printf("LABEL $$main\n");
        printf("DEFVAR GF@$pomBool\n");///
        printf("DEFVAR GF@$pomString\n");///
        printf("DEFVAR GF@$pomInt\n");///
        printf("DEFVAR GF@$pomFloat\n\n");///
        printf("CREATEFRAME\n");///
        generate_token();

        if(rule_st_list()){

            skip_blank_lines();
        	if(token.t_state == st_end){
        		generate_token();

        		if(token.t_state == st_scope){
        			generate_token();
        			return_value = true;
        		}
        	}

        }
    }
    return return_value;
}

bool rule_function(){ // stav <function>
    skip_blank_lines();
	bool return_value = false;
    if(token.t_state == st_function){ // simulace pravidla 3.
        return_value = (rule_function_head() && rule_st_list() && rule_function_tail() && rule_function());
    }
    else if(token.t_state == st_declare){ //simulace pravidla 4.
    	return_value = (rule_function_dec() && rule_function());
    }
    else if(token.t_state == st_scope){ //simulace pravidla 5.
    	return_value = true;
    }

    return return_value;

}

bool rule_function_dec(){ // stav <function-dec>
	bool return_value = false;

    if(token.t_state == st_declare){ // simulace pravidla 6.
    	generate_token();

    	if(token.t_state == st_function){
    		generate_token();

    		if(token.t_state == st_id){
                htab_t *local_table = htab_init(HTAB_SIZE);
                if(local_table == NULL) {
                    return ERR_INTERN;
                }

                function_data *data_f = create_data_function(&token);
                if(data_f == NULL) {
                    free(local_table);
                    return ERR_INTERN;
                }

                set_local_symbol_table(local_table, data_f);
    			generate_token();

    			if(token.t_state == st_levzav){
    				generate_token();

    				if(rule_par(data_f)){

    					if(token.t_state == st_pravzav){
    						generate_token();

    						if(token.t_state == st_as){
    							generate_token();

    							if(rule_ret_type(data_f)){
                                    function_data_to_table(global_table, data_f);
    								return_value = true;
    							}
    						}
    					}
    				}
    			}

    		}
    	}

    }

    return return_value;
}

bool rule_function_head(){ // stav <function-head>
	bool return_value = false;
    Tstring ident;///
    p = 1;

    if(token.t_state ==  st_function){ // simulace pravidla 7.
        generate_token();

        if(token.t_state == st_id){
            if(retrieve_function_data(token.t_str.data) != 1) {
                return ERR_SEM_PROG;
            }
            if(check_defined_function() == 1) {
                free_data_function(global_data);
                return ERR_SEM_PROG;
            }
            str_create_init(&ident, token.t_str.data);///
        	generate_token();

        	if(token.t_state == st_levzav){
        		generate_token();

        		if(rule_check_par()){

        			if(token.t_state == st_pravzav){
        				generate_token();

        				if(token.t_state == st_as){
        					generate_token();

        					if(rule_check_ret_type()){

        						if(token.t_state == st_eol){
                                    printf("LABEL $$%s\n", ident.data);///
                                    printf("PUSHFRAME\n");
                                    str_destroy(&ident);///
        							generate_token();
        							return_value = true;
        						}
        					}
        				}
        			}
        		}
        	}
        }
    }

    return return_value;
}

bool rule_function_tail(){ // stav <function-tail>
    skip_blank_lines();
	bool return_value = false;
    if(token.t_state == st_end){ // simulace pravidla 8.
        printf("POPFRAME\n");
        printf("RETURN\n\n");
        generate_token();

        if(token.t_state == st_function){
        	generate_token();
        	return_value = true;
        }
    }

    p = 0;
    ar_count = 0;
    return return_value;
}

bool rule_par(function_data *data_f){ // stav <par>
	bool return_value = false;
    if(token.t_state == st_id){ // simulace pravidla 9.
        variable_data *data = create_data_variable(&token);
        if(data == NULL) {
            return ERR_INTERN;
        }
        if(add_argument_function(data_f, &token) != 0) {
            free_data_variable(data);
            return ERR_SEM_PROG;
        }
        generate_token();

        if(token.t_state == st_as){
        	generate_token();

        	if(rule_arg_type(data_f) && rule_type(data) && rule_next_par(data_f)){
                variable_data_to_table(data_f->local_symbol_table, data);
                return_value = true;
            }
        }
    }

    else if(token.t_state == st_pravzav){ //simulace pravidla 10.
    	return_value = true;
    }

    return return_value;
}

bool rule_next_par(function_data *data_f){ // stav <next-par>
	bool return_value = false;
    if(token.t_state ==  st_carka){ // simulace pravidla 11.

        if(rule_par(data_f)){
        	return_value = true;
        }
    }
    else if(token.t_state == st_pravzav){ // simulace pravidla 12.
    	return_value = true;
    }


    return return_value;


}

bool rule_check_par(){ // stav <check-par>
    bool return_value = false;
    if(token.t_state == st_id){ // simulace pravidla 9.
        ar_count++;
        if(check_argument_name(token.t_str.data, ar_count) != 1) {
            return ERR_SEM_TYPE; //Meno premmenej nie je spravne
        }
        generate_token();

        if(token.t_state == st_as){
            generate_token();

            if(rule_check_arg_type()) {
                generate_token();
    
                if(rule_check_next_par()){
                    return_value = true;
                }
            }
        }
    }

    else if(token.t_state == st_pravzav){ //simulace pravidla 10.
        return_value = true;
    }
    return return_value;    
}

bool rule_check_next_par(){ // stav <check-next-par>
    bool return_value = false;
    if(token.t_state ==  st_carka){ // simulace pravidla 11. 
        
        if(rule_check_par()){
            return_value = true;
        }
    }
    else if(token.t_state == st_pravzav){ // simulace pravidla 12.
        return_value = true;
    }

    return return_value;    
}

bool rule_type(variable_data *data){ // stav <type>
	bool return_value = false;
    if(token.t_state == st_integer || token.t_state == st_double || token.t_state == st_string){ // simulace pravidla 13., 14., 15.
        set_type_variable(data, &token);
        generate_token();
        return_value = true;
    }

    return return_value;
}

bool rule_ret_type(function_data *data_f){ // stav <ret-type>
    bool return_value = false;
    if(token.t_state == st_integer || token.t_state == st_double || token.t_state == st_string){ // simulace pravidla 13., 14., 15.
        set_return_type_function(data_f, &token);
        generate_token();
        return_value = true;
    }
    
    return return_value;    
}

bool rule_check_ret_type(){ // stav <check-ret-type>
    bool return_value = false;
    if(token.t_state == st_integer || token.t_state == st_double || token.t_state == st_string){ // simulace pravidla 13., 14., 15.
        printf("%c\n", global_data->return_type);
        if(check_function_return_type(token.t_state) == 0) {
            return ERR_SEM_TYPE;
        }
        generate_token();
        return_value = true;
    }
    
    return return_value;    
}

bool rule_arg_type(function_data *data_f){ // stav <arg-type>
    bool return_value = false;
    if(token.t_state == st_integer || token.t_state == st_double || token.t_state == st_string){ // simulace pravidla 13., 14., 15.
        set_argument_type_function(data_f, &token);
        return_value = true;
    }
    
    return return_value;    
}

bool rule_check_arg_type(){ // stav <check-arg-type>
    bool return_value = false;
    if(token.t_state == st_integer || token.t_state == st_double || token.t_state == st_string){ // simulace pravidla 13., 14., 15.
        if(check_argument_type(token.t_state, ar_count) != 1) {
            return ERR_SEM_TYPE; //Neodpovedajuci datovy typ argumentu
        }
        return_value = true;
    }
    
    return return_value;    
}

bool rule_st_list(){ // stav <st-list>
    skip_blank_lines();
	bool return_value = false;
    if(token.t_state == st_id || token.t_state == st_dim || token.t_state == st_input ||
       token.t_state == st_print || token.t_state == st_if || token.t_state == st_do ||
       token.t_state == st_return  ){ // simulace pravidla 16.

        return_value = (rule_stat() && rule_st_list());
    }

		else if(token.t_state == st_else|| token.t_state == st_loop){
			return_value = true;
		}

    else if(token.t_state == st_end){ // simulace pravidla 17.
    	return_value = true;
    }

    return return_value;
}

bool rule_stat(){ // stav <stat>
	bool return_value = false;
    Tstring ident;///
    int print_command = 0;
    if(token.t_state == st_dim ){ // simulace pravidla 18.
        generate_token();

        if(token.t_state == st_id){
            htab_listitem *item = htab_find((p == 1 ? global_data->local_symbol_table : global_table), token.t_str.data);
            if(item != NULL) {
                return ERR_SEM_PROG; //Premmenna nebola vramci danej funkcie deklarovana
            }
            free(item);
            variable_data *data = create_data_variable(&token);
            if(data == NULL) {
                return ERR_INTERN;
            }
            str_create_init(&ident, token.t_str.data);///
        	generate_token();

        	if(token.t_state == st_as){
        		generate_token();

        		if(rule_type(data) && rule_eval()){
                    variable_data_to_table((p == 1 ? global_data->local_symbol_table : global_table), data);               
                    printf("DEFVAR %s\n", ident.data);///
                    str_destroy(&ident);///
        			return_value = true;
        		}
        	}
        }
    }

    else if(token.t_state == st_id){ // simulace pravidla 21.
        if(variable_exist(token.t_str.data) ==  0) {
            printf("Not defined variable\n");
            return ERR_SEM_PROG; //Premmenna nebola vramci danej funkcie deklarovana
        }
        Tstring id;
        str_create_init(&id, token.t_str.data);
    	generate_token();

    	if(token.t_state == st_rovno){
    		generate_token();

    		if(rule_assign(id)){
                str_destroy(&id);///
    			return_value = true;
    		}
    	}
    }

    else if(token.t_state == st_input){ // simulace pravidla 28.
    	generate_token();

    	if(token.t_state == st_id){
            if(variable_exist(token.t_str.data) == 0) {
                printf("Not defined variable\n");
                return ERR_SEM_PROG; //Premmenna nebola vramci danej funkcie deklarovana
            }
            printf("TYPE TF@pomString TF@%s\n",token.t_str.data);///
            printf("READ TF@%s, TF@pomString\n",token.t_str.data);///
    		generate_token();
    		return_value = true;
    	}
    }

    else if(token.t_state == st_print){ // simulace pravidla 29.
        print_command = 1;
    	generate_token();

        //printf("WRITE %s\n",token.t_str.data);///
        
    	if(precedent_analysis(print_command)){
    		if(token.t_state == st_stred){
    			generate_token();

    			if(rule_pr_expr()){
    				return_value = true;
    			}
    		}
    	}
    }

    else if(token.t_state == st_if){ // simulace pravidla 32.
    	generate_token();

    	if(precedent_analysis(print_command)){
            printf("JUMPIFNEQ else1 TF@$pomBool true\n");///
    		if(token.t_state == st_then){
    			generate_token();

    			if(token.t_state == st_eol){
    				generate_token();

    				if(rule_st_list()){
                        printf("LABEL $$else1\n");

    					if(token.t_state == st_else){
    						generate_token();

    						if(token.t_state == st_eol){
    							generate_token();

    							if(rule_st_list()){

    								if(token.t_state == st_end){
    									generate_token();

    									if(token.t_state == st_if){
    										generate_token();
    										return_value = true;
    									}
    								}
    							}
    						}
    					}
                        else if(token.t_state == st_end){
                                generate_token();
                                if(token.t_state == st_if){
                                    generate_token();
                                    return_value = true;
                                }
                            }
    				}
    			}
    		}
    	}
    }//konec pravidla 32.

    else if(token.t_state == st_do){ // simulace pravidla 33.
    	generate_token();

    	if(token.t_state == st_while){
    		generate_token();

    		if(precedent_analysis(print_command)){

    			if(token.t_state == st_eol){
    				generate_token();

    				if(rule_st_list()){

    					if(token.t_state == st_loop){
    						generate_token();
    						return_value = true;
    					}
    				}
    			}

    		}
    	}
    }//konec 33. pravidla

    else if(token.t_state == st_return){ // simulace pravidla 34.
    	generate_token();

    	if(precedent_analysis(print_command)){

    		return_value = true;
    	}
    }

    return return_value;
}// konec funkce rule_stat()

bool rule_eval(){ // stav <eval>
    skip_blank_lines();
	bool return_value = false;
    int print_command = 0;
    if(token.t_state == st_rovno ){ // simulace pravidla 19.
        generate_token();

        if(precedent_analysis(print_command)){

        	return_value = true;
    	}
    }

    else if(token.t_state == st_id || token.t_state == st_dim || token.t_state == st_input ||
         token.t_state == st_print || token.t_state == st_if  || token.t_state == st_do ||
        token.t_state == st_return || token.t_state == st_end ){ // simulace pravidla 20.

    	return_value = true;
    }
    return return_value;
}

bool rule_assign(Tstring id){ // stav <assign>
	bool return_value = false;
    int print_command = 0;
    if(precedent_analysis(print_command)){  //simulace pravidla 22.
            printf("MOV TF@$%s TF@$pom\n", id.data);
        	return_value = true;
    	}

    else if(token.t_state == st_id){ // simulace pravidla 23.
        htab_listitem *item = htab_find((p == 1 ? global_data->local_symbol_table : global_table), token.t_str.data);
        if(item == NULL) {
            return ERR_SEM_PROG;
        }

        if(item->pointer.function->defined == 0) {
            free(item);
            return ERR_SEM_OTHER;
        }
    	generate_token();

    	if(token.t_state == st_levzav){
    		generate_token();

    		if(rule_call_par()){

    			if(token.t_state == st_pravzav){
    				generate_token();

    				return_value = true;
    			}

    		}

    	}
    }// konec pravidla 23.

    return return_value;
}// konec funkce rule_assign()

bool rule_call_par(){ // stav <call-par>
	bool return_value = false;
    if(token.t_state == st_id){ // simulace pravidla 24.
        generate_token();

        if(rule_call_next_par()){
        	return_value = true;
        }
    }

    else if(token.t_state == st_pravzav){ //simulace pravidla 25.
    	return_value = true;
    }

    return return_value;
}

bool rule_call_next_par(){ // stav <call-next-par>
	bool return_value = false;
    if(token.t_state == st_carka){ // simulace pravidla 26.

        if(rule_call_par()){
        	return_value = true;
        }
    }

    else if(token.t_state == st_pravzav){ // simulace pravidla 27.
    	return_value = true;
    }

    return return_value;
}

bool rule_pr_expr(){ // stav <pr-expr>
	bool return_value = false;
    int print_command = 0;

    if(token.t_state == st_dim || token.t_state == st_input ||
       token.t_state == st_print || token.t_state == st_if || token.t_state == st_do ||
       token.t_state == st_return || token.t_state == st_end || token.t_state == st_eol ){  // simulace pravidla 31.
			skip_blank_lines();
    	return_value = true;
    }

		else if(precedent_analysis(print_command)){  //simulace pravidla 30.

        if(token.t_state == st_stred){
        	generate_token();

  			if(rule_pr_expr()){
        		return_value = true;
        	}
        }

    }

    return return_value;
}
