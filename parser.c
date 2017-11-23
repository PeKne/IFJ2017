#include "parser.h"
#include "expresion.h"
// TODO vyrazy, typy

/**************************FUNKCE-REKURZIVNIHO-SESTUPU*********************************/
/**************************************************************************************/

void skip_blank_lines(){ 
	while(token.t_state == st_eol){
		generate_token();
	}
}

bool rule_start_state(){ // stav <start t_state>
    printf(".IFJcode17\n");///
    generate_token();
    bool return_value = false;
    if(token.t_state == st_scope || token.t_state == st_declare || token.t_state == st_function){ // simulace pravidla 1.
        return_value = (rule_function() && rule_scope());
    }
    return return_value;
}

bool rule_scope(){ // pravidlo <scope>
    skip_blank_lines();
	bool return_value = false;
    if(token.t_state == st_scope){ // simulace pravidla 2.
        printf("CREATEFRAME\n");///
        printf("DEFVAR pomBool\n");///
        printf("DEFVAR pomString\n");///
        printf("DEFVAR pomInt\n");///
        printf("DEFVAR pomFloat\n\n");///
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
    			generate_token();

    			if(token.t_state == st_levzav){
    				generate_token();

    				if(rule_par()){

    					if(token.t_state == st_pravzav){
    						generate_token();

    						if(token.t_state == st_as){
    							generate_token();

    							if(rule_type()){
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
    if(token.t_state ==  st_function){ // simulace pravidla 7.
        generate_token();

        if(token.t_state == st_id){
        	generate_token();

        	if(token.t_state == st_levzav){
        		generate_token();

        		if(rule_par()){

        			if(token.t_state == st_pravzav){
        				generate_token();

        				if(token.t_state == st_as){
        					generate_token();

        					if(rule_type()){

        						if(token.t_state == st_eol){ 
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
        generate_token();

        if(token.t_state == st_function){
        	generate_token();
        	return_value = true;
        }
    }
    
    return return_value;    
}

bool rule_par(){ // stav <par>
	bool return_value = false;
    if(token.t_state == st_id){ // simulace pravidla 9.
        generate_token();

        if(token.t_state == st_as){
        	generate_token();

        	if(rule_type() && rule_next_par()){
        		return_value = true;
        	}
        }
    }

    else if(token.t_state == st_pravzav){ //simulace pravidla 10.
    	return_value = true;
    }
    
    return return_value;    
}

bool rule_next_par(){ // stav <next-par>
	bool return_value = false;
    if(token.t_state ==  st_carka){ // simulace pravidla 11. 
        
        if(rule_par()){
        	return_value = true;
        }
    }
    else if(token.t_state == st_pravzav){ // simulace pravidla 12.
    	return_value = true;
    }

    
    return return_value;    


}

bool rule_type(){ // stav <type>
	bool return_value = false;
    if(token.t_state == st_integer || token.t_state == st_double || token.t_state == st_string){ // simulace pravidla 13., 14., 15.
        generate_token();
        return_value = true;
    }
    
    return return_value;    
}

bool rule_st_list(){ // stav <st-list>
    skip_blank_lines();
	bool return_value = false;
    if(token.t_state == st_id || token.t_state == st_dim || token.t_state == st_input ||
       token.t_state == st_print || token.t_state == st_if || token.t_state == st_do || 
       token.t_state == st_return ){ // simulace pravidla 16.
        //printf("st_if\n");
        return_value = (rule_stat() && rule_st_list());
    }

    else if(token.t_state == st_end){ // simulace pravidla 17.
    	return_value = true;
    }

    return return_value;    
}

bool rule_stat(){ // stav <stat>
	bool return_value = false;
    Tstring ident;///
    
    if(token.t_state == st_dim ){ // simulace pravidla 18.
        generate_token();

        if(token.t_state == st_id){
            str_create_init(&ident, token.t_str.data);///
        	generate_token();

        	if(token.t_state == st_as){
        		generate_token();

        		if(rule_type() && rule_eval()){
                    printf("DEFVAR %s\n", ident.data);///
                    str_destroy(&ident);///
        			return_value = true;
        		}
        	}
        }
    }
    
    else if(token.t_state == st_id){ // simulace pravidla 21.
        Tstring id;
        str_create_init(&id, token.t_str.data);
    	generate_token();

    	if(token.t_state == st_rovno){
    		generate_token();

    		if(rule_assign(id)){
                str_destroy(&id);
    			return_value = true;
    		}
    	}
    }

    else if(token.t_state == st_input){ // simulace pravidla 28.
    	generate_token();

    	if(token.t_state == st_id){
            printf("TYPE TF@pomString TF@%s\n",token.t_str.data);///
            printf("READ TF@%s, TF@pomString\n",token.t_str.data);///
    		generate_token();
    		return_value = true;
    	}
    }

    else if(token.t_state == st_print){ // simulace pravidla 29.
    	generate_token();
        printf("WRITE %s\n",token.t_str.data);///
        int assign = 0;
    	if(precedent_analysis(assign)){ // TODO GENEROVANI
            // TODO: musim tady generovat token?
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
        printf("tady\n");
        int assign = 0;
    	if(precedent_analysis(assign)){ // TODO: EXPRESSION!!!! 
    			// TODO: musim tady generovat token?
            printf("JUMPIFNEQ else1 TF@pomBool true\n");///
    		if(token.t_state == st_then){
    			generate_token();

    			if(token.t_state == st_eol){
    				generate_token();

    				if(rule_st_list()){

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
    				}
    			}
    		}
    	}
    }//konec pravidla 32.

    else if(token.t_state == st_do){ // simulace pravidla 33.
    	generate_token();

    	if(token.t_state == st_while){
    		generate_token();
            int assign = 0;
    		if(precedent_analysis(assign)){ // TODO: EXPRESSION!!!! 
    			// TODO: musim tady generovat token?
    			
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
        int assign = 0;
    	if(precedent_analysis(assign)){ // TODO: EXPRESSION!!!! 
    			// TODO: musim tady generovat token?
    		return_value = true;
    	}
    }
    
    return return_value;   
}// konec funkce rule_stat()

bool rule_eval(){ // stav <eval>
    skip_blank_lines();
	bool return_value = false;
    if(token.t_state == st_rovno ){ // simulace pravidla 19.
        generate_token();
        int assign = 0;
        if(precedent_analysis(assign)){ // TODO: EXPRESSION!!!! 
    			// TODO: musim tady generovat token?
        	return_value = true;
    	}
    }
    
    else if(token.t_state == st_id || token.t_state == st_dim || token.t_state == st_input ||
       token.t_state == st_print || token.t_state == st_if || token.t_state == st_do || 
       token.t_state == st_return || token.t_state == st_end ){ // simulace pravidla 20.

    	return_value = true;
    }
    return return_value;
}

bool rule_assign(Tstring id){ // stav <assign>
	bool return_value = false;
    int assign = 1;
    if(precedent_analysis(assign)){ // TODO: EXPRESSION!!!! //simulace pravidla 22.
    			// TODO: musim tady generovat token?
            printf("MOV TF@%s TF@pom\n", id.data);
        	return_value = true;
    	}

    else if(token.t_state == st_id){ // simulace pravidla 23.
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
    skip_blank_lines();
	bool return_value = false;
    int assign = 0;
    if(precedent_analysis(assign)){ // TODO: EXPRESSION!!!! //simulace pravidla 30.
    			// TODO: musim tady generovat token?

        if(token.t_state == st_stred){
        	generate_token();

  			if(rule_pr_expr()){
        		return_value = true;
        	}
        }

    }

    else if(token.t_state == st_id || token.t_state == st_dim || token.t_state == st_input ||
       token.t_state == st_print || token.t_state == st_if || token.t_state == st_do || 
       token.t_state == st_return || token.t_state == st_end ){  // simulace pravidla 31.
    	return_value = true;
    }
    	
    return return_value;
}
