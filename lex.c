#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include "lex.h"
#include "strings.h"
#include "errors.h"

#define key_count 35

const char *key_words[] = {
    "as\0", "asc\0","declare\0","dim\0","do\0","double\0",
    "else\0","end\0","chr\0","function\0", "if\0", "input\0",
    "integer\0", "length\0", "loop\0", "print\0", "return\0", 
    "scope\0", "string\0", "substr\0", "then\0", "while\0",

    "and\0", "boolean\0", "continue\0", "elseif\0", "exit\0",
    "false\0", "for\0", "next\0", "not\0", "or\0", "shared\0",
    "static\0", "true\0",
};

void unget_char(int c) {
    if (!isspace(c) || (c == '\n')) {
        ungetc(c, stdin);
    }
}

Tstate key_or_id()
{
    for (int i = 0; i < key_count; i++)
    {
        if ((strcmp(token.t_str.data, key_words[i])) == 0)
            return i;
    }
    return st_id;
}

void delete_leading_zeroes_int()
{
    while ((token.t_str.data[0] == '0') && (token.t_str.length != 1)) {
        str_delete_index(&(token.t_str), 0);
    }
}

void delete_leading_zeroes_doub()
{
    while ((token.t_str.data[0] == '0') && (token.t_str.data[1] != '.')) {
        str_delete_index(&(token.t_str), 0);
    }
}

void delete_leading_zeroes_exp_int()
{
    while ((token.t_str.data[0] == '0') && (token.t_str.data[1] != 'e')) {
        str_delete_index(&(token.t_str), 0);
    }
}


void delete_zeroes_after_e() {
    // najde index znaku 'e'
    int i = 0;
    for (i = 0; i < token.t_str.length-1; i++)
    {
        if (token.t_str.data[i] == 'e') {
            break;
        }
    }
    // vymaze pocatecni nuly od znaku 'e' (na indexu i)
    if ((token.t_str.data[i+1] == '+') || (token.t_str.data[i+1] == '-'))
        i++;

    while ((token.t_str.data[i+1] == '0') && (token.t_str.length > i+2)) { // +2 i za 'e'
        str_delete_index(&(token.t_str), (i+1)); // i+1 existuje, jinak je to lex chyba
    }
}

int generate_token()
{
    bool get_next_char = true; // pokracovat v lex. anal.
    char c;
    int error = 0;
    
    Tstate state = st_begin;

    if ((error = str_clear(&(token.t_str)))) // jedno volání, jeden token
        return error;

    if (token.t_state == st_eol) { // pokud minuly token byl EOL, zvyš počet řádků
        token.t_line++;
    }

    while ((get_next_char) && (c = getc(stdin)))
    {   
        if (state != st_retez)
            c = tolower(c);

        switch(state)
        {
            case st_begin:
            {   
                
                if (c == '\n') {
                    state = st_eol;  
                    unget_char(c);
                    break;
                }

                else if (isspace(c)) {
                    state = st_begin;
                    break;
                }

                else if ((isalpha(c)) || c == '_') state = st_id; // ident zacina _ nebo pismenem
                else if (isdigit(c))               state = st_int_val;
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
                    state = st_error;
                    fprintf(stderr, "Error, uknown token!\n");
                    break;
                }
                // vlozi prvni znak do stringu
               if ((state != st_eof) && (state != st_vykric) && 
                  (state != st_radek_kom))
               {
                str_push_char(&(token.t_str), c);
               }
               break;
            }
            
            // neprazdna posloupnost cislic, pismen, podtrzitka. 
            case st_id:
            {
                if (isdigit(c) || isalpha(c) || c == '_') {
                    state = st_id;
                    str_push_char(&(token.t_str), c);
                
                // cokoliv jine + rez + klic. slova
                } else { 
                    // nacetl identifikator, ted se muze stav zmenit na rezev./klic. slovo nebo hotovo
                    token.t_state = key_or_id();
                    unget_char(c);
                    state = st_final;
                }
                break;
            }

            case st_mensi:
            {

                if(c == '='){
                    str_push_char(&(token.t_str), c);
                    state = st_menrov;
                }
                else if (c == '>'){
                    str_push_char(&(token.t_str), c);
                    state = st_nerov;
                } else {
                    token.t_state = state;
                    unget_char(c);
                    state = st_final;                    
                }
                break;
            }

            case st_vetsi:
            {
                if(c == '='){
                    str_push_char(&(token.t_str), c);
                    state = st_vetrov;
                } else {
                    token.t_state = state;
                    unget_char(c);
                    state = st_final;                    
                }
                break;
            }


            /***** retezec ********************************************************/
            case st_vykric:
            {
                if (c == '"'){
                    state = st_retez;
                    token.t_state = st_retez;
                } else {
                    fprintf(stderr, "Error, right after '!' must be '\"'!\n");
                    state = st_error;
                }
                break;
            }

            case st_retez:
            {
                if (c == '\\') {
                    str_push_char(&(token.t_str), c);
                    state = st_esc;
                } else if (c == '"') {
                    state = st_final;
                } else if (c == EOF) {
                    fprintf(stderr, "Error, after string must be \"\n");
                    state = st_error;
                } else {
                    str_push_char(&(token.t_str), c);
                    state = st_retez;
                }
                break;
            }

            case st_esc:
            {
                if (c == EOF) {
                    state = st_error;
                } else {
                    str_push_char(&(token.t_str), c);
                    state = st_retez;
                }
                break;
            }
            /***** konec retezce **************************************************/


            case st_radek_kom:
            {
                if ((c != '\n') && (c != EOF)) {
                    state = st_radek_kom;
                } else {
                    if (c == '\n')
                        unget_char(c);
                    state = st_begin;                  
                }
                break;
            }

            /***** blok koment ****************************************************/
            // pokud se za / vyskytuje ', jedna se o zacatek blok. komentu
            case st_blok_kom_0:
            {
                if (c == '\''){
                    str_pop_char(&(token.t_str));
                    state = st_blok_kom_1;
                } else {
                    unget_char(c);
                    state = st_del;
                }
                break;

            }
            // pokud je znak EOF, vrati error, pokud narazi na ', vejde do stavu 2
            // pokud nenajde ani jedno, vrati se zpet do stavu 1.
            case st_blok_kom_1:
            {
                if (c == '\'') {
                    state = st_blok_kom_2;
                } else {
                    if (c == EOF) {
                        fprintf(stderr, "Error, block comment not completed!\n");
                        state = st_error;
                    } else {
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
                    state = st_begin;
                } else if (c == EOF) {
                    fprintf(stderr, "Error, block comment not completed!\n");
                    state = st_error;
                } else {
                    unget_char(c);
                    state = st_blok_kom_1;
                }
                break;
            }
            /***** konec blok komentu ********************************************/


            case st_int_val:
            {
                if (isdigit(c)){
                    str_push_char(&(token.t_str), c);
                    state = st_int_val;
                } else if (c == '.') {
                    str_push_char(&(token.t_str), c);
                    state = st_double_val_dot;
                } else if (c == 'e') {
                    str_push_char(&(token.t_str), c);
                    state = st_exp_int_e;
                } else { // nacten jiny znak, tento je ukoncen
                    token.t_state = state;
                    unget_char(c);
                    state = st_final;
                }
                break;
            }

            case st_double_val_dot:
            {
                if (isdigit(c)) {
                    str_push_char(&(token.t_str), c);
                    state = st_double_val;
                } else {
                    fprintf(stderr, "In double value, after '.' must be a number!\n");
                    state = st_error;                    
                }
                break;
            }

            case st_double_val:
            {
                if (isdigit(c)) {
                    str_push_char(&(token.t_str), c);
                    state = st_double_val;
                } else if (c == 'e') {
                    str_push_char(&(token.t_str), c);
                    state = st_exp_doub_e;
                } else { // nacten jiny znak
                    token.t_state = state;
                    unget_char(c);
                    state = st_final;
                }
                break;
            }


            /***** int s exponentem **********************************************/
            case st_exp_int_e:
            {
                if ((c == '+') || (c == '-')) {
                    str_push_char(&(token.t_str), c);
                    state = st_exp_int_s;
                } else if (isdigit(c)) {
                    str_push_char(&(token.t_str), c);
                    state = st_exp_int;
                } else {
                    fprintf(stderr, "In exponential value, after E must follow a number or sign!\n");
                    state = st_error;
                }
                break;
            }

            case st_exp_int_s:
            {
                if (isdigit(c)) {
                    str_push_char(&(token.t_str), c);
                    state = st_exp_int;
                } else {
                    fprintf(stderr, "In exponential value, after sign must follow a number!\n");
                    state = st_error;
                }
                break;
            }

            case st_exp_int:
            {
                if (isdigit(c)) {
                    str_push_char(&(token.t_str), c);
                    state = st_exp_int;
                } else { // nacten jiny znak
                    token.t_state = state;
                    unget_char(c);
                    state = st_final;
                }
                break;
            }
            /***** konec int s exponentem ****************************************/


            /***** double s exponentem *******************************************/
            case st_exp_doub_e:
            {
                if ((c == '+') || (c == '-')) {
                    str_push_char(&(token.t_str), c);
                    state = st_exp_doub_s;
                } else if (isdigit(c)) {
                    str_push_char(&(token.t_str), c);
                    state = st_exp_doub;
                } else {
                    fprintf(stderr, "In exponential value, after E must follow a number or sign!\n");
                    state = st_error;
                }
                break;
            }

            case st_exp_doub_s:
            {
                if (isdigit(c)) {
                    str_push_char(&(token.t_str), c);
                    state = st_exp_doub;
                } else {
                    fprintf(stderr, "In exponential value, after sign must follow a number!\n");
                    state = st_error;
                }
                break;
            }

            case st_exp_doub:
            {
                if (isdigit(c)) {
                    str_push_char(&(token.t_str), c);
                    state = st_exp_doub;
                } else { // nacten jiny znak
                    token.t_state = state;
                    unget_char(c);
                    state = st_final;
                }
                break;
            }
            /***** konec double s exponentem **************************************/

            // koncove stavy
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
            case st_rovno:
            case st_menrov:
            case st_vetrov:
            case st_nerov:
            case st_eof:
            {
                token.t_state = state;
                unget_char(c);
                state = st_final;
                break;
            }

            case st_eol:
            {
                token.t_state = state;
                state = st_final;
                break;
            }

            case st_final:
            {
                unget_char(c);
                get_next_char = false;
                
                if (token.t_state == st_int_val) {
                    delete_leading_zeroes_int();
                } else if (token.t_state == st_double_val) {
                    delete_leading_zeroes_doub();
                } else if (token.t_state == st_exp_int) {
                    delete_leading_zeroes_exp_int();
                    delete_zeroes_after_e();
                } else if (token.t_state == st_exp_doub) {
                    delete_leading_zeroes_doub();
                    delete_zeroes_after_e();
                }

                break;
            }

            case st_error:
            {
                error = ERR_LEX;
                token.t_state = state;
                get_next_char = false;
                break;
            }

            default: 
            {
                error = ERR_LEX; // default nesmi nikdy nastat
                fprintf(stderr, "Error, uknown token! Ask developer to fix it.\n");
                get_next_char = false;
                break;
            }
        }
    }
    return error;
}
