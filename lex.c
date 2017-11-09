#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

#include "lex.h"

Ttoken token; // globalni token

typedef enum {
    st_as,
    st_asc,
    st_declare,
    st_dim,
    st_do,
    st_double,
    st_else,
    st_end,
    st_chr,
    st_function,
    st_if,
    st_input,
    st_integer,
    st_length,
    st_loop,
    st_print,
    st_return,
    st_scope,
    st_string,
    st_substr,
    st_then,
    st_while,
} Trez;

typedef enum {
    st_and,
    st_boolean,
    st_continue,
    st_elseif,
    st_exit,
    st_false,
    st_for,
    st_next,
    st_not,
    st_or,
    st_shared,
    st_static,
    st_true,
}Tkey;

void init_token()
{
    token.t_state = st_begin;
    token.t_value_ptr = NULL;
}

void unget_char(int c) {
    if (!isspace(c))
        ungetc(c, stdin);
}
int generate_token() //
{
    init_token();
    Tstate state = st_begin;
    bool get_next_char = true; // pokracovat v lex. anal.
    char c;

    while ((get_next_char) && (c = getc(stdin)))
    {
        switch(state)
        {
            case st_begin:
            {
                if (isspace(c)) {
                    state = st_begin;
                    break;
                }

                else if ((isalpha(c)) || c == '_') state = st_id; // ident zacina _ nebo pismenem
                else if (c == '/')                 state = st_del;
                else if (c == '\\')                state = st_del_cele;
                else if (c == '*')                 state = st_nas;
                else if (c == '+')                 state = st_scit;
                else if (c == '-')                 state = st_odcit;
                else if (c == '<')                 state = st_mensi;
                else if (c == '>')                 state = st_vetsi;
                else if (c == '=')                 state = st_rovno;
                else if (c == ';')                 state = st_stred;
                else if (c == ',')                 state = st_carka;
                else if (c == '(')                 state = st_levzav;
                else if (c == ')')                 state = st_pravzav;
                else if (c == '!')                 state = st_vykric;
                else if (c == EOF)                 state = st_eof;     
                
                else {
                    state = st_error;
                    break;
                }
                //-- add_char_to_string();
                break;
            }
            default: {
                get_next_char = false;
                break;
            }
            
            // neprazdna posloupnost cislic, pismen, podtrzitka. 
            case st_id:
            {
                if (isdigit(c) || isalpha(c) || c == '_') {
                    state = st_id;
                    //str_add_char();
                } else { 
                    // nacetl identifikator, ted se muze stav zmenit na rezev./klic. slovo
                    state = st_exit;
                    //-- token.t_state = get_special_id_case();
                    unget_char(c);
                }
            }
        }
        break;
    }

    return state;
}
