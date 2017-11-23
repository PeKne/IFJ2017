#include "expresion.h"
#include "code_gen_expres.h"
#include <stdio.h>
#include <stdlib.h>

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

int SPush (TStack *s, int tokenType, const char* string)
{
    TSElem *newElemPtr;
    if ( (newElemPtr = ((TSElem *) malloc(sizeof(TSElem)))) == NULL )
        return err_malloc;

    newElemPtr->type = tokenType;
    int length = strlen(string);
    char* new_string = (char*) malloc((++length)*sizeof(char));
    if (new_string == NULL){
      return err_malloc;
    }

    strcpy(new_string, string);
    newElemPtr->string = new_string;
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

    char* new_string = (char*) malloc(2*sizeof(char));
    if (new_string == NULL){
      free(newElemPtr);
      return err_malloc;
    }

    strcpy(new_string, " ");
    newElemPtr->string = new_string;

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

int SPop (TStack *s)
{
    if (!SEmpty(s)) {
        TSElem *elemPtr = s->topPtr;
        TSElem *activeElemPtr = s->topPtr;

        s->topPtr = elemPtr->prevPtr; // prenese top na dalsi prvek

      //  free((char*)elemPtr->string);
        while ((activeElemPtr->prevPtr != NULL) &&
                !isTerminal(activeElemPtr->type))
        {
            activeElemPtr = activeElemPtr->prevPtr;
        }
        s->activePtr = activeElemPtr;
        free(elemPtr);
    }
    return 0;
}

// vrati hodnotu na vrcholu zasobniku
int STopType (TStack *s)
{
    if (!SEmpty(s))
        return (s->topPtr->type);
    return err_StackEmpty;
}

const char* STopString (TStack *s)
{
  if (!SEmpty(s)) {
        return (s->topPtr->string);
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
        free(elemPtr);
    }
}

const char* oper_strings[] = {"*", "/", "\\", "+", "-", "=", "<>", "<", "<=", ">", ">=", "(", ")", "i", "num", "str", "bool", "$", "R", "]",};

void DBG_SPrint(TStack *s){
	TSElem *elemPtr;
	elemPtr = s->topPtr;
	printf("|TOP--->|%s", oper_strings[elemPtr->type]);
	while(elemPtr->prevPtr != NULL){
		elemPtr = elemPtr->prevPtr;
		printf("|%s", oper_strings[elemPtr->type]);
	}
    printf("|  SYMBOL\n");

    elemPtr = s->topPtr;
  	printf("|TOP--->|%s", elemPtr->string);
  	while(elemPtr->prevPtr != NULL){
  		elemPtr = elemPtr->prevPtr;
  		printf("|%s", elemPtr->string);
  	}
      printf("|  STRING\n");
}


/***************************FUNKCE-PRECEDENCNI-ANALYZY*********************************/
/**************************************************************************************/
PrecTabValues prec_table[ex_dollar+2][ex_dollar+2] = {
/*         *   /   \   +   -   =   <>  <   <=  >   >=  (   )   i   nm  sr  bl  $   R */
/* *  */ { GT, GT, GT, GT, GT, GT, GT, GT, GT, GT, GT, LT, GT, LT, LT, LT, LT, GT, LT },
/* /  */ { GT, GT, GT, GT, GT, GT, GT, GT, GT, GT, GT, LT, GT, LT, LT, LT, LT, GT, LT },
/* \  */ { GT, GT, GT, GT, GT, GT, GT, GT, GT, GT, GT, LT, GT, LT, LT, LT, LT, GT, LT },
/* +  */ { LT, LT, LT, GT, GT, GT, GT, GT, GT, GT, GT, LT, GT, LT, LT, LT, LT, GT, LT },
/* -  */ { LT, LT, LT, GT, GT, GT, GT, GT, GT, GT, GT, LT, GT, LT, LT, LT, LT, GT, LT },
/* =  */ { LT, LT, LT, LT, LT, GT, GT, GT, GT, GT, GT, LT, GT, LT, LT, LT, LT, GT, LT },
/* <> */ { LT, LT, LT, LT, LT, GT, GT, GT, GT, GT, GT, LT, GT, LT, LT, LT, LT, GT, LT },
/* <  */ { LT, LT, LT, LT, LT, GT, GT, GT, GT, GT, GT, LT, GT, LT, LT, LT, LT, GT, LT },
/* <= */ { LT, LT, LT, LT, LT, GT, GT, GT, GT, GT, GT, LT, GT, LT, LT, LT, LT, GT, LT },
/* >  */ { LT, LT, LT, LT, LT, GT, GT, GT, GT, GT, GT, LT, GT, LT, LT, LT, LT, GT, LT },
/* >= */ { LT, LT, LT, LT, LT, GT, GT, GT, GT, GT, GT, LT, GT, LT, LT, LT, LT, GT, LT },
/* (  */ { LT, LT, LT, LT, LT, LT, LT, LT, LT, LT, LT, LT, EQ, LT, LT, LT, LT, XX, LT },
/* )  */ { GT, GT, GT, GT, GT, GT, GT, GT, GT, GT, GT, XX, GT, XX, XX, XX, XX, XX, XX },
/* i  */ { GT, GT, GT, GT, GT, GT, GT, GT, GT, GT, GT, XX, GT, XX, LT, LT, LT, GT, XX },
/* nm */ { GT, GT, GT, GT, GT, GT, GT, GT, GT, GT, GT, XX, GT, XX, XX, XX, XX, GT, XX },
/* sr */ { GT, GT, GT, GT, GT, GT, GT, GT, GT, GT, GT, XX, GT, XX, XX, XX, XX, GT, XX },
/* bl */ { GT, GT, GT, GT, GT, GT, GT, GT, GT, GT, GT, XX, GT, XX, XX, XX, XX, GT, XX },
/* $  */ { LT, LT, LT, LT, LT, LT, LT, LT, LT, LT, LT, LT, XX, LT, LT, LT, LT, XX, LT },
/* R  */ { GT, GT, GT, GT, GT, GT, GT, GT, GT, GT, GT, XX, GT, XX, LT, LT, LT, GT, XX }
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
        case st_double_val:
        case st_exp_int:
        case st_exp_doub:
            return ex_num;

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
        case st_return:
        case st_end:

        	return ex_dollar;

        default:
            return -1;//TODO: ERROR

    }
}

const char* expresion_reduction(TStack *s, int assign) {

    printf("\nREDUKCNI STACK: \n");
    DBG_SPrint(s);

    const char* return_string;
    int symbol = STopType(s);

    if(symbol == ex_ident || symbol == ex_num ||
       symbol == ex_bool  || symbol == ex_str){ // R ---> i
         // do return stringu se uklada string identifikatoru, cisla, true/false nebo retezce
        return_string = STopString(s);
        SPop(s);
            if(SEmpty(s)){
                return return_string;
            }
    }

    else if(symbol == ex_leftBrac){ // R ---> (R)
        SPop(s);
        symbol = STopType(s);
        if(symbol == ex_reduction){
            return_string = STopString(s);
            SPop(s);
            symbol = STopType(s);

            if(symbol == ex_rightBrac){
              SPop(s);
                if(SEmpty(s)){
                return return_string;
                }
            }
        }
    }

    else if(symbol == ex_reduction){ // R ---> R "operator" R
        const char* operand_1 = STopString(s);
        return_string = "pom";
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
              const char* operand_2 = STopString(s);
                SPop(s);
                if(SEmpty(s)){
                    // TODO jak zjistim jestli je '=' prirazeni nebo porovnavani
                    expr_gen(assign, operator, operand_1, operand_2, return_string);
                    return return_string;
                }
            }
        }
    }

    return NULL;

}

bool precedent_analysis(int assign) {
    const char* top;//DEBUG
    TStack stack;
    SInit(&stack);

    int error;
    int stacked_operator, input_operator;

    error = SPush(&stack, ex_dollar, " "); //do prazdneho zasobniku vlozime znak dolaru
    if(error < 0){
    	//ERROR
    	printf("chyba mallocu\n");
    }
    STopString(&stack);
    printf("Inicializovany stack\n\n" );
    DBG_SPrint(&stack);
    printf("\n");
    do{
    	stacked_operator = SActive(&stack);

        if(stacked_operator < 0){
            printf("//na zasobniku neni zadny terminal\n");
            //ERROR prazdny zasobnik
        }
        input_operator = set_operator();
        if(input_operator < 0){
        	printf("//ERROR nevyhovujici symbol\n");
            //ERROR nevyhovujici symbol
        }

        switch (prec_table[stacked_operator][input_operator]){
            case EQ:
            {
            	  printf("case EQ\n\n");
                SPush(&stack, input_operator, token.t_str.data);
                generate_token();
                break;
            }

            case LT:
            {
            	printf("case LT\n");
                error = SPostActiveInsert(&stack, ex_rule_begin);
                if (error < 0){
                	printf("neexistuje aktivni prvek\n");
                }


                SPush(&stack, input_operator , token.t_str.data);

                STopString(&stack);
                DBG_SPrint(&stack);

                printf("\n");
                generate_token();
                STopString(&stack);
                input_operator = set_operator();
                if(input_operator < 0){
        			printf("ERROR nevyhovujici symbol\n");
        		    //ERROR nevyhovujici symbol
       			}

                break;
            }

            case GT:
            case XX:
            {
            	printf("case GT\n");
                TStack reduction_stack;
                SInit(&reduction_stack);

                while(STopType(&stack) != ex_rule_begin){
                    SPush(&reduction_stack, STopType(&stack), STopString(&stack)); // do pomoc. zasobniku pushneme symbol z vrcholu hl. zasobniku
                    SPop(&stack);

                }
                const char* reduced_string = expresion_reduction(&reduction_stack, assign);
                SClean(&reduction_stack);
                if(reduced_string == NULL){
                    //ERROR neexistujici pravidlo
                    printf("ERROR\n");
                    SClean(&stack);
                    return false;
                }


                SPop(&stack);
                stacked_operator = SActive(&stack);
                SPush(&stack, ex_reduction, reduced_string); //TODO: co ches?
                DBG_SPrint(&stack);
                printf("\n");
                break;
            }


        }


    }while ((SActive(&stack) != ex_dollar ) || (input_operator != ex_dollar));
    printf("opoustim case\n");
    SClean(&stack);

    return true;
}
