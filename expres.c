#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> 
#include "expres.h"

#define err_malloc -1
#define err_StackEmpty -2
#define err_nullPtr -3


/*const int transformSymbols[] = {
	[ex_mul] = ex_oper,
	[ex_div] = ex_div,
	[ex_wholeDiv] = ex_wholeDiv,
	[ex_plus] = ex_oper,
	[ex_minus] = ex_oper,
	[ex_equal] = ex_oper,
	[ex_notEq] = ex_oper,
	[ex_less] = ex_oper,
	[ex_lessEq] = ex_oper,
	[ex_great] = ex_oper,
	[ex_greatEq] = ex_oper,
	[ex_leftBrac] = ex_leftBrac,
	[ex_rightBrac] = ex_rightBrac,
	[ex_ident] = ex_ident,
	[ex_num] = ex_liter,
	[ex_str] = ex_liter,
	[ex_bool] = ex_liter,
	[ex_dollar] = ex_dollar,
}
*/


const int preced_table[][ex_dollar + 1] = {
/*         *   /   \   +   -   =   <>  <   <=  >   >=  (   )   i   nm  sr  bl  $  */
/* *  */ { GT, GT, GT, GT, GT, GT, GT, GT, GT, GT, GT, LT, GT, LT, LT, LT, LT, GT, },
/* /  */ { GT, GT, GT, GT, GT, GT, GT, GT, GT, GT, GT, LT, GT, LT, LT, LT, LT, GT, },
/* \  */ { GT, GT, GT, GT, GT, GT, GT, GT, GT, GT, GT, LT, GT, LT, LT, LT, LT, GT, },
/* +  */ { LT, LT, LT, GT, GT, GT, GT, GT, GT, GT, GT, LT, GT, LT, LT, LT, LT, GT, },
/* -  */ { LT, LT, LT, GT, GT, GT, GT, GT, GT, GT, GT, LT, GT, LT, LT, LT, LT, GT, },
/* =  */ { LT, LT, LT, LT, LT, GT, GT, GT, GT, GT, GT, LT, GT, LT, LT, LT, LT, GT, },
/* <> */ { LT, LT, LT, LT, LT, GT, GT, GT, GT, GT, GT, LT, GT, LT, LT, LT, LT, GT, },
/* <  */ { LT, LT, LT, LT, LT, GT, GT, GT, GT, GT, GT, LT, GT, LT, LT, LT, LT, GT, },
/* <= */ { LT, LT, LT, LT, LT, GT, GT, GT, GT, GT, GT, LT, GT, LT, LT, LT, LT, GT, },
/* >  */ { LT, LT, LT, LT, LT, GT, GT, GT, GT, GT, GT, LT, GT, LT, LT, LT, LT, GT, },
/* >= */ { LT, LT, LT, LT, LT, GT, GT, GT, GT, GT, GT, LT, GT, LT, LT, LT, LT, GT, },
/* (  */ { LT, LT, LT, LT, LT, LT, LT, LT, LT, LT, LT, LT, EQ, LT, LT, LT, LT, XX, },
/* )  */ { GT, GT, GT, GT, GT, GT, GT, GT, GT, GT, GT, XX, GT, XX, XX, XX, XX, XX, },
/* i  */ { GT, GT, GT, GT, GT, GT, GT, GT, GT, GT, GT, XX, GT, XX, LT, LT, LT, GT, },
/* nm */ { GT, GT, GT, GT, GT, GT, GT, GT, GT, GT, GT, XX, GT, XX, XX, XX, XX, GT, },
/* sr */ { GT, GT, GT, GT, GT, GT, GT, GT, GT, GT, GT, XX, GT, XX, XX, XX, XX, GT, },
/* bl */ { GT, GT, GT, GT, GT, GT, GT, GT, GT, GT, GT, XX, GT, XX, XX, XX, XX, GT, },
/* $  */ { LT, LT, LT, LT, LT, LT, LT, LT, LT, LT, LT, LT, XX, LT, LT, LT, LT, XX, },
};

typedef struct tselem {
	int type;
	struct tselem *nextPtr;
} TSElem;

typedef struct tstack {
	TSElem *topPtr;
	TSElem *activePtr;
} TStack;

/**************************************************************************************/
int isTerminal (int symbol) {
	return (symbol < ex_dollar);
}

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
		// zkopiruje top do elemPtr, aby se neztratil ukazatel na dalsi po zruseni top prvku
		elemPtr = s->topPtr; 
		s->topPtr = elemPtr->nextPtr; // top se prenese na dalsi (zrusi stary top)
		free(elemPtr);
	}
}

