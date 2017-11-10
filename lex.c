#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include "lex.h"
#include "strings.h"

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

void unget_char(int c) {
    if (!isspace(c))
        ungetc(c, stdin);
}

Tstate rez_key_id()
{
    /*for (int i = 0; i < Trez; i++)
    {
    
    }*/
    return st_id;
}

int generate_token() //
{
    bool get_next_char = true; // pokracovat v lex. anal.
    char c;
    int err = 0;
    
    Tstate state = st_begin;
    if ((token.t_state == st_retLZ) || (token.t_state == st_retez))
        state = st_retez;

    //str_clear(&(token.t_str));

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
                else if (isdigit(c))               state = st_int_value;
                else if (c == '/')                 state = st_blok_kom_0;
                else if (c == '\\')                state = st_del_cele;
                else if (c == '\'')                state = st_radek_kom;
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
                    printf ("st_begin error\n");
                    state = st_error;
                    err = ERR_UNKNOWN_CHAR;
                    break;
                }
                //str_add_char(&(token.t_str), c);
                break;
            }
            
            // neprazdna posloupnost cislic, pismen, podtrzitka. 
            case st_id:
            {
                if (isdigit(c) || isalpha(c) || c == '_') {
                    state = st_id;
                    //str_add_char(&(token.t_str), c);
                
                // cokoliv jine +REZERV. SLOVA**********************************************
                } else { 
                    // nacetl identifikator, ted se muze stav zmenit na rezev./klic. slovo nebo hotovo
                    
                    token.t_state = rez_key_id();
                    unget_char(c);
                    state = st_finish;
                }
                break;
            }

            case st_mensi:
            {
                
                if(c == '=')
                    state = st_menrov;
                else if (c == '>')
                    state = st_nerov;
                else {
                    token.t_state = state;
                    //str_add_char(&(token.t_str), c);
                    unget_char(c);
                    state = st_finish;                    
                }
                break;
            }

            case st_vetsi:
            {
                if(c == '=')
                    state = st_vetrov;
                else {
                    token.t_state = state;
                    //str_add_char(&(token.t_str), c);
                    unget_char(c);
                    state = st_finish;                    
                }
                break;
            }

            case st_vykric:
            {
                if (c == '"')
                    state = st_retLZ;
                else {
                    //printf("st_error\n");
                    err = ERR_UNKNOWN_CHAR;
                    state = st_error;
                }
                break;
            }

            case st_retLZ:
            {
                token.t_state = state;
                //str_add_char(&(token.t_str), c);
                unget_char(c);
                get_next_char = false;
                state = st_retez;
                break;
            }

            case st_retez:
            {
                if (c != '"') {
                    if (c == EOF){
                        state = st_finish;
                    }
                    else {
                        token.t_state = st_retez;
                        get_next_char = false;
                        //str_add_char(&(token.t_str), c);
                    }                    
                } else
                    state = st_retLK;
                break;
            }

            case st_radek_kom:
            {
                if ((c != '\n') && (c != EOF)){
                    state = st_radek_kom;
                }
                else
                    state = st_begin;                  
                break;
            }

            // blok. koment
            // pokud se za / vyskytuje ', jedna se o zacatek blok. komentu
            case st_blok_kom_0:
            {
                if (c == '\''){
                    //printf("0 do 1: %d, %c\n", state, c);
                    state = st_blok_kom_1;
                }
                else
                    state = st_del;
                break;

            }
            // pokud je znak EOF, vrati error, pokud narazi na ', vejde do stavu 2
            // pokud nenajde ani jedno, vrati se zpet do stavu 1.
            case st_blok_kom_1:
            {
                if (c == '\'') {
                    //printf("1 do 2: %d, %c\n", state, c);
                    state = st_blok_kom_2;
                } else {
                     if (c == EOF) {
                        err = ERR_BLOCK_COM_NOT_COMPLETED;
                        state = st_error;
                    } else {
                        //printf("1 do 1: %d, %c\n", state, c);
                        state = st_blok_kom_1;
                    }
                }
                break;
            }

            // pokud ihned najde /, ukonci komentar
            // pokud ne, vrati se zpet do stavu 1
            case st_blok_kom_2:
            {
                if (c == '/') {
                    //printf("2 na begin: %d, %c\n", state, c);
                    state = st_begin;
                }
                else {
                    //printf("2 na 1: %d, %c\n", state, c);
                    unget_char(c);
                    state = st_blok_kom_1;
                }
                break;
            }



            // koncove stavy na jeden znak
            case st_scit:
            case st_nas:
            case st_del:
            case st_del_cele:
            case st_odcit:
            case st_stred:
            case st_carka:
            case st_tecka:
            case st_levzav:
            case st_pravzav:
            case st_eof:
            case st_rovno:
            case st_menrov:
            case st_vetrov:
            case st_nerov:
            case st_retLK:
            {
                token.t_state = state;
                //str_add_char(&(token.t_str), c);
                unget_char(c);
                state = st_finish;
                break;
            }

            case st_finish:
            {
                unget_char(c);
                get_next_char = false;
                break;
            }

            case st_error:
            {
                token.t_state = state;
                get_next_char = false;
                break;
            }

            default: 
            {
                err = ERR_MISSING_STATE;
                get_next_char = false;
                break;
            }
        }
    }
    return err;
}
