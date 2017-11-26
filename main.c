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
 

int main() 
{
    TStack s;
    SInit(&s);

    SPush(&s, 1, "a");
    const char * top = STopString(&s);
    SPop(&s);
    printf("tady '%s'\n", top);

    return 0;
}
