#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "strings.h"
#include "stack.h"

int main() 
{

    TStack s;
    TStack p;

    SInit(&s);
    SInit(&p);

    SPush(&s, ex_mul, "*");
    //printf("znak: %d\n", STopType(&s));

    const char * str;
    str = STopString(&s);
    printf("str:%s\n",str);


    SPush(&p, STopType(&s), STopString(&s));

    const char * str2;
    str2 = STopString(&p);
    printf("str:%s\n",str);

    SPop(&p);
    str2 = STopString(&p);
    if (str2 == NULL){
        printf("prazdnej\n");
    }

    //gen(STopString(&s));

    SPop(&s);

    return 0;
}
