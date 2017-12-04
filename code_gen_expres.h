/*
 Implementace prekladace imperativniho jazyka IFJ17
 Petr Marek,       login: xmarek66
 Jakub Stefanisin, login: xstefa22
 Petr Knetl,       login: xknetl00
*/

#ifndef GEN_EXPR_H
#define GEN_EXPR_H

#include "lex.h"

int expr_gen(int operator, char *operand_1, char *operand_2, char *destination, Tstate dest_type);

#endif