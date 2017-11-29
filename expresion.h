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

int precedent_analysis(int print_command, int* destination);
int set_operator();
int expresion_reduction(TStack *s, int print_command, int reduce_counter, Tstring *ret_string, int convert);

typedef enum {
	LT, // <
	GT, // >
	EQ, // ==
	XX, // chyba
}PrecTabValues;

enum {
/*0 */	ex_mul,
/*1 */	ex_div,      /* / */
/*2 */	ex_wholeDiv, /* \ */
/*3 */	ex_plus,
/*4 */	ex_minus,
/*5 */	ex_equal, 	 /* =  */
/*6 */	ex_notEq, 	 /* <> */
/*7 */	ex_less,
/*8 */	ex_lessEq,
/*9 */	ex_great,
/*10*/	ex_greatEq,
/*11*/	ex_leftBrac, /* ( */
/*12*/	ex_rightBrac,
/*13*/	ex_ident,	 /* i */
/*14*/	ex_integer,
/*15*/	ex_double,
/*16*/	ex_str,
/*17*/	ex_bool,
/*18*/	ex_dollar, // zacatek zpracovani vyrazu
/*19*/	ex_red_int,
/*20*/	ex_red_double,
/*21*/	ex_red_str,
/*22*/	ex_red_bool,
/*23*/	ex_rule_begin, //prednostni symbol
} EXSymbols;

#endif
