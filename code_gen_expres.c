#include <stdlib.h>
#include <stdio.h>
#include "code_gen_expres.h"
#include "expresion.h"

void expr_gen(int operator, const char* operand_1, const char* operand_2, const char* destination) {

    switch(operator)
    {
        case ex_mul:
        {
            printf("MUL %s %s %s\n", destination, operand_1, operand_2);
            break;
        }
        case ex_div:
        {
            printf("DIV %s %s %s\n", destination, operand_1, operand_2);
            break;
        }

        case ex_plus:
        {   
            // cisla
                printf("ADD %s %s %s\n", destination, operand_1, operand_2);
            // else stringy
            // printf("CONCAT %s %s %s\n", destination, operand_1, operand_2);
            break;
        }
        case ex_minus:
        {
            printf("SUB %s %s %s\n", destination, operand_1, operand_2);
            break;
        }
        case ex_equal:
        {
            // expr_gen(=,a, ,dest)
            // expr_gen(=,a,b,dest)
            if (operand_1 != NULL && operand_2 != NULL && destination != NULL)
                printf("EQ %s %s %s\n", destination, operand_1, operand_2);
            break;
        }
        case ex_less:
        {
            printf("LT %s %s %s\n", destination, operand_1, operand_2);
            break;
        }
        case ex_great:
        {
            printf("GT %s %s %s\n", destination, operand_1, operand_2);
            break;
        }
        default:
            fprintf(stderr, "chybička\n");
    }
    
}