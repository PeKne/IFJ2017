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
#include "lexical.h"
#include <stdbool.h>


char* LAhodnota;
stav LAcurrentstav;
int LAradek =1;
int LAcounter =0;








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
    char c;
    char g;

    while (1)
    {
        c = getc(stdin);
        if(isspace(c)) //bílí znak
            {
            if(c=='\n') LAradek++;

            }
        else if(c==EOF) //konec souboru
            {
            LAcurrentstav = st_konecsouboru;
            LAcounter++;
            break;
            }
        else if(c=='_') //identifikator
        {
            scannerstring(g);
        }
        else if(c=='!') //stringbegin
             {
                 g = getc(stdin);
                 if(g=='"')
                 {
                     retezec();
                 }
                 else
                 {
                     //lexical error
                 }
             }
        else if(c=='+') //scitani
            {
                g = getc(stdin);
                if(isspace(g))
                {
                    LAcurrentstav =st_scit;
                    ungetc(g,stdin);
                    LAcounter++;
                    break;
                }
                else
                {
                    //lexikal error
                }
            }

        else if(c=='-') //odcitani
            {
                g = getc(stdin);
                if(isspace(g))
                {
                    LAcurrentstav =st_odcit;
                    ungetc(g,stdin);
                    LAcounter++;
                    break;
                }
                else
                {
                    //lexikal error
                }
            }

        else if(c=='*') //nasobeni
            {
                g = getc(stdin);
                if(isspace(g))
                {
                    LAcurrentstav =st_nas;
                    ungetc(g,stdin);
                    LAcounter++;
                    break;
                }
                else
                {
                    //lexikal error
                }
            }

        else if(c=='/') //deleni AND komentar
        {
            g = getc(stdin);
            if(g=='\'')
            {
                blokkoment();
            }
            else if (isspace(g))
            {
                LAcurrentstav = st_del;
                ungetc(g,stdin);
                LAcounter++;
                break;
            }
        }
        else if(c=='\\') //celociselne deleni
            {
                g = getc(stdin);
                if(isspace(g))
                {
                    LAcurrentstav =st_delwhole;
                    ungetc(g,stdin);
                    LAcounter++;
                    break;
                }
                else
                {
                    //lexikal error
                }
            }

        else if(c=='<') //mensi and mensirovno
            {
                g = getc(stdin);
                if(g=='>')
                {
                    g= getc(stdin);
                    if(!(isspace(g)))
                    {
                        //lexical error
                    }
                    ungetc(g,stdin);
                    LAcurrentstav = st_nerov;
                    LAcounter++;
                    break;
                }
                else if (g=='=')
                {
                    g= getc(stdin);
                    if(!(isspace(g)))
                    {
                        //lexical error
                    }
                    ungetc(g,stdin);
                    LAcurrentstav = st_menro;
                    LAcounter++;
                    break;

                }
                else if (isspace(g))
                {
                    ungetc(g,stdin);
                    LAcurrentstav = st_mensi;
                    LAcounter++;
                    break;
                }
                else
                {
                    //lexical error
                }
            }

        else if(c=='>') //vetsi and vetsirovno
        {
                g = getc(stdin);
                if (g=='=')
                {
                    g= getc(stdin);
                    if(!(isspace(g)))
                    {
                        //lexical error
                    }
                    ungetc(g,stdin);
                    LAcurrentstav = st_vetro;
                    LAcounter++;
                    break;

                }
                else if (isspace(g))
                {
                    ungetc(g,stdin);
                    LAcurrentstav = st_vetsi;
                    LAcounter++;
                    break;
                }
                else
                {
                    //lexical error
                }
        }
        else if(c=='=') //rovno
            {
                g = getc(stdin);
                if(isspace(g))
                {
                    LAcurrentstav =st_rovno;
                    ungetc(g,stdin);
                    LAcounter++;
                    break;
                }
                else
                {
                    //lexikal error
                }

            }
            //rovno
        else if((c>='a'&&c<='z')||(c>='A'||c<='Z')) //identifikator OR klicove slovo
            {
                scannerstring(c);
                break;
            }
        else if(c>='0'&&c<='9')//cislo
        {
            scannernumber(c);
            break;
        }
        else if(c=='\'') //radkovy komentar
            {
                while((c=getc(stdin))!='\n')
                {

                }
            LAradek++;
            }
        else if(c==')') //prava zavorka
            {
                    LAcurrentstav =st_pravzav;
                    LAcounter++;
                    break;
            }

        else if(c=='(') //leva zavorka
            {
                    LAcurrentstav =st_levzav;
                    LAcounter++;
                    break;
            }
    }

}
void scannerstring(char c) //scanner pro klicova slova a identifikatory
{
    char g;
    //zapis charu c do nekonecneho pole

    while(!(isspace(g = getc(stdin))))
    {
        g = tolower(g);
        if(!(isalpha(g)|| isdigit(g)))
        {
            //lexical error
        }
        //zapis do nekonecneho pole
    }
    ungetc(g,stdin); //navraceni bíleho znaku  do stdin

    /* ------ kontrola nekonecnyho stringu se klicovyma slovama - potreba nekonecny string

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
        LAcurrentstav = st_id
        LAcounter++;
        nekonecny string = hodnota
    }
    */

}
void scannernumber(int  c)
{
    //doplnit inzerce charakteru c do nekonečného pole.
    char g;
    bool prepE = true;
    bool prepDot = true;
    while(!(isspace(g = getc(stdin)))) // cyklus dokud nenarzíme na bílí znak - ukončení tokenu
    {
        if(isdigit(g))
        {
            //zapis do nekonečného pole
        }
        else if (tolower(g)=='e'&&prepE)
        {
            prepE=false;
            //zapis do nekonečného pole a kontrola
            if(!(isdigit(g=getc(stdin))))
            {
                //error
            }
            else ;//zapis do nekonečného pole

        }
        else if(g=='.'&&prepDot&&prepE)
        {
            prepDot=false;
            //zapis do nekonečného pole
            if(!(isdigit(g=getc(stdin))))
            {
                //error
            }
            else  ;//zapis do nekonečného pole

        }
        else
        {
            //error
        }
    }
    ungetc(g,stdin);
    if(!prepE||!prepDot)
    {
        LAcurrentstav = st_doublehodnota;
        LAcounter++;
    }
    else
    {
        LAcurrentstav = st_inthod;
        LAcounter++;
    }
}
void retezec () //doplnuje celou hodnotu stringu do retezce
{
    char c;
    char g;
    char i;
    while(1)
    {
        c = getc(stdin);
        if(c=='"')//prazdny string
        {
            LAcounter++;
            LAcurrentstav=st_ret;
            //hodnota = prazdna hodnota
        }
        if(c!='\\'&&c!=EOF&&c!='\n'&&(isalpha(c)||isdigit(c))) //dalsi char neni escape sekevnec ale je to char nebo cislo
        {
            //zapsani hodnoty c do nekonecneho stringu
            g =getc(stdin);
            if (g=='"') //ukonceni stringu
            {
                //hodnot = nekonecny string
                LAcounter++;
                LAcurrentstav=st_ret;
                break;
            }
            ungetc(g,stdin);
        }
        else if(c!=EOF&&c!='\n') //dalsi char je escape sekvence
        {
            c=getc(stdin);
            /*switch(c)    inzrce  do nekonecneho pole
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
                    string[delka]='\n';
                    string[delka+1]='\0';
                    delka++;
                }
            case 't':
                {
                    string[delka]='\t';
                    string[delka+1]='\0';
                    delka++;
                }
            }
            */
            if(c>='0'&&c<='2')
            {
                g = getc(stdin);
                if(g>='0'&&g<='5')
                {
                    i = getc(stdin);
                    if(i>='1'&&i<='5')
                    {
                        int character = (c-'0')*100+(g-'0')*10+(i-'0');
                        //nekonecne pole = \character
                    }
                    else
                    {
                        //lexical error
                    }

                }
                else
                {
                    //lexical error
                }
            }
            else
            {
                //lexical error
            }
        }
        else
        {
            //lexical error
        }

    }
}
void blokkoment()
{
    char c;
    while (1)
    {
        c = getc(stdin);
        if(c=='\n')
        {
            LAradek++;
        }
        if(c=='\'')
        {
            c = getc(stdin);
            if(c=='/')
            {
                break;
            }
        }
        if(c==EOF)
        {
            //lexical error
        }

    }
}



