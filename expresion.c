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
        free(elemPtr);
        //s->topPtr->nextPtr = NULL; TODO: jak tohle vyresit? mozna se jit vyspat...

        while ((activeElemPtr->prevPtr != NULL) && 
                !isTerminal(activeElemPtr->type))
        {
            activeElemPtr = activeElemPtr->prevPtr;
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
        s->topPtr = elemPtr->prevPtr; // top se prenese na dalsi (zrusi stary top)
        free(elemPtr);
    }
}

const char* oper_strings[] = {"*", "/", "\\", "+", "-", "=", "<>", "<", "<=", ">", ">=", "(", ")", "i", "num", "str", "bool", "$", "]", "R",};

void DBG_SPrint(TStack *s){
	TSElem *elemPtr;
	elemPtr = s->topPtr;
	printf("|TOP--->|%s", oper_strings[elemPtr->type]);
	while(elemPtr->prevPtr != NULL){
		elemPtr = elemPtr->prevPtr;
		printf("|%s", oper_strings[elemPtr->type]);
	}
    printf("|\n");
}


/***************************FUNKCE-PRECEDENCNI-ANALYZY*********************************/
/**************************************************************************************/
PrecTabValues prec_table[ex_dollar+1][ex_dollar+1] = {
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
    return (symbol <= ex_dollar);
}

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
        	return ex_dollar;

        default:
            return -1;//TODO: ERROR
       
    }
}

bool expresion_reduction(TStack *s){

    printf("REDUKCNI STACK: ");
    DBG_SPrint(s);
    int symbol = STop(s);

    if(symbol == ex_ident){ // E ---> i
        SPop(s);
            if(SEmpty(s)){
                return true;
            }
    }

    else if(symbol == ex_leftBrac){ // E ---> (E)
        SPop(s);
        symbol = STop(s);
        if(symbol == ex_reduction){
            SPop(s);
            symbol = STop(s);
            
            if(symbol == ex_rightBrac){
                if(SEmpty(s)){
                return true;
                }
            }
        }
    }

    else if(symbol == ex_reduction){ // E ---> E "operator" E
        SPop(s);
        symbol = STop(s);
        if(symbol == ex_mul   || symbol == ex_plus     || symbol == ex_minus  ||
           symbol == ex_div   || symbol == ex_wholeDiv || symbol == ex_equal  ||
           symbol == ex_notEq || symbol == ex_less     || symbol == ex_lessEq ||
           symbol == ex_great || symbol == ex_greatEq){
            SPop(s);
            symbol = STop(s);
            
            if(symbol == ex_reduction){
                SPop(s);
                if(SEmpty(s)){
                return true;
                }
            }
        }
    }

    return false;
    
}

bool precedent_analysis() {
	int i = 0; //DEBUG
    TStack stack;
    SInit(&stack);

    int error;
    int stacked_operator, input_operator;

    error = SPush(&stack, ex_dollar); //do prazdneho zasobniku vlozime znak dolaru
    if(error < 0){
    	//ERROR
    	printf("chyba mallocu\n");
    }

    do{
    	stacked_operator = SActive(&stack);

    	printf("iterace %d.\n", ++i);
        if(stacked_operator < 0){
            printf("//na zasobniku neni zadny terminal\n");
            //ERROR prazdny zasobnik
        }
        DBG_SPrint(&stack);
        input_operator = set_operator();
        if(input_operator < 0){
        	printf("//ERROR nevyhovujici symbol\n");
            //ERROR nevyhovujici symbol
        }

        switch (prec_table[stacked_operator][input_operator]){
            case EQ:
            	printf("case EQ\n\n");
                SPush(&stack, input_operator);
                generate_token();
                input_operator = set_operator();
                if(input_operator < 0){
        			printf("ERROR nevyhovujici symbol\n");
        		    //ERROR nevyhovujici symbol
       			}
                break;

            case LT:
            	printf("case LT\n");
                error = SPostActiveInsert(&stack, ex_rule_begin);
                if (error < 0){
                	printf("neexistuje aktivni prvek\n");
                }
                SPush(&stack, input_operator);
                DBG_SPrint(&stack);
                printf("\n");
                generate_token();
                input_operator = set_operator();
                if(input_operator < 0){
        			printf("ERROR nevyhovujici symbol\n");
        		    //ERROR nevyhovujici symbol
       			}
                break;

            case GT:
            	printf("case GT\n");
                TStack reduction_stack;
                SInit(&reduction_stack);

                while(STop(&stack) != ex_rule_begin){

                    SPush(&reduction_stack, STop(&stack)); // do pomoc. zasobniku pushneme symbol z vrcholu hl. zasobniku
                    SPop(&stack);

                }

                if(!expresion_reduction(&reduction_stack)){
                    //ERROR neexistujici pravidlo
                    printf("selhala redukce\n");
                    return false;
                }

                SClean(&reduction_stack);

                SPop(&stack);
                stacked_operator = SActive(&stack);
                SPush(&stack, ex_reduction);
                DBG_SPrint(&stack);
                printf("\n");
                break;

            case XX:
            	printf("case XX\n\n");

            	return false;
                //ERROR

        }


    }while ((SActive(&stack) != ex_dollar ) || (input_operator != ex_dollar));
    printf("opoustim case\n");
    SClean(&stack);


    return true;
}
 
