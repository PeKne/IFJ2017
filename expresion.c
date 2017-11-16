#include "expresion.h"
#include "lex.h"


 /**********************************ZASOBNIKOVE-OPERACE*********************************/
/**************************************************************************************/

void SInit (TStack *s)
{
    s->topPtr = NULL;
    s->activePtr = NULL;
}

bool SEmpty (TStack *s)
{
    return (s->topPtr == NULL);
}

int SPush (TStack *s, int tokenType)
{
    TSElem *newElemPtr;
    if ( (newElemPtr = ((TSElem *) malloc(sizeof(TSElem)))) == NULL )
        return err_malloc;

    newElemPtr->type = tokenType;

    if (SEmpty(s))
        newElemPtr->nextPtr = NULL;
    else
        newElemPtr->nextPtr = s->topPtr;
    s->topPtr = newElemPtr;
    if(isTerminal(tokenType))
        s->activePtr = newElemPtr;
    return 0;
}

int SPop (TStack *s)
{
    if (!SEmpty(s)) {
        TSElem *elemPtr = s->topPtr;
        TSElem *activeElemPtr = s->topPtr;
        
        s->topPtr = elemPtr->nextPtr; // prenese top na dalsi prvek
        free(elemPtr);  

        while ((activeElemPtr->nextPtr != NULL) && 
                !isTerminal(activeElemPtr->type))
        {
            activeElemPtr = activeElemPtr->nextPtr;
        }
        s->activePtr = activeElemPtr;
    }
    return 0;
}

// vrati hodnotu na vrcholu zasobniku
int STop (TStack *s)
{
    if (!SEmpty(s))
        return (s->topPtr->type);
    return err_StackEmpty;
}

int SActive(TStack *s)
{
    if (s->activePtr != NULL)
        return (s->activePtr->type);
    return err_StackEmpty;
}

void SClean (TStack *s)
{
    TSElem *elemPtr;
    // slo by pouzit SPop, ale zbytecne by se predelavalo i activePtr
    while (!SEmpty(s)) {
        // zkopiruje top do elemPtr, aby se po zruseni top prvku neztratil ukazatel na dalsi prvek
        elemPtr = s->topPtr; 
        s->topPtr = elemPtr->nextPtr; // top se prenese na dalsi (zrusi stary top)
        free(elemPtr);
    }
}

/***************************FUNKCE-PRECEDENCNI-ANALYZY*********************************/
/**************************************************************************************/
PrecTabValues prec_table[ex_dollar+1][ex_dolar+1] = {
/*         *   /   \   +   -   =   <>  <   <=  >   >=  (   )   i   nm  sr  bl  $  */
/* *  */ { GT, GT, GT, GT, GT, GT, GT, GT, GT, GT, GT, LT, GT, LT, LT, LT, LT, GT },
/* /  */ { GT, GT, GT, GT, GT, GT, GT, GT, GT, GT, GT, LT, GT, LT, LT, LT, LT, GT },
/* \  */ { GT, GT, GT, GT, GT, GT, GT, GT, GT, GT, GT, LT, GT, LT, LT, LT, LT, GT },
/* +  */ { LT, LT, LT, GT, GT, GT, GT, GT, GT, GT, GT, LT, GT, LT, LT, LT, LT, GT },
/* -  */ { LT, LT, LT, GT, GT, GT, GT, GT, GT, GT, GT, LT, GT, LT, LT, LT, LT, GT },
/* =  */ { LT, LT, LT, LT, LT, GT, GT, GT, GT, GT, GT, LT, GT, LT, LT, LT, LT, GT },
/* <> */ { LT, LT, LT, LT, LT, GT, GT, GT, GT, GT, GT, LT, GT, LT, LT, LT, LT, GT },
/* <  */ { LT, LT, LT, LT, LT, GT, GT, GT, GT, GT, GT, LT, GT, LT, LT, LT, LT, GT },
/* <= */ { LT, LT, LT, LT, LT, GT, GT, GT, GT, GT, GT, LT, GT, LT, LT, LT, LT, GT },
/* >  */ { LT, LT, LT, LT, LT, GT, GT, GT, GT, GT, GT, LT, GT, LT, LT, LT, LT, GT },
/* >= */ { LT, LT, LT, LT, LT, GT, GT, GT, GT, GT, GT, LT, GT, LT, LT, LT, LT, GT },
/* (  */ { LT, LT, LT, LT, LT, LT, LT, LT, LT, LT, LT, LT, EQ, LT, LT, LT, LT, XX },
/* )  */ { GT, GT, GT, GT, GT, GT, GT, GT, GT, GT, GT, XX, GT, XX, XX, XX, XX, XX },
/* i  */ { GT, GT, GT, GT, GT, GT, GT, GT, GT, GT, GT, XX, GT, XX, LT, LT, LT, GT },
/* nm */ { GT, GT, GT, GT, GT, GT, GT, GT, GT, GT, GT, XX, GT, XX, XX, XX, XX, GT },
/* sr */ { GT, GT, GT, GT, GT, GT, GT, GT, GT, GT, GT, XX, GT, XX, XX, XX, XX, GT },
/* bl */ { GT, GT, GT, GT, GT, GT, GT, GT, GT, GT, GT, XX, GT, XX, XX, XX, XX, GT },
/* $  */ { LT, LT, LT, LT, LT, LT, LT, LT, LT, LT, LT, LT, XX, LT, LT, LT, LT, XX },
};


int isTerminal (int symbol) {
    return (symbol < ex_dollar);
}

int set_operator(TSElem Elem){
    switch (Elem.Stoken.t_state){
        case st_nas:
            Elem.type = ex_mul;
        
        case st_del:
            Elem.type = ex_div;

        case st_del_cele:
            Elem.type = ex_wholeDiv;
        
        case st_scit:
            Elem.type = ex_plus;
        
        case st_odcit:
            Elem.type = ex_minus;
        
        case st_rovno:
            Elem.type = ex_equal;
        
        case st_nerov:
            Elem.type = ex_notEq;
        
        case st_mensi:
            Elem.type = ex_less;
        
        case st_menrov:
            Elem.type = ex_lessEq;
        
        case st_vetsi:
            Elem.type = ex_great;
        
        case st_vetrov:
            Elem.type = ex_greatEq;
        
        case st_levzav:
            Elem.type = ex_leftBrac;
        
        case st_pravzav:
            Elem.type = ex_rightBrac;
        
        case st_id:
            Elem.type = ex_ident;

        case st_int_val: //TODO: cisla s exponetem, double, double s exponentem
            Elem.type = ex_num;
        
        case st_retez:
            Elem.type = ex_str;

        case st_bool: //TODO: st_bool?
            Elem.type = ex_bool;

        default:
            return 1;//TODO: ERROR
        }
        return 0;
    }
}

bool precedent_analysis() {
    TStack *stack;
    SInit(stack);

    TSElem stacked, input;
    stacked.type = ex_dollar;

    input.Stoken = token;
    if(set_operator(input) != 0){
        //ERROR
    }




}
 
