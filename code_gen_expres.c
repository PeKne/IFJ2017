#include <stdlib.h>
#include <stdio.h>
#include "code_gen_expres.h"
#include "expresion.h"

extern int p;

void expr_gen(int operator, const char* operand_1, const char* operand_2, const char* destination, int print_command) {

    switch(operator)
    {
        const char * context = (p == 0) ? "@TF" : "@LF";

        case ex_mul:
        {
            printf("MUL %s%s %s%s %s%s\n", context, destination, context, operand_1, context, operand_2);
            break;
        }
        case ex_div:
        {
            printf("DIV %s%s %s%s %s%s\n", context, destination, context, operand_1, context, operand_2);
            break;
        }

        case ex_plus:
        {   
            // cisla
                printf("ADD %s%s %s%s %s%s\n", context, destination, context, operand_1, context, operand_2);
            // else stringy
            // printf("CONCAT %s %s %s\n", destination, operand_1, operand_2);
            break;
        }
        case ex_minus:
        {
            printf("SUB %s%s %s%s %s%s\n", context, destination, context, operand_1, context, operand_2);
            break;
        }
        case ex_equal:
        {
            printf("EQ %s%sBool %s%s %s%s\n", context, destination, context, operand_1, context, operand_2);
            break;
        }
        case ex_less:
        {
            printf("LT %s%sBool %s%s %s%s\n", context, destination, context, operand_1, context, operand_2);
            break;
        }
        case ex_great:
        {
            printf("GT %s%sBool %s%s %s%s\n", context, destination, context, operand_1, context, operand_2);
            break;
        }
        default:
            fprintf(stderr, "chybička v expr_gen\n");
    }
    
}