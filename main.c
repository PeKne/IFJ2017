#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "lex.h"
#include "strings.h"
#include "expresion.h"

int main() {
    int error = 0;
    if ((error = str_create(&(token.t_str))) != 0)
        return error;

    bool parser;
    generate_token();
    parser = precedent_analysis();
    if (parser == false){
    	str_destroy(&(token.t_str));
    	printf("ERROR!\n");
    	return -1;
    }
    else{
      printf("PRAISE THE IFJ!!!!\n");
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

    if (parser = false){
    	str_destroy(&(token.t_str));
    	printf("ERROR!\n");
    	return -1;
    }
    str_destroy(&(token.t_str));

    return error;
}
