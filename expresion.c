#define DEBUG

#include <stdio.h>
#include <stdlib.h>
#include "expresion.h"
#include "code_gen_expres.h"

#include "symbol.h"

 /**********************************ZASOBNIKOVE-OPERACE*********************************/
/**************************************************************************************/

void SInit (TStack *s)
{
    s->topPtr = NULL;
    s->activePtr = NULL;
}

int isTerminal (int symbol)
{
    return (symbol <= ex_dollar);
}

bool SEmpty (TStack *s)
{
    return (s->topPtr == NULL);
}

int SPush (TStack *s, int tokenType, char* string)
{
    TSElem *newElemPtr;
    if ( (newElemPtr = ((TSElem *) malloc(sizeof(TSElem)))) == NULL )
        return err_malloc;

    newElemPtr->type = tokenType;

    ////printf("str push:%s\n", string);
    str_create_init(&(newElemPtr->string), string);
    ////printf("push: %s\n",newElemPtr->string.data);
    newElemPtr->nextPtr = NULL;

    if (SEmpty(s))
        newElemPtr->prevPtr = NULL;
    else{
        s->topPtr->nextPtr = newElemPtr;
        newElemPtr->prevPtr = s->topPtr;
    }

    s->topPtr = newElemPtr;

    if(isTerminal(tokenType))
        s->activePtr = newElemPtr;
    return 0;
}

int SPostActiveInsert (TStack *s, int tokenType)
{
    if(s->activePtr == NULL){
        return err_nullPtr;
    }

    TSElem *newElemPtr;
    if ( (newElemPtr = ((TSElem *) malloc(sizeof(TSElem)))) == NULL )
        return err_malloc;

    str_create_init(&(newElemPtr->string), " ");

    newElemPtr->type = tokenType;
    newElemPtr->nextPtr = s->activePtr->nextPtr;
    newElemPtr->prevPtr = s->activePtr;
    s->activePtr->nextPtr = newElemPtr;

    if(newElemPtr->nextPtr != NULL){
        newElemPtr->nextPtr->prevPtr = newElemPtr;
    }
    else{
        s->topPtr = newElemPtr;
    }


    return 0;
}

void SPop (TStack *s)
{
    if (!SEmpty(s)) {
        TSElem *elemPtr = s->topPtr;
        TSElem *activeElemPtr = s->topPtr;

        s->topPtr = elemPtr->prevPtr; // prenese top na dalsi prvek

        while ((activeElemPtr->prevPtr != NULL) &&
                !isTerminal(activeElemPtr->type))
        {
            activeElemPtr = activeElemPtr->prevPtr;
        }
        s->activePtr = activeElemPtr;

        str_destroy(&(elemPtr->string));
        free(elemPtr);
    }
}

// vrati hodnotu na vrcholu zasobniku
int STopType (TStack *s)
{
    if (!SEmpty(s))
        return (s->topPtr->type);
    return err_StackEmpty;
}

char* STopString (TStack *s)
{
  if (!SEmpty(s)) {
        return (s->topPtr->string.data);
  }
  return NULL;

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
        s->topPtr = elemPtr->prevPtr; // top se prenese na dalsi (zrusi stary top)
        str_destroy(&(elemPtr->string));
        free(elemPtr);
    }
}

/***************************FUNKCE-PRECEDENCNI-ANALYZY*********************************/
/**************************************************************************************/
PrecTabValues prec_table[ex_dollar+2][ex_dollar+2] = {
/*         *   /   \   +   -   =   <>  <   <=  >   >=  (   )   i   int db  sr  bl  $   R */
/* *  */ { GT, GT, GT, GT, GT, GT, GT, GT, GT, GT, GT, LT, GT, LT, LT, LT, LT, LT, GT, LT },
/* /  */ { GT, GT, GT, GT, GT, GT, GT, GT, GT, GT, GT, LT, GT, LT, LT, LT, LT, LT, GT, LT },
/* \  */ { GT, GT, GT, GT, GT, GT, GT, GT, GT, GT, GT, LT, GT, LT, LT, LT, LT, LT, GT, LT },
/* +  */ { LT, LT, LT, GT, GT, GT, GT, GT, GT, GT, GT, LT, GT, LT, LT, LT, LT, LT, GT, LT },
/* -  */ { LT, LT, LT, GT, GT, GT, GT, GT, GT, GT, GT, LT, GT, LT, LT, LT, LT, LT, GT, LT },
/* =  */ { LT, LT, LT, LT, LT, GT, GT, GT, GT, GT, GT, LT, GT, LT, LT, LT, LT, LT, GT, LT },
/* <> */ { LT, LT, LT, LT, LT, GT, GT, GT, GT, GT, GT, LT, GT, LT, LT, LT, LT, LT, GT, LT },
/* <  */ { LT, LT, LT, LT, LT, GT, GT, GT, GT, GT, GT, LT, GT, LT, LT, LT, LT, LT, GT, LT },
/* <= */ { LT, LT, LT, LT, LT, GT, GT, GT, GT, GT, GT, LT, GT, LT, LT, LT, LT, LT, GT, LT },
/* >  */ { LT, LT, LT, LT, LT, GT, GT, GT, GT, GT, GT, LT, GT, LT, LT, LT, LT, LT, GT, LT },
/* >= */ { LT, LT, LT, LT, LT, GT, GT, GT, GT, GT, GT, LT, GT, LT, LT, LT, LT, LT, GT, LT },
/* (  */ { LT, LT, LT, LT, LT, LT, LT, LT, LT, LT, LT, LT, EQ, LT, LT, LT, LT, LT, XX, LT },
/* )  */ { GT, GT, GT, GT, GT, GT, GT, GT, GT, GT, GT, XX, GT, XX, XX, XX, XX, XX, XX, XX },
/* i  */ { GT, GT, GT, GT, GT, GT, GT, GT, GT, GT, GT, XX, GT, XX, LT, LT, LT, LT, GT, XX },
/* int*/ { GT, GT, GT, GT, GT, GT, GT, GT, GT, GT, GT, XX, GT, XX, XX, XX, XX, XX, GT, XX },
/* db */ { GT, GT, GT, GT, GT, GT, GT, GT, GT, GT, GT, XX, GT, XX, XX, XX, XX, XX, GT, XX },
/* sr */ { GT, GT, GT, GT, GT, GT, GT, GT, GT, GT, GT, XX, GT, XX, XX, XX, XX, XX, GT, XX },
/* bl */ { GT, GT, GT, GT, GT, GT, GT, GT, GT, GT, GT, XX, GT, XX, XX, XX, XX, XX, GT, XX },
/* $  */ { LT, LT, LT, LT, LT, LT, LT, LT, LT, LT, LT, LT, XX, LT, LT, LT, LT, LT, XX, LT },
/* R  */ { GT, GT, GT, GT, GT, GT, GT, GT, GT, GT, GT, XX, GT, XX, LT, LT, LT, LT, GT, XX }
};

int set_operator(){
    switch (token.t_state){
        case st_nas:
            return ex_mul;

        case st_del:
            return ex_div;

        case st_del_cele:
            return ex_wholeDiv;

        case st_scit:
            return ex_plus;

        case st_odcit:
            return ex_minus;

        case st_rovno:
            return ex_equal;

        case st_nerov:
            return ex_notEq;

        case st_mensi:
            return ex_less;

        case st_menrov:
            return ex_lessEq;

        case st_vetsi:
            return ex_great;

        case st_vetrov:
            return ex_greatEq;

        case st_levzav:
            return ex_leftBrac;

        case st_pravzav:
            return ex_rightBrac;

        case st_id:
            return ex_ident;

        case st_int_val:
            return ex_integer;
        case st_double_val:
        case st_exp_int:
        case st_exp_doub:
            return ex_double;

        case st_retez:
            return ex_str;

        case st_true:
        case st_false: //TODO: st_bool?
            return ex_bool;

        case st_eol:
        case st_stred:
        case st_dim:
        case st_input:
        case st_print:
        case st_if:
        case st_do:
        case st_then:
        case st_return:
        case st_end:
        	return ex_dollar;

        default:
            return -1;//TODO: ERROR

    }
}

int expresion_reduction(TStack *s, int instruction, int reduce_counter, Tstring *ret_string) {
    Tstate var_type;
    char * pom_integer= "&pomInt";
    char * pom_double = "&pomDouble";
    char * pom_string = "&pomString";

    char* context = (p == 0 ? "TF@" : "LF@");


    int symbol = STopType(s);

    if(symbol == ex_ident || symbol == ex_integer || symbol == ex_double ||
       symbol == ex_bool  || symbol == ex_str){ // R ---> i
         // do return stringu se uklada string identifikatoru, cisla, true/false nebo retezce

        str_rewrite_data(ret_string, STopString(s));

        if (instruction == ins_print) {
            if (symbol == ex_str) {
                 printf("WRITE string@%s\n",ret_string->data);
            } else {
                printf("WRITE %s%s\n",context, ret_string->data);
            }
        }

        if (reduce_counter == 0)  // jen prirazeni napr. a = b
        {
            if (symbol == ex_ident) {
                var_type = return_variable_type(ret_string->data);
                if (var_type == 0) {
                    fprintf(stderr, "expresion reduction, variable not declared\n");
                    return 0; //chyba
                }

                if (var_type == st_integer)
                    printf("MOV %s%s %s%s\n", context, pom_integer, context, ret_string->data);
                else if (var_type == st_double)
                    printf("MOV %s%s %s%s\n", context, pom_double, context, ret_string->data);
                else if (var_type == st_string)
                    printf("MOV %s%s %s%s\n", context, pom_string, context, ret_string->data);
            }  

            else if (symbol == ex_integer) {
                printf("MOV %s%s int@%s\n", context, pom_integer, ret_string->data);
            } else if (reduce_counter == 0 && symbol == ex_double) {
                printf("MOV %s%s float@%s\n", context, pom_integer, ret_string->data);
            }
        }

        printf("typ: %d\n", symbol);

        SPop(s);
        if(SEmpty(s)) return 0;
    }

    else if(symbol == ex_leftBrac){ // R ---> (R)
        SPop(s);
        symbol = STopType(s);
        if(symbol == ex_reduction){
            str_rewrite_data(ret_string, STopString(s));

            SPop(s);
            symbol = STopType(s);

            if(symbol == ex_rightBrac){
                SPop(s);
                if(SEmpty(s)) return 0;
            }
        }
    }

    else if(symbol == ex_reduction){ // R ---> R "operator" R
        Tstring operand_1;
        Tstring operand_2;

        str_create_init(&(operand_1),STopString(s));
        str_rewrite_data(ret_string, pom_integer);

        SPop(s);
        symbol = STopType(s);
        int operator = symbol;

        if(symbol == ex_mul   || symbol == ex_plus     || symbol == ex_minus  ||
           symbol == ex_div   || symbol == ex_wholeDiv || symbol == ex_equal  ||
           symbol == ex_notEq || symbol == ex_less     || symbol == ex_lessEq ||
           symbol == ex_great || symbol == ex_greatEq){
            SPop(s);

            symbol = STopType(s);
            if(symbol == ex_reduction){
                str_create_init(&(operand_2),STopString(s));

                SPop(s);
                if(SEmpty(s)){
                    var_type = return_variable_type(operand_1.data);
                if (var_type == 0) {
                    fprintf(stderr, "expresion reduction, variable not declared\n");
                    return 0; //chyba
                }
                    expr_gen(operator, operand_1.data, operand_2.data, ret_string->data, instruction);
                    str_destroy(&(operand_1));
                    str_destroy(&(operand_2));
                    return 0;
                }
                str_destroy(&(operand_1));
                str_destroy(&(operand_2));
            }
        }
    }

    return 1;

}

//hlavni funkce precedencni analyzy
int precedent_analysis(int instruction) {
    TStack stack;
    SInit(&stack);

    Tstring ret_string;
    str_create(&ret_string); //TODO: Petr Marek overit malloc

    int reduce_counter = 0;
    int error;  // pomocna promenna pro vraceni chyb
    int stacked_operator, input_operator; // promenne pro zasobnikovy a vstupni symbol

    error = SPush(&stack, ex_dollar, " "); //do prazdneho zasobniku vlozime znak dolaru
    if(error < 0){// nepodareny malloc prvnu zasobniku, ERROR
      str_destroy(&ret_string);
      SClean(&stack);
      return ERR_INTERN;
    }

    do{
    	stacked_operator = SActive(&stack); // nastavime aktivni prvek zasobniku
        if(stacked_operator < 0){// zasobnik je prazdny, ERROR
          str_destroy(&ret_string);
          SClean(&stack);
          return ERR_INTERN;
        }

        input_operator = set_operator(); //nastavime symbol vstupniho tokenu
        if(input_operator < 0 || input_operator > ex_reduction){// nevyhovujici symbol na vstupu
          str_destroy(&ret_string);
          SClean(&stack);
          return ERR_SYN;
        }
        switch (prec_table[stacked_operator][input_operator]){//porovnani operatoru v zasobniku a na vstupu
            case EQ:
            {
                error = SPush(&stack, input_operator, token.t_str.data); //vlozime vstupni symbol na zasobnik
                if(error < 0){// nepodareny malloc prvnu zasobniku, ERROR
                  str_destroy(&ret_string);
                  SClean(&stack);
                  return ERR_INTERN;
                }

                error = generate_token(); // generujeme dalsi token
                if(error != 0){// nepovedla se operace generate_token, ERROR
                  str_destroy(&ret_string);
                  SClean(&stack);
                  return error;
                }
                break;
            }

            case LT:
            {
                error = SPostActiveInsert(&stack, ex_rule_begin); // za aktivni prvek vlozime redukcni zavorku
                if (error < 0){//nepovedlo se vlozit za aktivni prvek, ERROR
                  str_destroy(&ret_string);
                  SClean(&stack);
                  return ERR_INTERN;
                }
                error = SPush(&stack, input_operator , token.t_str.data); //vlozime vstupni symbol na zasobnik
                if(error < 0){// nepodareny malloc prvnu zasobniku, ERROR
                  str_destroy(&ret_string);
                  SClean(&stack);
                  return ERR_INTERN;
                }

                error = generate_token(); // generujeme dalsi token
                if(error != 0){// nepovedla se operace generate_token, ERROR
                  str_destroy(&ret_string);
                  SClean(&stack);
                  return error;
                }

                input_operator = set_operator(); //nastavime symbol vstupniho tokenu
                if(input_operator < 0 || input_operator > ex_reduction){// nevyhovujici symbol na vstupu
                  str_destroy(&ret_string);
                  SClean(&stack);
                  return ERR_SYN;
                }
                break;
            }

            case GT:
            case XX:
            {
                TStack reduction_stack; // vytvorime pomocny redukcni zasobnik
                SInit(&reduction_stack);

                while(STopType(&stack) != ex_rule_begin){ //  dokud na hlavnim zasobniku nenarazime na ex_dollar

                    error = SPush(&reduction_stack, STopType(&stack), STopString(&stack)); // do pomoc. zasobniku pushneme symbol z vrcholu hl. zasobniku

                    if(error < 0){// nepodareny malloc prvnu zasobniku, ERROR
                      str_destroy(&ret_string);
                      SClean(&stack);
                      return ERR_INTERN;
                    }

                    SPop(&stack); // popneme vrchol hlavniho zasobniku
                }

                error = expresion_reduction(&reduction_stack, instruction, reduce_counter, &ret_string); // redukujeme vyraz na pomocnem zasobniku

                SClean(&reduction_stack); // rusime pomocny zasobnik

                if(error != 0){ // redukce se nezdarila
                  str_destroy(&ret_string);
                  SClean(&stack);
                  return ERR_SYN;
                }


                reduce_counter++;

                SPop(&stack); // ze zasobniku popujeme redukcni zavorku
                stacked_operator = SActive(&stack); // nastavujeme aktivni prvek zasobniku
                if(stacked_operator < 0){// zasobnik je prazdny, ERROR
                  str_destroy(&ret_string);
                  SClean(&stack);
                  return ERR_INTERN;
                }

                error = SPush(&stack, ex_reduction, ret_string.data); // vkladame redukovane pravidlo na zasobnik
                if(error < 0){// nepodareny malloc prvnu zasobniku, ERROR
                  str_destroy(&ret_string);
                  SClean(&stack);
                  return ERR_INTERN;
                }

                break;
            }


        }


    }while ((SActive(&stack) != ex_dollar ) || (input_operator != ex_dollar)); //dokud nezpracujeme cely vyraz

    str_destroy(&ret_string); //uvolneni pameti
    SClean(&stack); //uvolneni pameti

    return 0;
}
