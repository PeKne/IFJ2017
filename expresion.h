#ifndef EXPRESION
#define EXPRESION

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "lex.h"
#include "string.h"
#include "errors.h"

#define err_malloc -1
#define err_StackEmpty -2
#define err_nullPtr -3

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
void SPop (TStack *s);
int STopType (TStack *s);
char* STopString (TStack *s);
int SActive(TStack *s);
void SClean (TStack *s);

//vycty stavu precedencni analyzi
//#-------------------------------------------#//

int precedent_analysis(int print_command);
int set_operator();
int expresion_reduction(TStack *s, int print_command, int reduce_counter, Tstring *ret_string);

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
	ex_integer,
	ex_double,
	ex_str,
	ex_bool,
	ex_dollar, // zacatek zpracovani vyrazu
	ex_reduction,
	ex_rule_begin, //prednostni symbol
} EXSymbols;

#endif
