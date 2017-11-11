#ifndef LEX_H
#define LEX_H

#include "strings.h"

#define ERR_UNKNOWN_CHAR -1
#define ERR_MISSING_STATE -2
#define ERR_BLOCK_COM_NOT_COMPLETED -3

typedef enum {

// SMERODATNE STAVY
    st_eof,          /*0*/
    st_id,           /*1 identifikator  */
    st_del,          /*2 /   */
    st_del_cele,     /*3 \   */
    st_nas,          /*4 *   */
    st_scit,         /*5 +   */
    st_odcit,        /*6 -   */
    st_mensi,        /*7 <   */
    st_vetsi,        /*8 >   */
    st_menrov,       /*9 <= */
    st_vetrov,       /*10 >= */
    st_rovno,        /*11 =  */
    st_nerov,        /*12 <> */
    st_stred,        /*13 ;  */
    st_carka,        /*14 ,  */
    st_tecka,        /*15 .  */
    st_levzav,       /*16 (  */
    st_pravzav,      /*17 )  */
    st_retLZ,        /*18 retezcovy literal zacatek: !" */
    st_retez,        /*19 retezec */
    st_retLK,        /*20 retezcovy literal konec: " */
    st_int_value,    /*21*/                // TODO
    st_double_value, /*22*/                // TODO
    st_rez,          /*23 rezerv. slovo */ // TODO
    st_klic,         /*24 klic. slovo   */ // TODO
    st_error,        /*25*/

// POMOCNE STAVY
    st_vykric,       /*26 !  */
    st_radek_kom,    /*27 '  */
    st_blok_kom_0,   /*28 /' */
    st_blok_kom_1,   /*39 /'koment */
    st_blok_kom_2,   /*30 '/ */ 
    st_begin,        /*31*/
    st_finish,       /*32*/
} Tstate;


typedef struct t_token {
    Tstring t_str;         // jeden token ze vstupu (např."integer")
    Tstate t_state;        // int, double, string, as, asc,  ...
} Ttoken;

Ttoken token;   // globalni token

int generate_token();

#endif