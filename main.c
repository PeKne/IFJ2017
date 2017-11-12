#include <stdio.h>
#include <stdlib.h>
#include "lex.h"
#include "strings.h"


int main()
{    
    int err = 0;
    if ((err = str_create(&(token.t_str))) != 0)
        return err;
    
    /*for (int i = 0; i < 100; i++)
    {   
        err = generate_token();
        if (token.t_state == st_eof)
            return err;
        
            printf("%d) token: '%d', str: '%s'\n", i, token.t_state, token.t_str.data);
            //printf("str:%s delka:%d vel.:%d\n\n", token.t_str.data, token.t_str.length, token.t_str.size);
    }*/
    for (int i = 0; i < 30; ++i)
    {
        str_push_char(&(token.t_str), 'a');
    
    }
    printf("str:%s delka:%d vel.:%d\n\n", token.t_str.data, token.t_str.length, token.t_str.size);

    for (int i = 0; i < 30; ++i)
    {
        str_pop_char(&(token.t_str));
        printf("str:%s delka:%d vel.:%d\n", token.t_str.data, token.t_str.length, token.t_str.size);
    }
    
    

    str_destroy(&(token.t_str));

    return err;
}