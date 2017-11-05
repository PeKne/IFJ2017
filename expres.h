#ifndef EXPRES_H
#define EXPRES_H

enum {
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