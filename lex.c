#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

T_token token; // globalni token

typedef enum{
    st_del,        /* /  */
    st_del_cele,   /* \  */
    st_nas,        /* *  */
    st_scit,       /* +  */
    st_odcit,      /* -  */
    st_mensi,      /* <  */
    st_vetsi,      /* >  */
    st_menrov,     /* <= */
    st_vetrov,     /* >= */
    st_rovno,      /* =  */
    st_nerov,      /* <> */
    st_stred,      /* ;  */
    st_carka,      /* ,  */
    st_tecka       /* .  */
    st_levzav,     /* (  */
    st_pravzav,    /* )  */
    st_id,         /* identifikator  */
    st_retLZ,      /* retezcovy literal zacatek: !" */
    st_retez,      /* retezec */
    st_retLK,      /* retezcovy literal konec: " */
    st_int_value,
    st_double_value,
    st_eol,
    st_error,
} T_state;

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
} T_rez;

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
}T_key;

typedef struct t_token {
    char* t_value_ptr; //hodnota u int, double ...
    T_state t_state;    //int, double, string, as, asc,  ...
} T_token;

int init_token()
{
    token.t_state = ST_BEGIN;
    token.t_value_ptr = NULL;
}

void unget_char(int char) {
    if (!isspace(c))
        ungetc(char, stdin);
}
void generate_token() //
{
    init_token();
    T_state state = ST_BEGIN;
    bool get_next_token = true; // pokracovat v lex. anal.

    while ((get_next_token) && (c = getc(stdin)))
    {
        switch(state)
        {
            case ST_BEGIN:
            {
                if (isspace(c)) {
                    state = ST_BEGIN;
                    break;
                }

                else if ((isalpha(c)) || c == '_') state = ST_ID; // ident zacina _ nebo pismenem
                else if (c == '/')                 state = ST_DEL;
                else if (c == '\\')                state = ST_DEL;
                else if (c == '*')                 state = ST_NAS;
                else if (c == '+')                 state = ST_SCIT;
                else if (c == '-')                 state = ST_ODCIT;
                else if (c == '<')                 state = ST_MENSI;
                else if (c == '>')                 state = ST_VETSI;
                else if (c == '=')                 state = ST_ROVNO;
                else if (c == ';')                 state = ST_STRED;
                else if (c == ',')                 state = ST_CARKA;
                else if (c == '(')                 state = ST_LEV_ZAV;
                else if (c == ')')                 state = ST_PRAV_ZAV;
                else if (c == '!')                 state = ST_VYKRIC;
                else if (c == EOF)                 state = ST_EOF;     
                
                else {
                    state = ST_ERROR;
                    break;
                }
                //-- add_char_to_string();
                break;
            }

            // neprazdna posloupnost cislic, pismen, podtrzitka. 
            case ST_ID:
            {
                if (isdigit(c) || isalpha(c) || c == '_') {
                    state = ST_ID;
                    //-- add_char_to_string();
                } else { 
                    // nacetl identifikator, ted se muze stav zmenit na rezev./klic. slovo
                    state = S_EXIT;
                    //-- token.t_state = get_special_id_case();
                    unget_char(c);
                }
            }

        }
    }
}