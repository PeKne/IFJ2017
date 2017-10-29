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
st_id,

st_inthod,
st_doublehodnota,
st_ret,
st_konecsouboru,
st_levzav,
st_pravzav,
} stav;

char* LAhodnota;
stav LAcurrentstav;
int LAradek =1;
int LAcounter =0;
int LAkom=0; //pomocna promena pro pocitani radku



typedef struct tokens{
char* hodnota; //hodnota u int, double ...
stav state; //int, double, string, as, asc,  ...
int radek;
int counter;
} Token;

void scannerstring(char *string);
void scannernumber(char *string);
void scannerznak(char *string);
void scanner ();
void retezec(char *string);

Token generovattoken() //generovani tokenu pro analyzu
{
    Token token;
    scanner();

    token.hodnota =LAhodnota;
    token.state=LAcurrentstav;
    token.counter=LAcounter;
    token.radek=LAradek;
    return token;

}
void scanner()
{
    char string[1000];
    scanf("%s",string);
    for(int i =0;i<(strlen(string));i++)
        {
            string[i]=(tolower(string[i]));
        }
    if(string[0]>='a'&&string[0]<='z') //zacina pismenem
    {

        scannerstring(string);

    }
    else if (string[0]>='0'&&string[0]<='9') //zacina cislem
    {
        scannernumber(string);
    }
    else                                    //zacina znakem
    {
        scannerznak(string);
    }
    char c=getc(stdin);
    if(c=='\n'&&(LAkom==0||LAkom==2))
    {
        LAradek++;
    }
    else if(c==EOF)
    {
        LAcurrentstav = st_konecsouboru;
        LAcounter++;
    }
    if(LAkom!=0)
    {
        LAkom=0;
        scanner();
    }


}
void scannerstring(char *string) //skanuje vsechny moznosti ktere zacinaji charem COMPLETE
{
    if(strcmp(string,"as")==0)
    {
        LAcurrentstav = st_as;
        LAcounter++;
    }
    else if(strcmp(string,"asc")==0)
    {
        LAcurrentstav = st_asc;
        LAcounter++;
    }
    else if(strcmp(string,"declare")==0)
    {
        LAcurrentstav = st_declare;
        LAcounter++;
    }
    else if(strcmp(string,"dim")==0)
    {
        LAcurrentstav = st_dim;
        LAcounter++;
    }
    else if(strcmp(string,"do")==0)
    {
        LAcurrentstav = st_do;
        LAcounter++;
    }
    else if(strcmp(string,"double")==0)
    {
        LAcurrentstav = st_double;
        LAcounter++;
    }
    else if(strcmp(string,"else")==0)
    {
        LAcurrentstav = st_else;
        LAcounter++;
    }
    else if(strcmp(string,"end")==0)
    {
        LAcurrentstav = st_end;
        LAcounter++;
    }
    else if(strcmp(string,"chr")==0)
    {
        LAcurrentstav = st_chr;
        LAcounter++;
    }
    else if(strcmp(string,"function")==0)
    {
        LAcurrentstav = st_function;
        LAcounter++;
    }
    else if(strcmp(string,"if")==0)
    {
        LAcurrentstav = st_if;
        LAcounter++;
    }
    else if(strcmp(string,"input")==0)
    {
        LAcurrentstav = st_input;
        LAcounter++;
    }
    else if(strcmp(string,"integer")==0)
    {
        LAcurrentstav = st_integer;
        LAcounter++;
    }
    else if(strcmp(string,"length")==0)
    {
        LAcurrentstav = st_length;
        LAcounter++;
    }
    else if(strcmp(string,"loop")==0)
    {
        LAcurrentstav = st_loop;
        LAcounter++;
    }
    else if(strcmp(string,"print")==0)
    {
        LAcurrentstav = st_print;
        LAcounter++;
    }
    else if(strcmp(string,"return")==0)
    {
        LAcurrentstav = st_return;
        LAcounter++;
    }
    else if(strcmp(string,"scope")==0)
    {
        LAcurrentstav = st_scope;
        LAcounter++;
    }
    else if(strcmp(string,"string")==0)
    {
        LAcurrentstav = st_string;
        LAcounter++;
    }
    else if(strcmp(string,"substr")==0)
    {
        LAcurrentstav = st_substr;
        LAcounter++;
    }
    else if(strcmp(string,"then")==0)
    {
        LAcurrentstav = st_then;
        LAcounter++;
    }
    else if(strcmp(string,"while")==0)
    {
        LAcurrentstav = st_while;
        LAcounter++;
    }
    else if(strcmp(string,"and")==0)
    {
        LAcurrentstav = st_and;
        LAcounter++;
    }
    else if(strcmp(string,"boolean")==0)
    {
        LAcurrentstav = st_boolean;
        LAcounter++;
    }
    else if(strcmp(string,"continue")==0)
    {
        LAcurrentstav = st_continue;
        LAcounter++;
    }
    else if(strcmp(string,"elseif")==0)
    {
        LAcurrentstav = st_elseif;
        LAcounter++;
    }
    else if(strcmp(string,"exit")==0)
    {
        LAcurrentstav = st_exit;
        LAcounter++;
    }
    else if(strcmp(string,"false")==0)
    {
        LAcurrentstav = st_false;
        LAcounter++;
    }
    else if(strcmp(string,"for")==0)
    {
        LAcurrentstav = st_for;
        LAcounter++;
    }
    else if(strcmp(string,"next")==0)
    {
        LAcurrentstav = st_next;
        LAcounter++;
    }
    else if(strcmp(string,"not")==0)
    {
        LAcurrentstav = st_not;
        LAcounter++;
    }
    else if(strcmp(string,"or")==0)
    {
        LAcurrentstav = st_or;
        LAcounter++;
    }
    else if(strcmp(string,"shared")==0)
    {
        LAcurrentstav = st_shared;
        LAcounter++;
    }
    else if(strcmp(string,"static")==0)
    {
        LAcurrentstav = st_static;
        LAcounter++;
    }
    else if(strcmp(string,"true")==0)
    {
        LAcurrentstav = st_true;
        LAcounter++;
    }
    else
    {
        for(int i =0;i<strlen(string);i++)
        {
            if(!((string[i]>='a'&&string[i]<='z')||(string[i]>='0'&&string[i]<='9')))
            {
                //error neznamy char v ID
            }
        }
        LAcurrentstav =st_id;
        LAhodnota = string;
        LAcounter++;

    }
}
void scannernumber(char *string) //skanuje vsechny moznosti ktere zacinaji cislem COMPLETE
{
    int prepinacdes =0;
    int prepchyb =0;
    for(int i =0;i<strlen(string);i++)
    {
        if((string[i]<'0'||string[i]>'9')&&prepinacdes==0)
        {
            if(string[i]=='e'||string[i]=='E'||string[i]=='.')
            {
                if((string[i+1]>='0'&&string[i+1]<='9')||(string[i+1]=='+'||string[i+1]=='-'))
                {
                    prepinacdes = 1;

                }
                else
                {
                    //error za exponentem nejsou cisla
                }
            }
            else
            {
                //error neidentifikovatelny znak v cislu
            }
        }
        else if(string[i]<'0'||string[i]>'9')
            {
               //error neidentifikovatelnz znak v cislu
            }
    }
    if(prepchyb==0&&prepinacdes==0)
    {
        LAcurrentstav=st_inthod;
        LAhodnota =string;
        LAcounter++;
    }
    else if(prepchyb==0&&prepinacdes==1)
    {
        LAcurrentstav=st_doublehodnota;
        LAhodnota =string;
        LAcounter++;
    }
}
void scannerznak(char *string) //skanuje vsechny stringy ktere zacinaji specialni znakem
{
    if(string[0]=='(')
    {
        LAcurrentstav ==st_levzav;
        LAcounter++;
    }
    if(string[0]==')')
    {
        LAcurrentstav=st_pravzav;
        LAcounter++;
    }
    if(string[0]=='_')
    {
        for(int i =1;i<strlen(string);i++)
        {
            if(!((string[i]>='a'&&string[i]<='z')||(string[i]>='0'||string[i]<='9')))
            {
                //error neznamy char v ID
            }
        }
        LAcurrentstav = st_id;
        LAhodnota = string;
        LAcounter++;

    }
    if(string[0]=='!'&&string[1]=='"')
    {
        retezec(string);
    }
    if(string=="+")
    {
        LAcurrentstav = st_scit;
        LAcounter++;
    }
        if(string=="-")
    {
        LAcurrentstav = st_odcit;
        LAcounter++;
    }
        if(string=="*")
    {
        LAcurrentstav = st_nas;
        LAcounter++;
    }
        if(string=="/")
    {
        LAcurrentstav = st_del;
        LAcounter++;
    }
        if(string=="<")
    {
        LAcurrentstav = st_mensi;
        LAcounter++;
    }
        if(string==">")
    {
        LAcurrentstav = st_vetsi;
        LAcounter++;
    }
        if(string=="<=")
    {
        LAcurrentstav = st_menro;
        LAcounter++;
    }
        if(string==">=")
    {
        LAcurrentstav = st_vetro;
        LAcounter++;
    }
        if(string=="=")
    {
        LAcurrentstav = st_rovno;
        LAcounter++;
    }
        if(string=="<>")
    {
        LAcurrentstav = st_nerov;
        LAcounter++;
    }
        if(string[0]=='\'')
        {
            while(getc(stdin)!='\n')
            {

            }
            LAradek++;
            LAkom=1;
        }
        else if(string[0]=='\\'&&string[1]=='\'')
        {
            char c;
            while(1)
            {
                c =getc(stdin);
                if(c =='\n')
                {
                    LAradek++;
                }
                else if (c==EOF)
                {
                    LAcurrentstav = st_konecsouboru;
                    LAcounter++;
                }
                else if (c=='\'')
                {
                    c=getc(stdin);
                    if(c=='\\')
                    {
                        LAkom=2;
                        break;
                    }
                }

            }
        }

}
void retezec (char *string) //doplnuje celou hodnotu stringu do retezce
{
    int delka = strlen(string);
    for(int i =0;i<(strlen(string));i++) //kontrola  puvodniho stringu
    {
        if(string[i]!='\\'&&string[i+1]=='"')  //kontrola lomitka pred uvozovkama
        {
            if(string[i+2]=='\0')  //uvozovka je posledni znak stringu
                {
                    LAhodnota =string;
                    LAcounter++;
                    LAcurrentstav = st_ret;

                }
            else //ukonceni stringu nasleduje char
            {
                //error neplatny string
            }
        }
        if(string[i]=='\\') //char v strignu je lomitko
        {

        }
        else //char v stringu neni lomitko
        {

        }
    }
    while(1)
    {
        char c = getc(stdin);
        if(c!='\\') //dalsi char neni lomitko
        {
            char g =getc(stdin);
            if (g=='"') //ukonceni stringu
            {
                string[delka]=c;
                string[delka+1]='\0';
                LAhodnota =string;
                LAcounter++;
                LAcurrentstav=st_ret;
                break;
            }
            else   //neukonecni stringu
            {
                if(c==EOF||g==EOF||c=='\n'||g=='\n')
                {
                    //error
                    break;
                }
                string[delka]=c;
                string[delka+1]=g;
                string[delka+2]='\0';
                delka=delka+2;
            }
        }
        else //dalsi char je lomitko
        {
            c=getc(stdin);
            switch(c)
            {
            case '/':
                {
                    string[delka]=c;
                    string[delka+1]='\0';
                    delka++;
                }
            case '"':
                {
                    string[delka]=c;
                    string[delka+1]='\0';
                    delka++;
                }
            case 'n':
                {
                    string[delka]=c;
                    string[delka+1]='\0';
                    delka++;
                }
            case 't':
                {
                    string[delka]=c;
                    string[delka+1]='\0';
                    delka++;
                }
            }
            if(c>='0'||c<='9')
            {
                char g =getc(stdin);
                char f =getc(stdin);
                if(isdigit(g)&&isdigit(f))
                {
                    int complete = c*100+g*10+f;
                    string[delka]=complete;
                    string[delka+1]='\0';
                    delka++;

                }
                else
                {
                    //error
                    break;
                }
            }
        }
    }
}




