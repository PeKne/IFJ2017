#include <stdlib.h>
#include <stdio.h>
#include "code_gen_expres.h"
#include "expresion.h"

void expr_gen(int operator, const char* operand_1, const char* operand_2, const char* destination, int print_command) {

    switch(operator)
    {
        case ex_mul:
        {
            printf("MUL TF@%s TF@%s TF@%s\n", destination, operand_1, operand_2);
            break;
        }
        case ex_div:
        {
            printf("DIV TF@%s TF@%s TF@%s\n", destination, operand_1, operand_2);
            break;
        }

        case ex_plus:
        {   
            // cisla
                printf("ADD TF@%s TF@%s TF@%s\n", destination, operand_1, operand_2);
            // else stringy
            // printf("CONCAT %s %s %s\n", destination, operand_1, operand_2);
            break;
        }
        case ex_minus:
        {
            printf("SUB TF@%s TF@%s TF@%s\n", destination, operand_1, operand_2);
            break;
        }
        case ex_equal:
        {
            // expr_gen(=,a, ,dest)
            // expr_gen(=,a,b,dest)
            printf("EQ TF@%sBool %s TF@%s\n", destination, operand_1, operand_2);
            break;
        }
        case ex_less:
        {
            printf("LT TF@%sBool TF@%s TF@%s\n", destination, operand_1, operand_2);
            break;
        }
        case ex_great:
        {
            printf("GT TF@%sBool TF@%s TF@%s\n", destination, operand_1, operand_2);
            break;
        }
        default:
            fprintf(stderr, "chybička\n");
    }
    
}