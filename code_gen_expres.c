#include <stdlib.h>
#include <stdio.h>
#include "code_gen_expres.h"
#include "expresion.h"
#include "lex.h"
#include "symbol.h"
#include <string.h>
#include <ctype.h>

extern int p;

int isDouble(char * operand) {
    int operand_type = st_integer;
    int length = strlen(operand);
    for (int i = 0; i < length; i++)
        {   
            if (!isdigit(operand[i])) {
                operand_type = st_double;
            }
        }
    return operand_type;
}

int check_operand (char * operand) {
    int operand_type;
    if (!isdigit(operand[0])) {
        if (strcmp(operand,"&pomInteger") == 0)
            operand_type = st_id;
        else if (strcmp(operand,"&pomFloat") == 0)
            operand_type = st_id;        
        else if (strcmp(operand,"&pomString") == 0)
            operand_type = st_id;
        else if ((return_variable_type(operand)) == 0) {
            operand_type = st_string;
        }
        else {
            operand_type = st_id;
        }
        
    } else {
        operand_type = isDouble(operand);        
    }
    return operand_type;
}

void expr_gen(int operator, char* operand_1, char* operand_2, char* destination, int instruction) {

    int operand_1_type = check_operand(operand_1);
    int operand_2_type = check_operand(operand_2);

   /*printf("operand1_type %d\n",operand_1_type);
   printf("operand2_type %d\n",operand_2_type);*/

   Tstring context_1;
   Tstring context_2;

   str_create(&context_1);
   str_create(&context_2);

    if (operand_1_type == st_id) {
        if (p == 0) str_rewrite_data(&context_1, "TF@");
        else  str_rewrite_data(&context_1, "LF@");
    } else if (operand_1_type == st_integer){
        str_rewrite_data(&context_1, "int@");
    } else if (operand_1_type == st_double) {
        str_rewrite_data(&context_1, "float@");
    } else if (operand_1_type == st_string) {
        str_rewrite_data(&context_1, "string@");
    }

    if (operand_2_type == st_id) {
        if (p == 0) str_rewrite_data(&context_2, "TF@");
        else  str_rewrite_data(&context_2, "LF@");
    } else if (operand_2_type == st_integer){
        str_rewrite_data(&context_2, "int@");
    } else if (operand_2_type == st_double) {
        str_rewrite_data(&context_2, "float@");
    } else if (operand_2_type == st_string) {
        str_rewrite_data(&context_2, "string@");
    }

   char* context = (p == 0 ? "TF@" : "LF@");


   /*printf("context_1 %s\n",context_1.data);
   printf("context_2 %s\n",context_2.data);*/

    
    switch(operator) {
        
        case ex_mul:
        {
            printf("MUL %s%s %s%s %s%s\n", context, destination, context_1.data, operand_1, context_2.data, operand_2);
            break;
        }

        case ex_div:
        {
            printf("DIV %s%s %s%s %s%s\n", context, destination, context_1.data, operand_1, context_2.data, operand_2);
            break;
        }

        case ex_plus:
        {   
            if (operand_1_type == st_string)
                printf("CONCAT %s%s %s%s %s%s\n", context, destination, context_1.data, operand_1, context_2.data, operand_2);
            else 
                printf("ADD %s%s %s%s %s%s\n", context, destination, context_1.data, operand_1, context_2.data, operand_2);
            break;
        }

        case ex_minus:
        {
            printf("SUB %s%s %s%s %s%s\n", context, destination, context_1.data, operand_1, context_2.data, operand_2);
            break;
        }

        case ex_equal:
        {
            printf("EQ %s&pomBool %s%s %s%s\n", context, context_1.data, operand_1, context_2.data, operand_2);
            break;
        }

        case ex_less:
        {
            printf("LT %s&pomBool %s%s %s%s\n", context, context_1.data, operand_1, context_2.data, operand_2);
            break;
        }

        case ex_great:
        {
            printf("GT %s&pomBool %s%s %s%s\n", context, context_1.data, operand_1, context_2.data, operand_2);
            break;
        }

        default:
            fprintf(stderr, "Chyba v code_gen_expres %s%s %s%s %s%s\n", context, destination, context_1.data, operand_1, context_2.data, operand_2);

    }
    str_destroy(&context_1);
    str_destroy(&context_2);    
}