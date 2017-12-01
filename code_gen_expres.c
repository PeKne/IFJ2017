#include <stdlib.h>
#include <stdio.h>
#include "expresion.h"
#include "code_gen_expres.h"
#include "lex.h"
#include "symbol.h"
#include <string.h>
#include <ctype.h>

extern int p;
extern char *str;
/*
int gen_print(char *arg) {
    int error = 0;
    char *str;
    asprintf(&str, arg);
    printf("str: %s\n",str);
    int len = strlen(str);
    for (int i = 0; i < len; i++)
    {
        error = str_push_char(&gen_str, str[i]);
        if (error) { str_destroy(&gen_str); return error; }
    }
    free(str);
    return 0;
}
*/
int isDouble(char *operand) {
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

int check_operand (char *operand) {
    int operand_type;
    if (!isdigit(operand[0])) {
        if ((strcmp(operand,"GF@&pomInteger") == 0) || (strcmp(operand,"GF@&pomFloat")  == 0)  ||  
            (strcmp(operand,"GF@&pomFloat")   == 0) || (strcmp(operand,"GF@&pomDouble") == 0)  ||
            (strcmp(operand,"GF@&pomBool")    == 0) || (strcmp(operand,"GF@&pomType")   == 0))
            operand_type = 0; // neprida se zadny kontext - str_create vytvori prazdny retezec

        else if ((return_variable_type(operand)) == 0) {
            operand_type = st_string;         
        } else {
            operand_type = st_id;
        }
        
    } else {
        operand_type = isDouble(operand);        
    }
    return operand_type;
}

int set_context(Tstring *context, int operand_type, Tstate dest_type, Tstring *operand) {  
    double converted = 0.0;  
    if (operand_type == st_id) {
        if (p == 0) str_rewrite_data(context, "TF@");
        else        str_rewrite_data(context, "LF@");
    } else if (operand_type == st_integer){
        if (dest_type == st_double || dest_type == 0) {
            str_rewrite_data(context, "float@");
            str_push_char(operand,'.');
            str_push_char(operand,'0');
        }
        else if (dest_type == st_integer || dest_type == 0) {
            str_rewrite_data(context, "int@");
        }
        else {
            fprintf(stderr, "Wrong types of operands!\n");
            return 1;
        }

    } else if (operand_type == st_double) {
        if (dest_type == st_double || dest_type == 0) {
            str_rewrite_data(context, "float@");
        }
        else if (dest_type == st_integer || dest_type == 0) {
            Tstring res;
            str_create(&res);
            str_rewrite_data(context, "int@");
            int val = converted = nearbyint(strtod(operand->data,NULL));
            sprintf(res.data, "%d", val);
            str_rewrite_data(operand, res.data);
            str_destroy(&res);
        }
        else {
            fprintf(stderr, "Wrong types of operands!\n");
            return 1;
        }
    } else if (operand_type == st_string) {
        if (dest_type == st_string || dest_type == 0)
            str_rewrite_data(context, "string@");
        else {
            fprintf(stderr, "Wrong types of operands!\n");
            return 1;
        }
    } else if (operand_type == 0) {
        str_rewrite_data(context, "");

    }
    return 0;
}

int expr_gen(int operator, char *operand_1, char *operand_2, char *destination, int instruction, Tstate dest_type) {

    int error = 0;

    int operand_1_type = check_operand(operand_1);
    int operand_2_type = check_operand(operand_2);

    Tstring context_1;
    Tstring context_2;

    Tstring convert_op_1;
    Tstring convert_op_2;

    str_create(&context_1);
    str_create(&context_2);

    str_create_init(&convert_op_1, operand_1);
    str_create_init(&convert_op_2, operand_2);

    error = set_context(&context_1, operand_1_type, dest_type, &convert_op_1);
    if (error) {
        str_destroy(&context_1);
        str_destroy(&context_2);
        str_destroy(&convert_op_1);
        str_destroy(&convert_op_2);
        return 1;
    }
    error = set_context(&context_2, operand_2_type, dest_type, &convert_op_2);    
    if (error) {
        str_destroy(&context_1);
        str_destroy(&context_2);
        str_destroy(&convert_op_1);
        str_destroy(&convert_op_2);
        return 1;
    }

    char* context = (p == 0 ? "TF@" : "LF@");

    switch(operator) {
        
        case ex_mul:
        {
            asprintf(&str, "MUL %s %s%s %s%s\n", destination, context_1.data, convert_op_1.data, context_2.data, convert_op_2.data);
            add_inst_to_string(str);
            break;
        }

        case ex_div:
        {
            asprintf(&str, "DIV %s %s%s %s%s\n", destination, context_1.data, convert_op_1.data, context_2.data, convert_op_2.data);
            add_inst_to_string(str);
            break;
        }

        case ex_plus:
        {   
            if (operand_1_type == st_string) {
                asprintf(&str, "CONCAT GF@&pomString GF@&pomString %s%s\n", context_2.data, convert_op_2.data);
                add_inst_to_string(str);
            } else {
                asprintf(&str, "ADD %s %s%s %s%s\n", destination, context_1.data, convert_op_1.data, context_2.data, convert_op_2.data);
                add_inst_to_string(str);
            }
            break;
        }

        case ex_minus:
        {
            asprintf(&str, "SUB %s %s%s %s%s\n", destination, context_1.data, convert_op_1.data, context_2.data, convert_op_2.data);
            add_inst_to_string(str);
            break;
        }

        case ex_equal:
        {
            asprintf(&str, "EQ GF@&pomBool %s%s %s%s\n", context_1.data, convert_op_1.data, context_2.data, convert_op_2.data);
            add_inst_to_string(str);
            break;
        }

        case ex_less:
        {
            asprintf(&str, "LT GF@&pomBool %s%s %s%s\n", context_1.data, convert_op_1.data, context_2.data, convert_op_2.data);
            add_inst_to_string(str);
            break;
        }

        case ex_great:
        {
            asprintf(&str, "GT GF@&pomBool %s%s %s%s\n", context_1.data, convert_op_1.data, context_2.data, convert_op_2.data);
            add_inst_to_string(str);
            break;
        }

        default:
            fprintf(stderr, "Chyba v code_gen_expres %s%s %s%s %s%s\n", context, destination, context_1.data, convert_op_1.data, context_2.data, convert_op_2.data);

    }
    str_destroy(&convert_op_1);
    str_destroy(&convert_op_2);
    str_destroy(&context_1);
    str_destroy(&context_2);
    return 0;
}