#ifndef LEX_H
#define LEX_H

#include "strings.h"

#define ERR_LEX -1

typedef enum {

// SMERODATNE STAVY
    st_eof,            /*0*/
    st_error,          /*1*/
    st_id,             /*2 identifikator  */
    st_del,            /*3 /   */
    st_del_cele,       /*4 \   */
    st_nas,            /*5 *   */
    st_scit,           /*6 +   */
    st_odcit,          /*7 -   */
    st_mensi,          /*8 <   */
    st_vetsi,          /*9 >   */
    st_menrov,         /*10 <= */
    st_vetrov,         /*11 >= */
    st_rovno,          /*12 =  */
    st_nerov,          /*13 <> */
    st_stred,          /*14 ;  */
    st_carka,          /*15 ,  */
    st_tecka,          /*16 .  */
    st_levzav,         /*17 (  */
    st_pravzav,        /*18 )  */
    st_retez,          /*19 retezec */
    st_int_val,        /*20*/
    st_double_val,     /*21*/
    st_exp,            /*22 1E1 nebo 1.2E1*/
    st_rez,            /*23 rezerv. slovo */ // TODO
    st_klic,           /*24 klic. slovo   */ // TODO

// POMOCNE STAVY
    st_begin,          /*25*/
    st_final,          /*26*/
    st_vykric,         /*27 !  */
    st_radek_kom,      /*28 '  */
    st_blok_kom_0,     /*29 /' */
    st_blok_kom_1,     /*30 /'koment */
    st_blok_kom_2,     /*31 '/ */ 
    st_retLZ,          /*32 retezcovy literal zacatek: !" */
    st_esc,            /*33 escape znak \ */
    st_double_val_dot, /*34*/
    st_exp_e,          /*35*/
    st_exp_s,          /*36*/
} Tstate;


typedef struct t_token {
    Tstring t_str;         // jeden token ze vstupu (např."integer")
    Tstate t_state;        // int, double, string, as, asc,  ...
} Ttoken;

Ttoken token;   // globalni token

int generate_token();

#endif