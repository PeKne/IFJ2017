#ifndef GEN_EXPR_H
#define GEN_EXPR_H

#include "lex.h"

//int gen_print(char *arg);
int expr_gen(int operator, char *operand_1, char *operand_2, char *destination, int instruction, Tstate dest_type);

Tstring gen_str;

#endif