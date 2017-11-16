#ifndef EXPRESION
#define EXPRESION


#define err_malloc -1
#define err_StackEmpty -2
#define err_nullPtr -3 

//Funkce precedencni analyzy
//#-------------------------------------------#//
bool operator_precedent_analysis();

//Definice datovych typu pr zasobnik
//#-------------------------------------------#//

typedef struct {
	Ttoken Stoken;
	int type;
	struct tselem *nextPtr;
} TSElem;

typedef struct {
	TSElem *topPtr;
	TSElem *activePtr;
} TStack;

typedef char TStack;

//funkce definujici zasobnik
//#-------------------------------------------#//

int isTerminal (int symbol);
void SInit (TStack *s);
bool SEmpty (TStack *s);
int SPush (TStack *s, int tokenType);
int SPop (TStack *s);
int STop (TStack *s);
int SActive(TStack *s);
void SClean (TStack *s);

//vycty stavu precedencni analyzi
//#-------------------------------------------#//

bool precedent_analysis();
int set_operator(TSElem Elem);

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
} EXSymbols;

enum {
	ex_oper,
}EX;


#endif