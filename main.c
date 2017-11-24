#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "lex.h"
#include "strings.h"
#include "symbol.h"
#include "symtable.h"
#include "parser.h"

htab_t *global_table;
function_data *global_data;
int p = 0;
int ar_count = 0;

int main() {   
    global_table = htab_init(HTAB_SIZE);
    global_data = create_global_data();

    int error = 0;
    if ((error = str_create(&(token.t_str))) != 0)
        return error;
    token.t_line = 1;
    bool parser;
    parser = rule_start_state();
    printf("%d\n",parser);
    if (parser == false){
    	str_destroy(&(token.t_str));
    	printf("ERROR!\n");
    	return -1;
    }

    // while (1)
    // {
    //     error = generate_token();
    //     if (error) {
    //         break;
    //     } else if (token.t_state == st_eof) {
    //         printf("EOF\n");
    //         break;
    //     }

    //     printf("token: '%d', str: '%s'\n", token.t_state, token.t_str.data);
    //         //printf("str:%s delka:%d vel.:%d\n\n", token.t_str.data, token.t_str.length, token.t_str.size);

    // }

    /*if (parser = false){
    	str_destroy(&(token.t_str));
    	printf("ERROR!\n");
    	return -1;
    }*/
    str_destroy(&(token.t_str));

    htab_free(global_table);
    free_data_function(global_data);

    return error;
}