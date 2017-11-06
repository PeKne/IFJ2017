typedef enum{
st_del      ,st_nas     ,st_scit    ,st_odcit,
st_mensi    ,st_vetsi   ,st_menro   ,st_vetro,
st_rovno    ,st_nerov   ,st_and     ,st_boolean,
st_continue ,st_elseif  ,st_exit    ,st_false,
st_for      ,st_next    ,st_not     ,st_or,
st_shared   ,st_static  ,st_true    ,st_as,
st_asc      ,st_declare ,st_dim     ,st_do,
st_double   ,st_else    ,st_end     ,st_chr,
st_function ,st_if      ,st_input   ,st_integer,
st_length   ,st_loop    ,st_print   ,st_return,
st_scope    ,st_string  ,st_substr  ,st_then,
st_while    ,st_id      ,st_inthod  ,st_doublehodnota,
st_ret      ,st_konecsouboru,st_levzav,st_pravzav,
st_delwhole ,
} stav;

typedef struct tokens{
char* hodnota; //hodnota u int, double ...
stav state; //int, double, string, as, asc,  ...
int radek;
int counter;
} Token;

void scannerstring(char c);
void scannernumber(int c);
void scanner ();
void retezec();
void blokkoment();
