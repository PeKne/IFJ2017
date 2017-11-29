#include "parser.h"
#include "expresion.h"
#include "code_gen_expres.h"

extern function_data *global_data;
extern int p;
extern int ar_count;
extern int if_counter;
extern int while_counter;

/**************************FUNKCE-REKURZIVNIHO-SESTUPU*********************************/
/**************************************************************************************/

int skip_blank_lines(){
    int return_value;
    while(token.t_state == st_eol){
        if(return_value = generate_token()) return return_value;
    }
    return 0;
}

int rule_start_state(){ // stav <start t_state>
    int return_value = ERR_SYN;
    if(return_value = generate_token()) return return_value;    
    if(token.t_state == st_scope || token.t_state == st_declare || token.t_state == st_function){ // simulace pravidla 1.
        printf(".IFJcode17\n");///
        printf("JUMP $$scope\n\n");

        if ((return_value = rule_function()) == 0)
                    return_value = rule_scope();
    }
    return return_value;
}

int rule_scope(){ // pravidlo <scope>
    if(skip_blank_lines() != 0) return ERR_LEX;
    int return_value = ERR_SYN;
    if(token.t_state == st_scope){ // simulace pravidla 2.
        printf("LABEL $$scope\n");
        printf("DEFVAR GF@&pomInteger\n");///
        printf("DEFVAR GF@&pomFloat\n");///
        printf("DEFVAR GF@&pomString\n");///
        printf("DEFVAR GF@&pomBool\n");///
        printf("DEFVAR GF@&pomType\n\n");///

        printf("CREATEFRAME\n");///
        if(return_value = generate_token()) return return_value;

        if((return_value = rule_st_list()) == 0){
            if(skip_blank_lines() != 0) return ERR_LEX;
            if(token.t_state == st_end){
                if(return_value = generate_token()) return return_value;

                if(token.t_state == st_scope){
                    if(return_value = generate_token()) return return_value;
                    return_value = 0;
                }
            }

        }
    }
    return return_value;
}

int rule_function(){ // stav <function>
    if(skip_blank_lines() != 0) return ERR_LEX;
    int return_value = ERR_SYN;
    if(token.t_state == st_function){ // simulace pravidla 3.
            if((return_value = rule_function_head()) == 0)
                if((return_value = rule_st_list()) == 0)
                    if((return_value = rule_function_tail()) == 0)
                        if((return_value = rule_function()) == 0)
                            return_value = 0;
    }
    else if(token.t_state == st_declare){ //simulace pravidla 4.
            if((return_value = rule_function_dec()) == 0)
                if((return_value = rule_function()) == 0)
                    return_value = 0;
    }
    else if(token.t_state == st_scope){ //simulace pravidla 5.
        return_value = 0;
    }

    return return_value;

}

int rule_function_dec(){ // stav <function-dec>
    int return_value = ERR_SYN;

    if(token.t_state == st_declare){ // simulace pravidla 6.
        if(return_value = generate_token()) return return_value;

        if(token.t_state == st_function){
            if(return_value = generate_token()) return return_value;

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
                if(return_value = generate_token()) return return_value;

                if(token.t_state == st_levzav){
                    if(return_value = generate_token()) return return_value;

                    if((return_value = rule_par(data_f)) == 0){

                        if(token.t_state == st_pravzav){
                            if(return_value = generate_token()) return return_value;

                            if(token.t_state == st_as){
                                if(return_value = generate_token()) return return_value;

                                if((return_value = rule_ret_type(data_f)) == 0){
                                    function_data_to_table(global_table, data_f);
                                    return_value = 0;
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

int rule_function_head(){ // stav <function-head>
    int return_value = ERR_SYN;
    Tstring ident;///
    p = 1;

    if(token.t_state ==  st_function){ // simulace pravidla 7.
        if(return_value = generate_token()) return return_value;

        if(token.t_state == st_id){

          if(retrieve_function_data(token.t_str.data) != 1) {
              return ERR_SEM_PROG;
          }
          if(check_defined_function() == 1) {
              free_data_function(global_data);
              return ERR_SEM_PROG;
          }
          set_defined_function(global_data);
          if(push_function_data(global_data->name) != 1) {
            return ERR_SEM_OTHER;
          }
           if(retrieve_function_data(token.t_str.data) != 1) {
              return ERR_SEM_PROG;
          }
          str_create_init(&ident, token.t_str.data);///
            if(return_value = generate_token()) return return_value;

            if(token.t_state == st_levzav){
                if(return_value = generate_token()) return return_value;

                if((return_value = rule_check_par()) == 0){

                    if(token.t_state == st_pravzav){
                        if(return_value = generate_token()) return return_value;

                        if(token.t_state == st_as){
                            if(return_value = generate_token()) return return_value;

                            if((return_value = rule_check_ret_type()) == 0){

                                if(token.t_state == st_eol){
                                    printf("LABEL $%s\n", ident.data);///
                                    printf("PUSHFRAME\n");
                                    str_destroy(&ident);///
                                    if(return_value = generate_token()) return return_value;
                                    return_value = 0;
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

int rule_function_tail(){ // stav <function-tail>
    if(skip_blank_lines() != 0) return ERR_LEX;
    int return_value = ERR_SYN;
    if(token.t_state == st_end){ // simulace pravidla 8.
        printf("POPFRAME\n");
        printf("RETURN\n\n");
        if(return_value = generate_token()) return return_value;

        if(token.t_state == st_function){
            if(return_value = generate_token()) return return_value;
            return_value = 0;
        }
    }

    if(push_function_data(global_data->name) != 1) {
        printf("ERROR pushing data\n");
        return ERR_SEM_OTHER;
    }
    p = 0;
    ar_count = 0;
    return return_value;
}

int rule_par(function_data *data_f){ // stav <par>
    int return_value = ERR_SYN;
    if(token.t_state == st_id){ // simulace pravidla 9.
        variable_data *data = create_data_variable(&token);
        if(data == NULL) {
            return ERR_INTERN;
        }
        if(add_argument_function(data_f, &token) != 0) {
            free_data_variable(data);
            return ERR_SEM_PROG;
        }
        if(return_value = generate_token()) return return_value;

        if(token.t_state == st_as){
            if(return_value = generate_token()) return return_value;

            if((return_value = rule_arg_type(data_f)) == 0)
                if((return_value = rule_type(data)) == 0)
                    if((return_value = rule_next_par(data_f)) == 0){
                        variable_data_to_table(data_f->local_symbol_table, data);
                        return_value = 0;
            }
        }
    }

    else if(token.t_state == st_pravzav){ //simulace pravidla 10.
        return_value = 0;
    }

    return return_value;
}

int rule_next_par(function_data *data_f){ // stav <next-par>
    int return_value = ERR_SYN;
    if(token.t_state ==  st_carka){ // simulace pravidla 11.

        if((return_value = rule_par(data_f)) == 0){
            return_value = 0;
        }
    }
    else if(token.t_state == st_pravzav){ // simulace pravidla 12.
        return_value = 0;
    }


    return return_value;


}

int rule_check_par(){ // stav <check-par>
    int return_value = ERR_SYN;
    if(token.t_state == st_id){ // simulace pravidla 9.
        ar_count++;
        if(check_argument_name(token.t_str.data, ar_count) != 1) {
            return ERR_SEM_TYPE; //Meno premmenej nie je spravne
        }
        if(return_value = generate_token()) return return_value;

        if(token.t_state == st_as){
            if(return_value = generate_token()) return return_value;

            if((return_value = rule_check_arg_type()) == 0) {
                if(return_value = generate_token()) return return_value;

                if((return_value = rule_check_next_par()) == 0){
                    return_value = 0;
                }
            }
        }
    }

    else if(token.t_state == st_pravzav){ //simulace pravidla 10.
        return_value = 0;
    }
    return return_value;
}

int rule_check_next_par(){ // stav <check-next-par>
    int return_value = ERR_SYN;
    if(token.t_state ==  st_carka){ // simulace pravidla 11.

        if((return_value = rule_check_par()) == 0){
            return_value = 0;
        }
    }
    else if(token.t_state == st_pravzav){ // simulace pravidla 12.
        return_value = 0;
    }

    return return_value;
}

int rule_type(variable_data *data){ // stav <type>
    int return_value = ERR_SYN;
    if(token.t_state == st_integer || token.t_state == st_double || token.t_state == st_string){ // simulace pravidla 13., 14., 15.
        set_type_variable(data, &token);
        if(return_value = generate_token()) return return_value;
        return_value = 0;
    }

    return return_value;
}

int rule_ret_type(function_data *data_f){ // stav <ret-type>
    int return_value = ERR_SYN;
    if(token.t_state == st_integer || token.t_state == st_double || token.t_state == st_string){ // simulace pravidla 13., 14., 15.
        set_return_type_function(data_f, &token);
        if(return_value = generate_token()) return return_value;
        return_value = 0;
    }

    return return_value;
}

int rule_check_ret_type(){ // stav <check-ret-type>
    int return_value = ERR_SYN;
    if(token.t_state == st_integer || token.t_state == st_double || token.t_state == st_string){ // simulace pravidla 13., 14., 15.
        if(check_function_return_type(token.t_state) == 0) {
            return ERR_SEM_TYPE;
        }
        if(return_value = generate_token()) return return_value;
        return_value = 0;
    }

    return return_value;
}

int rule_arg_type(function_data *data_f){ // stav <arg-type>
    int return_value = ERR_SYN;
    if(token.t_state == st_integer || token.t_state == st_double || token.t_state == st_string){ // simulace pravidla 13., 14., 15.
        set_argument_type_function(data_f, &token);
        return_value = 0;
    }

    return return_value;
}

int rule_check_arg_type(){ // stav <check-arg-type>
    int return_value = ERR_SYN;
    if(token.t_state == st_integer || token.t_state == st_double || token.t_state == st_string){ // simulace pravidla 13., 14., 15.
        if(check_argument_type(token.t_state, ar_count) != 1) {
            return ERR_SEM_TYPE; //Neodpovedajuci datovy typ argumentu
        }
        return_value = 0;
    }

    return return_value;
}

int rule_st_list(){ // stav <st-list>
    if(skip_blank_lines() != 0) return ERR_LEX;
    int return_value = ERR_SYN;
    if(token.t_state == st_id || token.t_state == st_dim || token.t_state == st_input ||
       token.t_state == st_print || token.t_state == st_if || token.t_state == st_do ||
       token.t_state == st_return  ){ // simulace pravidla 16.
                if((return_value = rule_stat()) == 0)
                    if((return_value = rule_st_list()) == 0)
                        return_value = 0;
    }

    else if(token.t_state == st_end || token.t_state == st_else|| token.t_state == st_loop){ // simulace pravidla 17.
        return_value = 0;
    }

    return return_value;
}

// TODO v else uvolnovat Tstring ident******
int rule_stat(){ // stav <stat>
    int return_value = ERR_SYN;
    Tstring ident;///
    int instruction;
    int destination = -1;

    if(token.t_state == st_dim ){ // simulace pravidla 18.
        if(return_value = generate_token()) return return_value;

        if(token.t_state == st_id){
            htab_listitem *item;
            item = htab_find((p == 1 ? global_data->local_symbol_table : global_table), token.t_str.data);
            if(item != NULL) {
                return ERR_SEM_PROG; //Premmenna nebola vramci danej funkcie deklarovana
            }
            free(item);
            variable_data *data = create_data_variable(&token);
            if(data == NULL) {
                return ERR_INTERN;
            }
            str_create_init(&ident, token.t_str.data);///
            if(return_value = generate_token()){
                str_destroy(&ident);///
                return return_value;
            }

            if(token.t_state == st_as){
                if(return_value = generate_token()){
                    str_destroy(&ident);///
                    return return_value;
                }

                if((return_value = rule_type(data)) == 0){
                    if((return_value = rule_eval()) == 0){
                        variable_data_to_table((p == 1 ? global_data->local_symbol_table : global_table), data);

                        char* context = (p == 0 ? "TF@" : "LF@");
                        printf("DEFVAR %s%s\n", context, ident.data);///
                        return_value = 0;
                    }
                }
            }
            str_destroy(&ident);///
        }
    }

    else if(token.t_state == st_id){ // simulace pravidla 21.
        if(variable_exist(token.t_str.data) ==  0){
            printf("Not defined variable\n");
            return ERR_SEM_PROG; //Premmenna nebola vramci danej funkcie deklarovana
        }
        str_create_init(&ident, token.t_str.data);
        if(return_value = generate_token()){
            str_destroy(&ident);///
            return return_value;
        }

        if(token.t_state == st_rovno){
            if(return_value = generate_token()){
                str_destroy(&ident);///
                return return_value;
            }
            if((return_value = rule_assign(ident)) == 0){
                return_value = 0;
            } 
        }
        str_destroy(&ident);///
    }
    

    else if(token.t_state == st_input){ // simulace pravidla 28.
        if(return_value = generate_token()) return return_value;

        if(token.t_state == st_id){
            if(variable_exist(token.t_str.data) == 0) {
                printf("Not defined variable\n");
                return ERR_SEM_PROG; //Premmenna nebola vramci danej funkcie deklarovana
            }
            char* context = (p == 0 ? "TF@" : "LF@");

            printf("TYPE GF@&pomType %s%s\n",context, token.t_str.data);///
            printf("READ %s%s, GF@&pomType\n",context, token.t_str.data);///
            if(return_value = generate_token()) return return_value;
            return_value = 0;
        }
    }

    else if(token.t_state == st_print){ // simulace pravidla 29.
        instruction = ins_print;
        if(return_value = generate_token()) return return_value;

        //printf("WRITE %s\n",token.t_str.data);///

        if((return_value = precedent_analysis(instruction, &destination)) == 0){
            if(token.t_state == st_stred){
                if(return_value = generate_token()) return return_value;

                if((return_value = rule_pr_expr()) == 0){
                    return_value = 0;
                }
            }
        }
    }

    else if(token.t_state == st_if){ // simulace pravidla 32.
        instruction = st_if;
        if(return_value = generate_token()) return return_value;

        if((return_value = precedent_analysis(instruction, &destination)) == 0){
            printf("JUMPIFNEQ $$else_%d TF@&pomInteger true\n", if_counter);///
            if(token.t_state == st_then){
                if(return_value = generate_token()) return return_value;

                if(token.t_state == st_eol){
                    if(return_value = generate_token()) return return_value;

                    if((return_value = rule_st_list()) == 0){
                        printf("JUMP $$endif_%d\n", if_counter);
                        printf("LABEL $$else_%d\n", if_counter);
                        if(token.t_state == st_else){
                            if(return_value = generate_token()) return return_value;

                            if(token.t_state == st_eol){
                                if(return_value = generate_token()) return return_value;

                                if(rule_st_list() == 0){

                                    if(token.t_state == st_end){
                                        if(return_value = generate_token()) return return_value;

                                        if(token.t_state == st_if){
                                            printf("LABEL $$endif_%d\n", if_counter);
                                            if_counter++;
                                            if(return_value = generate_token()) return return_value;
                                            return_value = 0;
                                        }
                                    }
                                }
                            }
                        }
              else if(token.t_state == st_end){
                if(return_value = generate_token()) return return_value;
                  if(token.t_state == st_if){
                    if(return_value = generate_token()) return return_value;
                      return_value = 0;
                  }
              }
                    }
                }
            }
        }
    }//konec pravidla 32.

    else if(token.t_state == st_do){ // simulace pravidla 33.
        if(return_value = generate_token()) return return_value;

        if(token.t_state == st_while){
            printf("LABEL $$loop_%d\n", while_counter);
            if(return_value = generate_token()) return return_value;

            if((return_value = precedent_analysis(instruction, &destination)) == 0){

                if(token.t_state == st_eol){
                    printf("JUMPIFENQ $$loop_end_%d TF@&pomBool true\n", while_counter);
                    if(return_value = generate_token()) return return_value;

                    if((return_value = rule_st_list()) == 0){
                        printf("JUMP $$loop_%d\n", while_counter);
                        if(token.t_state == st_loop){
                            printf("LABEL $$loop_end_%d\n", while_counter);
                            while_counter++;
                            if(return_value = generate_token()) return return_value;
                            return_value = 0;
                        }
                    }
                }

            }
        }
    }//konec 33. pravidla

    else if(token.t_state == st_return){ // simulace pravidla 34.
        if(return_value = generate_token()) return return_value;

        if((return_value = precedent_analysis(instruction, &destination)) == 0){

            return_value = 0;
        }
    }

    return return_value;
}// konec funkce rule_stat()

int rule_eval(){ // stav <eval>
    if(skip_blank_lines() != 0) return ERR_LEX;
    int return_value = ERR_SYN;
    int instruction = -1;
    int destination = -1;

    if(token.t_state == st_rovno ){ // simulace pravidla 19.
        if(return_value = generate_token()) return return_value;

        if((return_value = precedent_analysis(instruction, &destination)) == 0){
            return_value = 0;
        }
    }

    else if(token.t_state == st_id || token.t_state == st_dim || token.t_state == st_input ||
         token.t_state == st_print || token.t_state == st_if  || token.t_state == st_do ||
        token.t_state == st_return || token.t_state == st_end ){ // simulace pravidla 20.

        return_value = 0;
    }
    return return_value;
}

int rule_assign(Tstring id){ // stav <assign>
    int return_value = ERR_SYN;
    int instruction = -1;
    int destination = -1;
    char* context = (p == 0 ? "TF@" : "LF@");

    if(token.t_state == st_id){ // simulace pravidla 23.
        htab_listitem *item;
        item = htab_find((p == 1 ? global_data->local_symbol_table : global_table), token.t_str.data);
        if(item == NULL) { // tady konci vetsina vstupu TODO: OPRAVIT!
            item = htab_find(global_table, token.t_str.data);
            if(item == NULL || item->type != type_function) {
                printf("NULL\n");
                return ERR_SEM_PROG;
            }
        }
        if(item->type != type_function){// identifikator neni funkce
          if((return_value = precedent_analysis(instruction, &destination)) == 0){
            if      (destination == ex_red_int)    printf("MOV %s%s GF@&pomInteger\n",context, id.data);      
            else if (destination == ex_red_double) printf("MOV %s%s GF@&pomFloat\n",context, id.data);
            else if (destination == ex_red_str)    printf("MOV %s%s GF@&pomString\n",context, id.data);
          }
          return return_value;
        }
        if(item->pointer.function->defined == 0) {//identifikator nedefinovane funkce
            //free(item);
            return ERR_SEM_OTHER;
        }

        if(return_value = generate_token()) return return_value;
        if(token.t_state == st_levzav){
            if(return_value = generate_token()) return return_value;

            if((return_value = rule_call_par()) == 0){

                if(token.t_state == st_pravzav){
                    if(return_value = generate_token()) return return_value;
                    printf("funkce\n");
                    return_value = 0;
                }

            }

        }

    }// konec pravidla 23.
    else if((return_value = precedent_analysis(instruction, &destination)) == 0){  //simulace pravidla 22.   
        if      (destination == ex_red_int)    printf("MOV %s%s GF@&pomInteger\n",context, id.data);      
        else if (destination == ex_red_double) printf("MOV %s%s GF@&pomFloat\n",context, id.data);
        else if (destination == ex_red_str)    printf("MOV %s%s GF@&pomString\n",context, id.data);
        return_value = 0;
    }

    return return_value;
}// konec funkce rule_assign()

int rule_call_par(){ // stav <call-par>
    int return_value = ERR_SYN;
    if(token.t_state == st_id){ // simulace pravidla 24.
        if(return_value = generate_token()) return return_value;

        if((return_value = rule_call_next_par()) == 0){
            return_value = 0;
        }
    }

    else if(token.t_state == st_pravzav){ //simulace pravidla 25.
        return_value = 0;
    }

    return return_value;
}

int rule_call_next_par(){ // stav <call-next-par>
    int return_value = ERR_SYN;
    if(token.t_state == st_carka){ // simulace pravidla 26.

        if((return_value = rule_call_par()) == 0){
            return_value = 0;
        }
    }

    else if(token.t_state == st_pravzav){ // simulace pravidla 27.
        return_value = 0;
    }

    return return_value;
}

int rule_pr_expr(){ // stav <pr-expr>
    int return_value = ERR_SYN;
    int instruction = -1;
    int destination = -1;

    if(token.t_state == st_dim || token.t_state == st_input ||
       token.t_state == st_print || token.t_state == st_if || token.t_state == st_do ||
       token.t_state == st_return || token.t_state == st_end || token.t_state == st_eol ){  // simulace pravidla 31.
            if(skip_blank_lines() != 0) return ERR_LEX;
        return_value = 0;
    }

        else if((return_value = precedent_analysis(instruction, &destination)) == 0){  //simulace pravidla 30.

        if(token.t_state == st_stred){
            if(return_value = generate_token()) return return_value;

              if((return_value = rule_pr_expr()) == 0){
                return_value = 0;
            }
        }

    }

    return return_value;
}
