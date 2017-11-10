#ifndef LEX_H
#define LEX_H

#include "strings.h"

#define ERR_UNKNOWN_CHAR -1
#define ERR_MISSING_STATE -2
#define ERR_BLOCK_COM_NOT_COMPLETED -3

typedef enum {

// SMERODATNE STAVY
    st_eof,          /*1*/
    st_id,           /*2 identifikator  */
    st_del,          /*3 /   */
    st_del_cele,     /*4 \   */
    st_nas,          /*5 *   */
    st_scit,         /*6 +   */
    st_odcit,        /*7 -   */
    st_mensi,        /*8 <   */
    st_vetsi,        /*9 >   */
    st_menrov,       /*10 <= */
    st_vetrov,       /*11 >= */
    st_rovno,        /*12 =  */
    st_nerov,        /*13 <> */
    st_stred,        /*14 ;  */
    st_carka,        /*15 ,  */
    st_tecka,        /*16 .  */
    st_levzav,       /*17 (  */
    st_pravzav,      /*18 )  */
    st_retLZ,        /*19 retezcovy literal zacatek: !" */
    st_retez,        /*20 retezec */
    st_retLK,        /*21 retezcovy literal konec: " */
    st_int_value,    /*22*/                // TODO
    st_double_value, /*23*/                // TODO
    st_rez,          /*24 rezerv. slovo */ // TODO
    st_klic,         /*25 klic. slovo   */ // TODO

// POMOCNE STAVY
    st_vykric,       /*26 !  */
    st_radek_kom,    /*27 '  */
    st_blok_kom_0,   /*28 /' */
    st_blok_kom_1,   /*29 /'koment */
    st_blok_kom_2,   /*30 '/ */ 
    st_begin,        /*31*/
    st_finish,       /*32*/
    st_error,        /*33*/
} Tstate;


typedef struct t_token {
    Tstring t_str;         // jeden token ze vstupu (např."integer")
    Tstate t_state;        // int, double, string, as, asc,  ...
} Ttoken;

Ttoken token;   // globalni token

int generate_token();

#endif