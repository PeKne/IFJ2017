#include <stdio.h>
#include <stdlib.h>
#include "lex.h"
#include "strings.h"

int main()
{    
    int error = 0;
    if ((error = str_create(&(token.t_str))) != 0)
        return error;

    // inicializace počtu řádků. Pokud se neprovede, bude se počítat od 0.
    token.t_line = 1;

    while (1)
    {
        error = generate_token();
        if (error) {
            break;
        } else if (token.t_state == st_eof) {
            printf("EOF\n");
            break;
        }

        printf("token: '%d', str: '%s' line: %d\n", token.t_state, token.t_str.data, token.t_line);
        //printf("str:%s delka:%d vel.:%d\n\n", token.t_str.data, token.t_str.length, token.t_str.size);
    }

    str_destroy(&(token.t_str));

    return error;
}