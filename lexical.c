/*
Ifj projekt 2017

Lexikální analyzátor



*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include <string.h>
#include <limits.h>

typedef enum{
st_del,
st_nas,
st_scit,
st_odcit,
st_mensi,
st_vetsi,
st_menro,
st_vetro,
st_rovno,
st_nerov,
st_r_kom,
st_b_komZ,
st_b_komK,
st_and,      //rezervovana slova
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
st_true, //rezervovana slova
st_as, //klic slova
st_declare,
st_dim,
st_do,
st_double,
St_else,
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
st_while, //klic slova
st_id,
st_retLZ,
st_retLK,
st_str,
st_int,
st_doublehodnota,
st_ret,
st_prazdny,
} stav;




typedef struct token{
char* hodnota; //hodnota u int, double ...
char* nazev; //int, double, string, as, asc,  ...
} Token;

void scannerstring(char *string, char ***nazev, char ***hodnota);
void scannernumber(char *string, char ***nazev, char ***hodnota);
void scannerznak(char *string, char ***nazev, char ***hodnota);
void scanner (char **nazev, char **hodnota);

Token generovattoken() //generovani tokenu pro analyzu
{
    char* nazev;
    char* hodnota;
    scanner(&nazev,&hodnota);
  Token tken;
  tken.hodnota = hodnota;
  tken.nazev = nazev;

  return tken;
}
void scanner(char **nazev,char **hodnota)
{
    char string[1000];
    scanf("%s",string);
    if(string[0]>='a'&&string[0]<='z') //zacina pismenem
    {
        scannerstring(string,&nazev,&hodnota);
    }
    else if (string[0]>='0'&&string[0]<='9') //zacina cislem
    {
        scannernumber(string,&nazev,&hodnota);
    }
    else                                    //zacina znakem
    {
        scannerznak(string,&nazev,&hodnota);
    }
}
void scannerstring(char *string, char ***nazev, char ***hodnota) //skanuje vsechny moznosti ktere zacinaji charem
{
    if(strcmp(string,"as")==0)
    {

    }
    else if(strcmp(string,"asc")==0)
    {

    }
    else if(strcmp(string,"declare")==0)
    {

    }
    else if(strcmp(string,"dim")==0)
    {

    }
    else if(strcmp(string,"do")==0)
    {

    }
    else if(strcmp(string,"double")==0)
    {

    }
    else if(strcmp(string,"else")==0)
    {

    }
    else if(strcmp(string,"end")==0)
    {

    }
    else if(strcmp(string,"chr")==0)
    {

    }
    else if(strcmp(string,"function")==0)
    {

    }
    else if(strcmp(string,"if")==0)
    {

    }
    else if(strcmp(string,"input")==0)
    {

    }
    else if(strcmp(string,"integer")==0)
    {

    }
    else if(strcmp(string,"length")==0)
    {

    }
    else if(strcmp(string,"loop")==0)
    {

    }
    else if(strcmp(string,"print")==0)
    {

    }
    else if(strcmp(string,"return")==0)
    {

    }
    else if(strcmp(string,"scope")==0)
    {

    }
    else if(strcmp(string,"string")==0)
    {

    }
    else if(strcmp(string,"substr")==0)
    {

    }
    else if(strcmp(string,"then")==0)
    {

    }
    else if(strcmp(string,"while")==0)
    {

    }
    else if(strcmp(string,"and")==0)
    {

    }
    else if(strcmp(string,"boolean")==0)
    {

    }
    else if(strcmp(string,"continue")==0)
    {

    }
    else if(strcmp(string,"elseif")==0)
    {

    }
    else if(strcmp(string,"exit")==0)
    {

    }
    else if(strcmp(string,"false")==0)
    {

    }
    else if(strcmp(string,"for")==0)
    {

    }
    else if(strcmp(string,"next")==0)
    {

    }
    else if(strcmp(string,"not")==0)
    {

    }
    else if(strcmp(string,"or")==0)
    {

    }
    else if(strcmp(string,"shared")==0)
    {

    }
    else if(strcmp(string,"static")==0)
    {

    }
    else if(strcmp(string,"true")==0)
    {

    }
    else
    {
        printf("identifikator");
    }
}
void scannernumber(char *string, char ***nazev, char ***hodnota) //skanuje vsechny moznosti ktere zacinaji cislem
{
    printf("cislo je to ");
}
void scannerznak(char *string, char ***nazev, char ***hodnota) //skanuje vsechny stringy ktere zacinaji specialni znakem
{
    if(string[0]=='!'&&string[1]=='"')
    {
        printf("retezec");
    }
    if(string=="+")
    {

    }
        if(string=="-")
    {

    }
        if(string=="*")
    {

    }
        if(string=="/")
    {

    }
        if(string=="<")
    {

    }
        if(string==">")
    {

    }
        if(string=="<=")
    {

    }
        if(string==">=")
    {

    }
        if(string=="=")
    {

    }
        if(string=="<>")
    {

    }
}





