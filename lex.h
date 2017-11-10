#ifndef LEX_H
#define LEX_H

#include "strings.h"

#define ERR_UNKNOWN_CHAR -1
#define ERR_MISSING_STATE -2

typedef enum {
    st_begin,        /*0*/ //hotovo
    st_error,        /*1*/ //hotovo
    
    st_finish,       /*2*/ //hotovo
    st_eof,          /*3*/ //hotovo
    st_del,          /*4 /  */ //hotovo
    st_del_cele,     /*5 \  */
    st_nas,          /*6 *  */ //hotovo
    st_scit,         /*7 +  */ //hotovo
    st_odcit,        /*8 -  */ //hotovo
    st_mensi,        /*9 <  */ //hotovo
    st_vetsi,        /*10 >  */ //hotovo
    st_menrov,       /*11 <= */ //hotovo
    st_vetrov,       /*12 >= */ //hotovo
    st_rovno,        /*13 =  */ //hotovo
    st_nerov,        /*14 <> */ //hotovo
    st_stred,        /*15 ;  */ //hotovo
    st_carka,        /*16 ,  */ //hotovo
    st_tecka,        /*17 .  */ //hotovo
    st_levzav,       /*18 (  */ //hotovo
    st_pravzav,      /*19 )  */ //hotovo
    st_vykric,       /*20 !  */ //hotovo
    st_id,           /*21 identifikator  */ //hotovo
    st_retLZ,        /*22 retezcovy literal zacatek: !" */ //hotovo
    st_retez,        /*23 retezec */ //hotovo
    st_retLK,        /*24 retezcovy literal konec: " */ //hotovo
    st_int_value,    /*25*/
    st_double_value, /*26*/
    st_rez,          /*27 rezerv. slovo */
    st_klic,         /*28 klic. slovo   */
} Tstate;


typedef struct t_token {
    Tstring t_str;         // jeden token ze vstupu (např."integer")
    Tstate t_state;        // int, double, string, as, asc,  ...
} Ttoken;

Ttoken token;   // globalni token

void init_token();
void unget_char(int c);
int generate_token();

#endif