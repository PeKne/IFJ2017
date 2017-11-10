#include <stdio.h>
#include <stdlib.h>
#include "lex.h"
#include "strings.h"


int main()
{    
    int err = 0;
    if ((err = str_create(&(token.t_str))) != 0)
        return err;
    str_add_char(&(token.t_str), 'a');
    

    printf("str:%s, delka:%d vel.:%d\n", token.t_str.data, token.t_str.length, token.t_str.size);
    str_destroy(&(token.t_str));

    for (int i = 0; i < 2; ++i)
    {
        err = generate_token();
        printf("token: %d, error: %d\n\n", token.t_state, err);
    }

    return err;
}