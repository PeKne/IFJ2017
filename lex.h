#ifndef LEX_H
#define LEX_H

typedef enum {
    st_begin,
    st_del,        /* /  */
    st_del_cele,   /* \  */
    st_nas,        /* *  */
    st_scit,       /* +  */
    st_odcit,       /* -  */
    st_mensi,      /* <  */
    st_vetsi,      /* >  */
    st_menrov,     /* <= */
    st_vetrov,     /* >= */
    st_rovno,      /* =  */
    st_nerov,      /* <> */
    st_stred,      /* ;  */
    st_carka,      /* ,  */
    st_tecka,      /* .  */
    st_levzav,     /* (  */
    st_pravzav,    /* )  */
    st_vykric,     /* !  */
    st_id,         /* identifikator  */
    st_retLZ,      /* retezcovy literal zacatek: !" */
    st_retez,      /* retezec */
    st_retLK,      /* retezcovy literal konec: " */
    st_int_value,
    st_double_value,
    st_eof,
    st_rez,        /* rezerv. slovo */
    st_klic,       /* klic. slovo   */
    st_error,
} Tstate;


typedef struct t_token {
    char* t_value_ptr; //hodnota u int, double ...
    Tstate t_state;   //int, double, string, as, asc,  ...
} Ttoken;

void init_token();
void unget_char(int c);
int generate_token();


#endif