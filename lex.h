#ifndef LEX_H
#define LEX_H

#include "strings.h"

#define ERR_LEX -1

typedef enum {

/******NEMENIT ANI NEPRIDAVAT******/
/******ZDE ZALEZI NA PORADI********/
// KLICOVA SLOVA
/*0*/ st_as,
/*1*/ st_asc,
/*2*/ st_declare,
/*3*/ st_dim,
/*4*/ st_do,
/*5*/ st_double,
/*6*/ st_else,
/*7*/ st_end,
/*8*/ st_chr,
/*9*/ st_function,
/*10*/ st_if,
/*11*/ st_input,
/*12*/ st_integer,
/*13*/ st_length,
/*14*/ st_loop,
/*15*/ st_print,
/*16*/ st_return,
/*17*/ st_scope,
/*18*/ st_string,
/*19*/ st_substr,
/*20*/ st_then,
/*21*/ st_while,

// REZERVOVANA KLICOVA SLOVA
/*22*/ st_and,
/*23*/ st_boolean,
/*24*/ st_continue,
/*25*/ st_elseif,
/*26*/ st_exit,
/*27*/ st_false,
/*28*/ st_for,
/*29*/ st_next,
/*30*/ st_not,
/*31*/ st_or,
/*32*/ st_shared,
/*33*/ st_static,
/*34*/ st_true,
/******NEMENIT ANI NEPRIDAVAT******/


// SMERODATNE STAVY
/*35*/ st_eof,
/*36*/ st_error,
/*37*/ st_id,             /*identifikator  */
/*38*/ st_del,            /* /   */
/*39*/ st_del_cele,       /* \   */
/*40*/ st_nas,            /* *   */
/*41*/ st_scit,           /* +   */
/*42*/ st_odcit,          /* -   */
/*43*/ st_mensi,          /* <   */
/*44*/ st_vetsi,          /* >   */
/*45*/ st_menrov,         /* <= */
/*46*/ st_vetrov,         /* >= */
/*47*/ st_rovno,          /* =  */
/*48*/ st_nerov,          /* <> */
/*49*/ st_stred,          /* ;  */
/*50*/ st_carka,          /* ,  */
/*51*/ st_tecka,          /* .  */
/*52*/ st_levzav,         /* (  */
/*53*/ st_pravzav,        /* )  */
/*54*/ st_retez,          /* retezec */
/*55*/ st_int_val,        /* hodnota int bez exp*/
/*56*/ st_double_val,     /* hodnota double bez exp*/
/*57*/ st_exp,            /* int nebo double s exp*/

// POMOCNE STAVY
/*58*/ st_begin,
/*59*/ st_final,
/*60*/ st_vykric,         /* !  */
/*61*/ st_radek_kom,      /* '  */
/*62*/ st_blok_kom_0,     /* /' */
/*63*/ st_blok_kom_1,     /* /'koment */
/*64*/ st_blok_kom_2,     /* '/ */ 
/*65*/ st_retLZ,          /* retezcovy literal zacatek: !" */
/*66*/ st_esc,            /* escape znak \ */
/*67*/ st_double_val_dot,
/*68*/ st_exp_e,
/*69*/ st_exp_s,
} Tstate;


typedef struct t_token {
    Tstring t_str;         // jeden token ze vstupu (např."integer")
    Tstate t_state;        // int, double, string, as, asc,  ...
} Ttoken;

Ttoken token;   // globalni token

int generate_token();

#endif