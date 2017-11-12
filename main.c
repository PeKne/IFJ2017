#include <stdio.h>
#include <stdlib.h>
#include "lex.h"
#include "strings.h"


int main()
{    
    int err = 0;
    if ((err = str_create(&(token.t_str))) != 0)
        return err;
    
    for (int i = 0; i < 10; i++)
    {   
        err = generate_token();
        if (token.t_state == st_eof)
            return err;
        printf("token: %d, error: %d\n", token.t_state, err);
        printf("str:'%s' delka:%d vel.:%d\n\n", token.t_str.data, token.t_str.length, token.t_str.size);
    }
    str_destroy(&(token.t_str));

    return err;
}