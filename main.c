#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "strings.h"
#include "stack.h"

void DBG_SPrint(TStack *s){
    const char* oper_strings[] = {"*", "/", "\\", "+", "-", "=", "<>", "<", "<=", ">", ">=", "(", ")", "i", "num", "str", "bool", "$", "R", "]",};
    TSElem *elemPtr;
    elemPtr = s->topPtr;
    printf("|TOP--->|%s", oper_strings[elemPtr->type]);
    while(elemPtr->prevPtr != NULL){
        elemPtr = elemPtr->prevPtr;
        printf("|%s", oper_strings[elemPtr->type]);
    }
    printf("|  SYMBOL\n");

    elemPtr = s->topPtr;
    printf("|TOP--->|%s", elemPtr->string.data);
    while(elemPtr->prevPtr != NULL){
        elemPtr = elemPtr->prevPtr;
        printf("|%s", elemPtr->string.data);
    }
      printf("|  STRING\n");
}

int expresion_reduction(TStack *s, Tstring *ret_string) 
{
    str_rewrite_data(ret_string, STopString(s));

    SPop(s);
    return 0;
}

int precedence_analysis() 
{
    TStack stack;
    SInit(&stack);

    SPush(&stack, 1, "jedna");
    SPush(&stack, 2, "dva");

    Tstring ret_string;
    str_create(&ret_string);

    expresion_reduction(&stack, &ret_string);
    printf("reduced_string: %s\n", ret_string.data);

    str_destroy(&ret_string);
    SClean(&stack);

}

int main() 
{ // parser

    precedence_analysis();

    return 0;
}
