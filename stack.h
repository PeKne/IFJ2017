#ifndef EXPRESION
#define EXPRESION

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "strings.h"

#define err_malloc -1
#define err_StackEmpty -2
#define err_nullPtr -3

//Funkce precedencni analyzy
//#-------------------------------------------#//


//vycty stavu precedencni analyzi
//#-------------------------------------------#//
/*
bool precedent_analysis();
int set_operator();
char* expresion_reduction(TStack *stack);*/

typedef enum {
	LT, // <
	GT, // >
	EQ, // ==
	XX, // chyba
}PrecTabValues;

enum {
	ex_mul,
	ex_div,      /* / */
	ex_wholeDiv, /* \ */
	ex_plus,
	ex_minus,
	ex_equal, 	 /* =  */
	ex_notEq, 	 /* <> */
	ex_less,
	ex_lessEq,
	ex_great,
	ex_greatEq,
	ex_leftBrac, /* ( */
	ex_rightBrac,
	ex_ident,	 /* i */
	ex_num,
	ex_str,
	ex_bool,
	ex_dollar, // zacatek zpracovani vyrazu
	ex_rule_begin, //prednostni symbol
	ex_reduction,
} EXSymbols;

enum {
	ex_oper,
}EX;
//Definice datovych typu pr zasobnik
//#-------------------------------------------#//

typedef struct tselem {
	int type;
	Tstring string;
	struct tselem *prevPtr;
	struct tselem *nextPtr;
} TSElem;

typedef struct {
	TSElem *topPtr;
	TSElem *activePtr;
} TStack;


//funkce definujici zasobnik
//#-------------------------------------------#//

int isTerminal (int symbol);
void SInit (TStack *s);
bool SEmpty (TStack *s);
int SPush (TStack *s, int tokenType , char* string);
int SPostActiveInsert (TStack *s, int tokenType);
int SPop (TStack *s);
int STopType (TStack *s);
char* STopString (TStack *s);
int SActive(TStack *s);
void SClean (TStack *s);

void gen(const char* ptr);

#endif
